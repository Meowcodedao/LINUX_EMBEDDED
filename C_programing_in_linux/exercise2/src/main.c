#include <stdio.h>
#include <string.h>
#include "stringutils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }
    char s1[256], s2[256];
    strncpy(s1, argv[1], sizeof(s1) - 1);
    s1[sizeof(s1) - 1] = '\0';
    strncpy(s2, argv[1], sizeof(s2) - 1);
    s2[sizeof(s2) - 1] = '\0';

    printf("Original  : %s\n", argv[1]);
    printf("Length    : %d\n", str_len(argv[1]));
    str_to_upper(s1);
    printf("Uppercase : %s\n", s1);
    str_reverse(s2);
    printf("Reversed  : %s\n", s2);
    return 0;
}
