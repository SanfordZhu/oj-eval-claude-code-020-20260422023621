#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

#define TESTSIZE (128)
#define MAXRANK0PAGE (TESTSIZE * 1024 / 4)

int main() {
    printf("MAXRANK0PAGE = %d\n", MAXRANK0PAGE);

    void *p = malloc(TESTSIZE * 1024 * 1024);  // 128MB
    printf("Memory pool at: %p\n", p);

    int ret = init_page(p, MAXRANK0PAGE);
    printf("init_page returned: %d\n", ret);

    // Check first allocation
    void *r = alloc_pages(1);
    printf("First alloc_pages(1) returned: %p (expected: %p)\n", r, p);

    // Check query_page_counts
    printf("\nQuery page counts after first allocation:\n");
    for (int i = 1; i <= 5; i++) {
        printf("query_page_counts(%d) = %d\n", i, query_page_counts(i));
    }

    return 0;
}