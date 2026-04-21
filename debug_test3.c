#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int main() {
    void *p = malloc(128 * 1024 * 1024);  // 128MB
    printf("Memory pool at: %p\n", p);

    int ret = init_page(p, 128 * 1024 / 4);  // 32K pages
    printf("init_page returned: %d\n", ret);

    // Phase 2 simulation
    printf("\nPhase 2 simulation:\n");
    void *q = p;
    for (int pgIdx = 0; pgIdx < 10; pgIdx++, q = q + 4096) {
        void *r = alloc_pages(1);
        printf("Expected: %p, Got: %p, Match: %s\n", q, r, (r == q) ? "YES" : "NO");
    }

    return 0;
}