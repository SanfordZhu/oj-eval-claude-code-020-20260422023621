#include "buddy.h"
#include <stddef.h>

#define NULL ((void *)0)
#define PAGE_SIZE 4096
#define MAX_RANK 16

// Structure to represent a free block
typedef struct free_block {
    struct free_block *next;
} free_block_t;

// Global variables
static void *memory_pool = NULL;
static int total_pages = 0;
static free_block_t *free_lists[MAX_RANK + 1];  // 1-indexed
static int block_rank[(1 << 20)];  // Track block ranks (allocated or free)

// Helper functions
static int get_block_index(void *ptr) {
    return ((char *)ptr - (char *)memory_pool) / PAGE_SIZE;
}

static void *get_block_address(int index) {
    return (char *)memory_pool + index * PAGE_SIZE;
}

static int get_buddy_index(int block_index, int rank) {
    int block_size = 1 << (rank - 1);
    return block_index ^ block_size;
}

static void add_to_free_list(void *ptr, int rank) {
    free_block_t *block = (free_block_t *)ptr;
    block->next = free_lists[rank];
    free_lists[rank] = block;

    // Mark all pages in this block with the rank
    int index = get_block_index(ptr);
    int size = 1 << (rank - 1);
    for (int i = 0; i < size; i++) {
        block_rank[index + i] = rank;
    }
}

int init_page(void *p, int pgcount) {
    if (!p || pgcount <= 0) {
        return EINVAL;
    }

    memory_pool = p;
    total_pages = pgcount;

    // Initialize all free lists
    for (int i = 1; i <= MAX_RANK; i++) {
        free_lists[i] = NULL;
    }

    // Clear block rank tracking
    for (int i = 0; i < pgcount; i++) {
        block_rank[i] = 0;
    }

    // Add all pages to rank 1 free list (in reverse order for sequential allocation)
    for (int i = pgcount - 1; i >= 0; i--) {
        void *page_ptr = get_block_address(i);
        add_to_free_list(page_ptr, 1);
    }

    return OK;
}

void *alloc_pages(int rank) {
    if (rank < 1 || rank > MAX_RANK) {
        return ERR_PTR(-EINVAL);
    }

    // Find the smallest available block
    int current_rank = rank;
    while (current_rank <= MAX_RANK && free_lists[current_rank] == NULL) {
        current_rank++;
    }

    if (current_rank > MAX_RANK) {
        return ERR_PTR(-ENOSPC);
    }

    // Split larger blocks if necessary
    while (current_rank > rank) {
        // Remove block from current rank
        free_block_t *block = free_lists[current_rank];
        free_lists[current_rank] = block->next;

        // Split into two buddies
        int block_size = 1 << (current_rank - 2); // Size of each half in pages
        void *buddy = (char *)block + block_size * PAGE_SIZE;

        // Add both halves to lower rank
        add_to_free_list(buddy, current_rank - 1);
        add_to_free_list(block, current_rank - 1);

        current_rank--;
    }

    // Now we have a block of the exact rank needed
    free_block_t *allocated_block = free_lists[rank];
    free_lists[rank] = allocated_block->next;

    // Mark as allocated
    int index = get_block_index(allocated_block);
    int size = 1 << (rank - 1);
    for (int i = 0; i < size; i++) {
        block_rank[index + i] = -rank;  // Negative means allocated
    }

    return allocated_block;
}

long return_pages(void *p) {
    if (!p || !memory_pool) {
        return EINVAL;
    }

    // Check if pointer is within bounds and aligned
    if ((char *)p < (char *)memory_pool ||
        (char *)p >= (char *)memory_pool + total_pages * PAGE_SIZE ||
        ((char *)p - (char *)memory_pool) % PAGE_SIZE != 0) {
        return EINVAL;
    }

    int block_index = get_block_index(p);

    // Check if it's allocated
    if (block_rank[block_index] >= 0) {
        return EINVAL;  // Not allocated
    }

    int rank = -block_rank[block_index];
    if (rank < 1 || rank > MAX_RANK) {
        return EINVAL;
    }

    // Clear allocation markers
    int size = 1 << (rank - 1);
    for (int i = 0; i < size; i++) {
        block_rank[block_index + i] = 0;
    }

    // Try to merge with buddy
    void *current_ptr = p;
    int current_rank = rank;
    int current_index = block_index;

    while (current_rank < MAX_RANK) {
        int buddy_index = get_buddy_index(current_index, current_rank);

        // Check if buddy exists and is free with the same rank
        if (buddy_index >= total_pages || buddy_index < 0) {
            break;
        }

        int buddy_rank = block_rank[buddy_index];
        if (buddy_rank != current_rank) {
            break;
        }

        // Remove buddy from free list
        free_block_t **prev = &free_lists[current_rank];
        free_block_t *curr = free_lists[current_rank];
        void *buddy_ptr = get_block_address(buddy_index);

        while (curr) {
            if (curr == buddy_ptr) {
                *prev = curr->next;
                break;
            }
            prev = &curr->next;
            curr = curr->next;
        }

        // Merge - keep the lower address
        if (current_index > buddy_index) {
            current_ptr = buddy_ptr;
            current_index = buddy_index;
        }
        current_rank++;
    }

    // Add to free list
    add_to_free_list(current_ptr, current_rank);

    return (long)OK;
}

int query_ranks(void *p) {
    if (!p || !memory_pool) {
        return EINVAL;
    }

    // Check if pointer is within bounds and aligned
    if ((char *)p < (char *)memory_pool ||
        (char *)p >= (char *)memory_pool + total_pages * PAGE_SIZE ||
        ((char *)p - (char *)memory_pool) % PAGE_SIZE != 0) {
        return EINVAL;
    }

    int block_index = get_block_index(p);

    // If allocated, return its rank
    if (block_rank[block_index] < 0) {
        return -block_rank[block_index];
    }

    // If not allocated, find the rank of the containing free block
    return block_rank[block_index];
}

int query_page_counts(int rank) {
    if (rank < 1 || rank > MAX_RANK) {
        return EINVAL;
    }

    int count = 0;
    free_block_t *curr = free_lists[rank];
    while (curr) {
        count += 1 << (rank - 1);
        curr = curr->next;
    }

    return count;
}