#include <iostream>

#include "fib.h"

#ifndef TEST
int main() {
    int n = 0;
    printf("Enter a number: ");
    scanf("%d", &n);

    int ret = fib(n);

    printf("Sum of Fibonacci series up to %d is %d\n", n, ret);
    return 0;
}
#endif