#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "strutils.h"


void str_reverse(char *str) {
    if (!str) return;
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
}


void str_trim(char *str) {
    if (!str) return;


    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    // Trim cuối
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';


    memmove(str, start, strlen(start) + 1);
}


int str_to_int(const char *str, int *out) {
    if (!str || !out) return 0;

    char *endptr;
    long val = strtol(str, &endptr, 10);

    if (*endptr != '\0') {
        return 0; 
    }
    *out = (int)val;
    return 1;
}
