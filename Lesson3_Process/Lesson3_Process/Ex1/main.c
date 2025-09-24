#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char const *argv[])
{
    pid_t pid;
    int status;

    pid = fork();
    
    if (0 == pid)            /* Children Process */
    {
        printf("\nThe child process:");
        printf("\nMy PID is: %d", getpid());
        exit(10);
    }
    
    else if (pid > 0)       /* Parent Process  */
    {
        printf("\nThe parent process:");
        printf("\nMy PID is: %d, my parent PID is: %d\n", getpid(), pid);
        
        pid_t child_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("\nChild exit with status = %d\n", WEXITSTATUS(status));
        }
        else 
            printf("Failed");   
        
    }       
}