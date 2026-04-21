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

    printf("Phase 8A simulation:\n");

    // Allocate all pages
    for (int i = 0; i < 32768; i++) {
        alloc_pages(1);
    }
    printf("After allocating all pages:\n");
    printf("  query_page_counts(1) = %d\n", query_page_counts(1));

    // Return pages as per Phase 8A logic
    void *q = p;
    int currentRank = 1;

    while (currentRank <= 5) {  // Just test first few ranks
        int returnPages = pow2f(currentRank - 2);
        printf("\nFor currentRank=%d, returnPages=%d\n", currentRank, returnPages);

        // Return the pages
        for (int i = 0; i < returnPages; i++, q = q + 4096) {
            int ret = return_pages(q);
            if (ret != 0) {
                printf("  return_pages failed at i=%d with %d\n", i, ret);
            }
        }

        // Check all ranks
        for (int rankIdx = 1; rankIdx <= currentRank; rankIdx++) {
            int count = query_page_counts(rankIdx);
            printf("  query_page_counts(%d) = %d", rankIdx, count);
            if (rankIdx < currentRank) {
                printf(" (expected: 0)");
            } else {
                printf(" (expected: 1)");
            }
            printf("\n");
        }

        currentRank++;
    }

    return 0;
}