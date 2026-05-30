#include "mathutils.h"

long math_factorial(int n) {
    if (n <= 1) return 1;
    return (long)n * math_factorial(n - 1);
}
