#include <stdio.h>

int pow2f(int n) {
    if (n <= 0) return 1;
    return 2 * pow2f(n - 1);
}

int main() {
    for (int i = -2; i <= 5; i++) {
        printf("pow2f(%d) = %d\n", i, pow2f(i));
    }
    return 0;
}