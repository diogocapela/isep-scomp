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

#define NUMBER_OF_THREADS 10
#define NUMBER_OF_TICKETS 10000
#define NUMBER_OF_CHOICES 5
#define MAX_NUMBER_LOTTERY 49

int lottery_numbers[NUMBER_OF_TICKETS][NUMBER_OF_CHOICES];
int number_of_picks[MAX_NUMBER_LOTTERY];
pthread_mutex_t mutex;

void *find_statistics(void *arg)
{

    int *index = (int *)arg;

    int i;
    int k;
    for (i = *index * (NUMBER_OF_TICKETS/NUMBER_OF_THREADS); i < (*index + 1) * (NUMBER_OF_TICKETS/NUMBER_OF_THREADS); i++)
    {
        //printf("here again\n");
        if (pthread_mutex_lock(&mutex) != 0)
        {
            perror("lock not successful");
        }
        for (k = 0; k < NUMBER_OF_CHOICES; k++)
        {
            number_of_picks[lottery_numbers[i][k]]++;
        }

        if (pthread_mutex_unlock(&mutex) != 0)
        {
            perror("unlock not successful");
        }
        //printf("im here\n");
    }
    pthread_exit((void *)NULL);
}

void fill_array()
{
    int im;
    int k;
    int i;
    time_t t;
    srand((unsigned)time(&t));
    for (k = 0; k < NUMBER_OF_TICKETS; k++)
    {
        im = 0;
        for (i = 0; i < MAX_NUMBER_LOTTERY && im < NUMBER_OF_CHOICES; ++i)
        {
            int rn = MAX_NUMBER_LOTTERY - i;
            int rm = NUMBER_OF_CHOICES - im;
            if (rand() % rn < rm)
                lottery_numbers[k][im++] = i + 1;
        }
        assert(im == NUMBER_OF_CHOICES);
    }
}

int main()
{
    fill_array();
    int i;
    for (i = 0; i < MAX_NUMBER_LOTTERY; i++)
    {
        number_of_picks[i] = 0;
    }
   

    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_id[NUMBER_OF_THREADS];

    int j;
    int position[NUMBER_OF_THREADS];

    for (j = 0; j < NUMBER_OF_THREADS; j++)
    {   
        position[j] = j;
        pthread_create(&thread_id[j], NULL, find_statistics, (void *)&position[j]);
    }

    for (j = 0; j < NUMBER_OF_THREADS; j++)
    {
        pthread_join(thread_id[j], (void *)NULL);
    }
    pthread_mutex_destroy(&mutex);

    for (i = 0; i < MAX_NUMBER_LOTTERY; i++)
    {
        printf("pick number %d had %d picks\n", i+1, number_of_picks[i]);
    }
    return 0;
}