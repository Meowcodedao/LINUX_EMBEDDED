#include <stdio.h>
#include "mathutils.h"

int main(void) {
    int a, b, n;
    printf("Enter two integers: ");
    scanf("%d %d", &a, &b);
    printf("Add: %d + %d = %d\n", a, b, math_add(a, b));
    printf("Sub: %d - %d = %d\n", a, b, math_sub(a, b));
    printf("Enter a non-negative integer for factorial: ");
    scanf("%d", &n);
    printf("Factorial of %d = %ld\n", n, math_factorial(n));
    return 0;
}
