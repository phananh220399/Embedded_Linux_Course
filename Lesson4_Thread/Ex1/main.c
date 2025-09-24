#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


pthread_t tid1, tid2;

// typedef struct {
//     char name[30];
//     char msg[30];
// } thr_data_t;


static void *thr_handler(void *args)
{
    if (pthread_equal(pthread_self(), tid1) != 0) 
    {
        printf("Thread with ID <%lu> running\n", pthread_self());
    } 
    else {
        printf("Thread with ID <%lu> running\n", pthread_self());
    }

}

int main(int argc, char const *argv[])
{
    int ret;
    

    if (ret = pthread_create(&tid1, NULL, &thr_handler, NULL)) {
        printf("pthread_create() error number-%d\n", ret);
        return -1;
    }
    if (ret = pthread_create(&tid2, NULL, &thr_handler, NULL)) {
        printf("pthread_create() error number=%d\n", ret);
        return -1;
    }

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    sleep(1);

    printf("Main Thread Finished\n");

    return 0;
}

