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

#define NUMBER_OF_THREADS 5
#define NUMBER_OF_SIGNALS 3
#define NUMBER_OF_STATIONS 4
#define NUMBER_OF_CONNECTIONS 3

typedef struct
{
    char station_name[7];
    int connections[NUMBER_OF_CONNECTIONS];
} station;

time_t t;

station stations[NUMBER_OF_STATIONS];
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];
//pthread_cond_t sinal[NUMBER_OF_SIGNALS];

int get_starting_city(int min_city, int max_city)
{

    return (rand() % max_city) + min_city;
}

int get_next_city(int current_city)
{
    if (current_city != 1)
    {
        //printf("entrei, sou a cidade %d e %d\n", current_city,  stations[current_city].connections[0]);
        return stations[current_city].connections[0];
    }
    else
    {
        int next_city = get_starting_city(1, 3);
        if (next_city == 1)
        {
            return 0;
        }
        else
        {
            return next_city;
        }
    }
}

int get_connection(int starting_city, int next_city)
{
    if (starting_city == 0)
    {
        return 0;
    }
    if (starting_city == 2)
    {
        return 1;
    }
    if (starting_city == 3)
    {
        return 2;
    }
    if (next_city == 0)
    {
        return 0;
    }
    if (next_city == 2)
    {
        return 1;
    }
    if (next_city == 3)
    {
        return 2;
    }
    return -1;
}

void *fill_result(void *arg)
{

    int index = (int)arg;
    int starting_city = get_starting_city(0, 3);
    int next_city = get_next_city(starting_city);
    printf("I am thread %d, i am starting in city %d and i am goind to city %d next\n", index, starting_city, next_city);
    int count =0;
    while (count<4)
    {
        int connection = get_connection(starting_city, next_city);

        if (pthread_mutex_lock(&mutex[connection]) != 0)
        {
            perror("lock not successful");
        }

        printf("sou o comboio %d e estou a fazer a travessia %d\n", index, connection);
        sleep(2);

        if (pthread_mutex_unlock(&mutex[connection]) != 0)
        {
            perror("unlock not successful");
        }
        starting_city= next_city;
        next_city = get_next_city(starting_city);
        count++;
    }

    pthread_exit((void *)NULL);
}

int main()
{

    strcpy(stations[0].station_name, "City A");
    strcpy(stations[1].station_name, "City B");
    strcpy(stations[2].station_name, "City C");
    strcpy(stations[3].station_name, "City D");

    stations[0].connections[0] = 1;
    stations[1].connections[0] = 0;
    stations[1].connections[1] = 2;
    stations[1].connections[2] = 3;
    stations[2].connections[0] = 1;
    stations[3].connections[0] = 1;

    srand((unsigned)time(&t));

    int i;
    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        //pthread_cond_init(&sinal[i], NULL);
        pthread_mutex_init(&mutex[i], NULL);
    }
    pthread_t thread_id[NUMBER_OF_THREADS];

    int position[NUMBER_OF_THREADS];
    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        position[i] = i;
        pthread_create(&thread_id[i], NULL, fill_result, (void *)position[i]);
    }

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(thread_id[i], (void *)NULL);
    }
    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        //pthread_cond_destroy(&sinal[i]);
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}