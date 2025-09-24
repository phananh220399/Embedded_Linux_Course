#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc , char const *argv[])
{
    pid_t pid;
    int status;


    setenv("MY_COMMAND", "ls", 1);
    pid = fork();

    if (0 == pid)            /* Children Process */
    {
        char *cmd = getenv("MY_COMMAND");
        execlp(cmd, cmd, (char *)NULL);
        exit(0);
    }

    else if (pid > 0)       /* Parent Process   */
    {
        printf("Parent: PID = %d, Child PID = %d\n", getpid(), pid);
        pid_t child_pid = wait(&status);
    }

    return 0;
}