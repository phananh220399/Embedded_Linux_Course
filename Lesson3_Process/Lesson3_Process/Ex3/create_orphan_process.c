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
        printf("My PID: %d\n", getpid());
        
        for(int i = 0; i< 10 ; i++){
        printf("My Parent PID: %d\n", getppid());
        sleep(1);
        }

    }


    else if(pid >0)                     /*  Parent Process  */
    {
        printf("Parent PID : %d\n", getpid());
        exit(0);
    }

    return 0;
}