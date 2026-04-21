#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "buddy.h"
#include "utils.h"
int fake_mode = 0;
int cont = 0;  // Stop on error

#define MAXRANK (16)
#define TESTSIZE (128)
#define MAXRANK0PAGE (TESTSIZE * 1024 / 4)
int tCnt = 0;

int main() {
    void *p, *q;
    int pgIdx;

    printf("Unit test suite: \n");
    {
        printf("Phase 1 : \n");
        p = malloc(TESTSIZE * sizeof(char) * 1024 * 1024);
        int ret = init_page(p, MAXRANK0PAGE);
        ok(ret == OK);
    }
    {
        printf("Phase 2: alloc pages\n");
        tCnt = 0;
        q = p;
        for (pgIdx = 0; pgIdx < MAXRANK0PAGE; pgIdx++, q = q + 1024 * 4) {
            void *r = alloc_pages(1);
            if (r != q) {
                printf("\nFAILURE at iteration %d:\n", pgIdx);
                printf("  Expected: %p\n", q);
                printf("  Got: %p\n", r);
                printf("  Difference: %ld pages\n", ((char *)r - (char *)q) / 4096);
                fflush(stdout);
            }
            dotOk(r == q);
        }
        dotDone();
    }

    return 0;
}