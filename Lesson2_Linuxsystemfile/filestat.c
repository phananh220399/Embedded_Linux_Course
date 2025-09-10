#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <myfile>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    struct stat sb;

    if (lstat(path, &sb) == -1) {
        perror("lstat");
        return 1;
    }

    printf("File Path: %s\n", path);

    printf("File Type: ");
    if (S_ISREG(sb.st_mode)) {
        printf("Regular File\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("Symbolic Link\n");
    } else {
        printf("Other\n");
    }


    printf("Size: %lld bytes\n", (long long) sb.st_size);


    char timebuf[64];
    struct tm *tm_info = localtime(&sb.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Last Modified: %s\n", timebuf);

    return 0;
}