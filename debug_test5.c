#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

#define MAXRANK (16)
#define TESTSIZE (128)
#define MAXRANK0PAGE (TESTSIZE * 1024 / 4)

int main() {
    void *p, *q;
    int pgIdx;

    printf("Starting test simulation...\n");
    p = malloc(TESTSIZE * sizeof(char) * 1024 * 1024);
    printf("Allocated memory pool at: %p\n", p);

    int ret = init_page(p, MAXRANK0PAGE);
    printf("init_page returned: %d\n", ret);

    printf("\nPhase 2 simulation:\n");
    q = p;
    for (pgIdx = 0; pgIdx < 10; pgIdx++, q = q + 1024 * 4) {
        void *r = alloc_pages(1);
        printf("pgIdx=%d: q=%p, r=%p, match=%s\n", pgIdx, q, r, (r == q) ? "YES" : "NO");
        if (r != q) {
            printf("MISMATCH! Stopping.\n");
            break;
        }
    }

    return 0;
}