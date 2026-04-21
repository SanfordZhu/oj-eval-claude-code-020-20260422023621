#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

#define MAXRANK0PAGE (128 * 1024 / 4)

int main() {
    void *p = malloc(128 * 1024 * 1024);
    printf("Memory pool: %p\n", p);

    init_page(p, MAXRANK0PAGE);

    // Phase 2 test
    void *q = p;
    for (int i = 0; i < MAXRANK0PAGE; i++) {
        void *r = alloc_pages(1);
        if (r != q) {
            printf("ERROR at i=%d: expected %p, got %p\n", i, q, r);
            printf("Difference: %ld pages\n", ((char *)r - (char *)q) / 4096);
            break;
        }
        q = q + 4096;
        if (i % 1000 == 0) {
            printf("Progress: %d/%d\n", i, MAXRANK0PAGE);
        }
    }

    printf("Phase 2 completed\n");
    return 0;
}