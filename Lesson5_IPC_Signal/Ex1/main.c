#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int count ;

void handle_signint(int sig)
{
    count ++;
    printf("\nSIGINT received: %d\n",count);

    if(count >= 3)
    {
        printf("Signal Finshised\n");
        exit(0);
    }

}


int main(int argc, char const *argv[])
{
    signal(SIGINT,handle_signint); 
    //signal(SIGINT,SIG_IGN);      // Nothing happen

    while(1);
    printf("Signal Finshised\n");
    return 0;
}