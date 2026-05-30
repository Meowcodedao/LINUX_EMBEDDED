#include "stringutils.h"

void str_reverse(char *s) {
    int len = str_len(s);
    for (int i = 0; i < len / 2; i++) {
        char tmp        = s[i];
        s[i]            = s[len - 1 - i];
        s[len - 1 - i]  = tmp;
    }
}
