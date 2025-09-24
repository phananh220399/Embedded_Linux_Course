#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



#define NUMB_READ 5
#define NUMB_WRITE 2

int shared_counter = 0;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

static void *reader(void *args)
{
    for(int i = 0 ; i< NUMB_READ ; i++)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("Value Read: %d\n",shared_counter);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

static void *writer(void *args)
{
    for(int i = 0 ; i< NUMB_WRITE ; i++)
    {
        pthread_rwlock_wrlock(&rwlock);
        shared_counter ++;
        printf("Value Write: %d\n",shared_counter);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t p_read[NUMB_READ], p_write[NUMB_WRITE];

    pthread_rwlock_init(&rwlock, NULL);

    for(int i = 0; i<NUMB_READ; i++)
    {
        if (pthread_create(&p_read[i],NULL,reader,NULL) != 0){
            perror("Failed to create thread\n");
            return 1;
    }
}
    for(int i = 0; i<NUMB_WRITE; i++)
    {
        if (pthread_create(&p_write[i],NULL,writer,NULL) != 0){
            perror("Failed to create thread\n");
            return 1;
    }
}

    for(int i = 0; i<NUMB_READ; i++)
    {
        pthread_join(p_read[i],NULL);
    }

    for(int i = 0; i<NUMB_WRITE; i++)
    {
        pthread_join(p_write[i],NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    printf("Main Thread Finished\n");
    
    return 0;
}