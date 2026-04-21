#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int main() {
    void *p = malloc(128 * 1024 * 1024);  // 128MB
    printf("Memory pool at: %p\n", p);
    printf("Page size: %d\n", 4096);
    printf("First page should be at: %p\n", p);
    printf("Second page should be at: %p\n", (char *)p + 4096);

    int ret = init_page(p, 128 * 1024 / 4);  // 32K pages
    printf("init_page returned: %d\n", ret);

    // Check what's in the free list
    printf("Checking first few allocations:\n");
    for (int i = 0; i < 5; i++) {
        void *r = alloc_pages(1);
        printf("alloc_pages(1) #%d returned: %p (offset: %ld)\n",
               i+1, r, (char *)r - (char *)p);
    }

    return 0;
}