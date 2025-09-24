#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define NUMB 10

int data = 0;
int data_ready = 0;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void *producer(void *args)
{
    for(int i = 0; i < NUMB ; i++)
    {
        pthread_mutex_lock(&mtx);
        data = rand() % 100;
        data_ready = 1; 
        printf("The value data of Producer: %d\n",data);

        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);

        sleep(1);
    }
    return NULL;
}

static void *consumer(void *args)
{
    for(int i = 0; i < NUMB ; i++)
    {
        pthread_mutex_lock(&mtx);
        while(!data_ready)
        {
            pthread_cond_wait(&cond, &mtx);
        }
        printf("The value data of CONSUMER read: %d\n", data);
        data_ready = 0;
        pthread_mutex_unlock(&mtx);
        
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t prod_t , cons_t;

    if (pthread_create(&prod_t,NULL,producer,NULL) != 0){
            perror("Failed to create thread\n");
            return 1;
        }
    
    if (pthread_create(&cons_t,NULL,consumer,NULL) != 0){
            perror("Failed to create thread\n");
            return 1;
        }    

    pthread_join(prod_t, NULL);
    pthread_join(cons_t, NULL);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
    
    return 0;
}

