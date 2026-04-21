#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int pow2f(int n) {
    if (n <= 0) return 1;
    return 2 * pow2f(n - 1);
}

int main() {
    void *p = malloc(128 * 1024 * 1024);
    init_page(p, 128 * 1024 / 4);

    printf("Phase 8A detailed debug:\n");

    // Step 1: Allocate all pages
    printf("\n1. Allocating all pages...\n");
    for (int i = 0; i < 32768; i++) {
        alloc_pages(1);
    }
    printf("   query_page_counts(1) = %d\n", query_page_counts(1));

    // Step 2: Return pages for currentRank=1
    printf("\n2. For currentRank=1:\n");
    void *q = p;
    int returnPages = pow2f(1 - 2);  // pow2f(-1) = 1
    printf("   returnPages = %d\n", returnPages);

    for (int i = 0; i < returnPages; i++) {
        printf("   Returning page at %p\n", q);
        return_pages(q);
        q += 4096;
    }

    printf("   After returning:\n");
    for (int rank = 1; rank <= 3; rank++) {
        printf("   query_page_counts(%d) = %d\n", rank, query_page_counts(rank));
    }

    // Step 3: Return pages for currentRank=2
    printf("\n3. For currentRank=2:\n");
    returnPages = pow2f(2 - 2);  // pow2f(0) = 1
    printf("   returnPages = %d\n", returnPages);

    for (int i = 0; i < returnPages; i++) {
        printf("   Returning page at %p\n", q);
        return_pages(q);
        q += 4096;
    }

    printf("   After returning:\n");
    for (int rank = 1; rank <= 3; rank++) {
        printf("   query_page_counts(%d) = %d\n", rank, query_page_counts(rank));
    }

    // Step 4: Return pages for currentRank=3
    printf("\n4. For currentRank=3:\n");
    returnPages = pow2f(3 - 2);  // pow2f(1) = 2
    printf("   returnPages = %d\n", returnPages);

    for (int i = 0; i < returnPages; i++) {
        printf("   Returning page at %p\n", q);
        return_pages(q);
        q += 4096;
    }

    printf("   After returning:\n");
    for (int rank = 1; rank <= 3; rank++) {
        printf("   query_page_counts(%d) = %d", rank, query_page_counts(rank));
        if (rank < 3) {
            printf(" (expected: 0)");
        } else {
            printf(" (expected: 1)");
        }
        printf("\n");
    }

    return 0;
}