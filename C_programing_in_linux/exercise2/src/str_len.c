#include "stringutils.h"

int str_len(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}
