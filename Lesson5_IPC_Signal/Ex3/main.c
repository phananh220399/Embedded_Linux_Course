#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


void handle_sigusr(int sig)
{
    printf("Received signal from parent\n");
}


int main(int argc, char const *argv[])
{
    pid_t pid;
    int status;

    pid = fork();

    if (0 == pid)            /* Children Process */
    {
        signal(SIGUSR1,handle_sigusr);
        while(1) pause;
    }

    else if (pid > 0)       /* Parent Process   */
    {
        for(int i =0; i<5 ; i++)
        {
            sleep(2);
            kill(pid,SIGUSR1);
        }

    }
  

}