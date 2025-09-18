#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc , char const *argv[])
{
    pid_t pid;
    int status;
    
    pid = fork();


    if(0 == pid)                         /*  Child Process   */
    {
        printf("Child PID : %d\n", getpid());
        exit(0);
    }


    else if(pid >0)                     /*  Parent Process  */
    {
        sleep(100);
        printf("Parent PID : %d\n", getpid());

    }

    return 0;
}