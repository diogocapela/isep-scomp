#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <assert.h>

#define NUMBER_OF_THREADS 50
#define ARRAY_SIZE 10000

int data[ARRAY_SIZE];
int result[ARRAY_SIZE];
pthread_mutex_t mutex;
pthread_cond_t turn;
int current_index;

void *fill_result(void *arg)
{

    int *index = (int *)arg;
    int i;
    for (i = *index * (ARRAY_SIZE / NUMBER_OF_THREADS); i < (*index + 1) * (ARRAY_SIZE / NUMBER_OF_THREADS); i++)
    {
        result[i] = data[i] * 2 + 10;
    }

    if (pthread_mutex_lock(&mutex) != 0)
    {
        perror("lock not successful");
    }

    while (*index != current_index)
    {
        pthread_cond_wait(&turn, &mutex);
        pthread_cond_signal(&turn);
    }

    for (i = *index * (ARRAY_SIZE / NUMBER_OF_THREADS); i < (*index + 1) * (ARRAY_SIZE / NUMBER_OF_THREADS); i++)
    {
        printf("index %d: %d\n", i, result[i]);
        fflush(stdout);
    }

    current_index++;

     pthread_cond_signal(&turn);
    if (pthread_mutex_unlock(&mutex) != 0)
    {
        perror("unlock not successful");
    }
    

    pthread_exit((void *)NULL);
}

int main()
{

    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        data[i] = 1;
    }

    pthread_cond_init(&turn, NULL);
    pthread_mutex_init(&mutex, NULL);
    current_index = 0;

    pthread_t thread_id[NUMBER_OF_THREADS];

    int position[NUMBER_OF_THREADS];

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        position[i] = i;
        pthread_create(&thread_id[i], NULL, fill_result, (void *)&position[i]);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(thread_id[i], (void *)NULL);
    }

    pthread_cond_destroy(&turn);
    pthread_mutex_destroy(&mutex);

    return 0;
}