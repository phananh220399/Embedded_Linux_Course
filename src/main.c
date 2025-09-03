#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "strutils.h"

int main() {
    char s1[] = "  Hello World  ";
    char s2[] = "abcdef";
    char s3[] = "12345";
    int number;

    printf("Original s1: '%s'\n", s1);
    str_trim(s1);
    printf("Trimmed s1: '%s'\n\n", s1);

    printf("Original s2: '%s'\n", s2);
    str_reverse(s2);
    printf("Reversed s2: '%s'\n\n", s2);

    if (str_to_int(s3, &number)) {
        printf("Converted '%s' to int = %d\n", s3, number);
    } else {
        printf("Failed to convert '%s' to int\n", s3);
    }

    return 0;
}
