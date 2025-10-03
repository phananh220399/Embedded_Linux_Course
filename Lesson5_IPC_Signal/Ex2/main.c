#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int count;

void handle_alarm(int sig)
{
    count ++;
    printf("\nTimer : %d seconds\n",count);

    if(count >= 10 )
    {
        printf("\nTimer Finished\n");
        exit(0);
    }
    alarm(1);                       // maintain state
  
}


int main(int argc, char const *argv[])
{
    signal(SIGALRM,handle_alarm);
    alarm(1);                       // first statels
    while(1);

    printf("Timer Finished");
    return 0;
}