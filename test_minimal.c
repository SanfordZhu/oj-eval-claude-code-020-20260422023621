#include <stdio.h>
#include <stdlib.h>
#include "buddy.h"

int main() {
    // Test 1: Basic allocation
    void *p = malloc(128 * 1024 * 1024);
    init_page(p, 128 * 1024 / 4);

    void *r1 = alloc_pages(1);
    printf("First allocation: %p (expected: %p) - %s\n",
           r1, p, (r1 == p) ? "PASS" : "FAIL");

    void *r2 = alloc_pages(1);
    void *expected2 = p + 4096;
    printf("Second allocation: %p (expected: %p) - %s\n",
           r2, expected2, (r2 == expected2) ? "PASS" : "FAIL");

    // Test 2: Check query_page_counts
    printf("query_page_counts(1) after 2 allocs: %d (expected: 32766) - %s\n",
           query_page_counts(1), (query_page_counts(1) == 32766) ? "PASS" : "FAIL");

    // Test 3: Return pages
    int ret = return_pages(r1);
    printf("return_pages: %d (expected: 0) - %s\n",
           ret, (ret == 0) ? "PASS" : "FAIL");

    printf("query_page_counts(1) after return: %d (expected: 32767) - %s\n",
           query_page_counts(1), (query_page_counts(1) == 32767) ? "PASS" : "FAIL");

    return 0;
}