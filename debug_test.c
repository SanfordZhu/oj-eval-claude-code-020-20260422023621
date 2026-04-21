#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int main() {
    void *p = malloc(128 * 1024 * 1024);  // 128MB
    printf("Memory pool at: %p\n", p);

    int ret = init_page(p, 128 * 1024 / 4);  // 32K pages
    printf("init_page returned: %d\n", ret);

    // Try to allocate first page
    void *r = alloc_pages(1);
    printf("First alloc_pages(1) returned: %p\n", r);
    printf("Expected: %p\n", p);

    return 0;
}