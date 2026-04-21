#include <stdio.h>
#include "buddy.h"

int main() {
    // Test the problematic lines
    void *p = NULL;

    // This is what the test is trying to do
    // int ret = PTR_ERR(return_pages(NULL));

    // Since return_pages returns int, we should check it directly
    int ret = return_pages(NULL);
    printf("return_pages(NULL) = %d\n", ret);
    printf("Expected: %d\n", -EINVAL);

    // The test should be checking ret == -EINVAL, not PTR_ERR(ret) == -EINVAL
    printf("Test would pass if checked directly: %s\n",
           (ret == -EINVAL) ? "YES" : "NO");

    return 0;
}