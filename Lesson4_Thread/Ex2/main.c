#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


pthread_t tid1, tid2;


long long counter = 0;

#define NUM_THREADS 3
#define INCREMENT_PER_THREAD 1000000

pthread_mutex_t lock_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *increment_counter(void *args)
{
    pthread_mutex_lock(&lock_mutex);
    for (int i = 0; i < INCREMENT_PER_THREAD; i++) 
    {   
        counter++;                   
    }
    pthread_mutex_unlock(&lock_mutex); 
    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(&threads[i],NULL,increment_counter,NULL) != 0){
            perror("Failed to create thread");
            return 1;
        }
    }

    for(int i =0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("The Count Value: %lld\n", counter);

}











