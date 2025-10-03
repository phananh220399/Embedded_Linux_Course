#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


void handle_sigint(int sig)
{
    printf("\nSigint received\n");
  
}

void handle_sigterm(int sig)
{
    printf("Finished,Exit Program\n");
    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);

    char buf[256];
    printf("Program PID = %d\n", getpid());
    printf("Nhap du lieu: \n");
    while (1) {
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            printf("Output: %s", buf);
        }
    }
    return 0;
}