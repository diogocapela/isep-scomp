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

#define NUMBER_OF_THREADS 6
#define NUMBER_OF_INITIAL_THREADS 3
#define NUMBER_OF_SIGNALS 3
#define NUMBER_OF_SUPERMARKETS 3
#define NUMBER_OF_PRODUCTS 5
#define MAXIMUM_PRICE 100
#define MINIMUM_PRICE 20
#define SIZE_OF_ARRAY 1000

int flag[] = {0, 0, 0};
typedef struct
{
    int id_h;
    int id_p;
    int product_price;
} product_info;

time_t t;
int lowest_cost = INT_MAX;
int super_id = 0;

product_info vec[SIZE_OF_ARRAY];
product_info vec1[SIZE_OF_ARRAY];
product_info vec2[SIZE_OF_ARRAY];
product_info vec3[SIZE_OF_ARRAY];
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];
pthread_cond_t sinal[NUMBER_OF_SIGNALS];

void fill_array()
{
    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++)
    {
        vec[i].id_h = (rand() % NUMBER_OF_SUPERMARKETS) + 1;
        vec[i].id_p = (rand() % NUMBER_OF_PRODUCTS) + 1;
        vec[i].product_price = (rand() % (MAXIMUM_PRICE - MINIMUM_PRICE + 1)) + MINIMUM_PRICE;
    }
}

void *fill_vec(void *arg)
{
    int index = (int)arg;

    if (pthread_mutex_lock(&mutex[index - 1]) != 0)
    {
        perror("lock not successful");
    }

    product_info *vec_ref;

    if (index == 1)
    {
        vec_ref = vec1;
    }
    if (index == 2)
    {
        vec_ref = vec2;
    }
    if (index == 3)
    {
        vec_ref = vec3;
    }

    int count = 0;
    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++)
    {
        if (vec[i].id_h == index)
        {
            vec_ref[count].id_h = vec[i].id_h;
            vec_ref[count].id_p = vec[i].id_p;
            vec_ref[count].product_price = vec[i].product_price;
        }
        count++;
    }
    flag[index - 1]++;
    pthread_cond_signal(&sinal[index - 1]);
    if (pthread_mutex_unlock(&mutex[index - 1]) != 0)
    {
        perror("unlock not successful");
    }

    pthread_exit((void *)NULL);
}

void *compute_vec(void *arg)
{
    int index = (int)arg;
    if (pthread_mutex_lock(&mutex[index - 1]) != 0)
    {
        perror("lock not successful");
    }
    while (flag[index - 1] != 1)
    {
        pthread_cond_wait(&sinal[index - 1], &mutex[index - 1]);
    }

    product_info *vec_ref;
    if (index == 1)
    {
        vec_ref = vec1;
    }
    if (index == 2)
    {
        vec_ref = vec2;
    }
    if (index == 3)
    {
        vec_ref = vec3;
    }
    int price_sum[NUMBER_OF_PRODUCTS];
    int count_prod[NUMBER_OF_PRODUCTS];
    int k;

    for (k = 0; k < NUMBER_OF_PRODUCTS + 1; k++)
    {
        price_sum[k] = 0;
        count_prod[k] = 0;
    }

    int i;

    for (i = 0; i < SIZE_OF_ARRAY; i++)
    {
        for (k = 1; k < NUMBER_OF_PRODUCTS + 1 + 1; k++)
        {
            if (vec_ref[i].id_p == k)
            {
                price_sum[k - 1] += vec_ref[i].product_price;
                count_prod[k - 1]++;
                break;
            }
        }
    }

    int avg_prod[NUMBER_OF_PRODUCTS];
    int sum_prod = 0;

    for (k = 1; k < NUMBER_OF_PRODUCTS + 1; k++)
    {
        avg_prod[k] = price_sum[k] / count_prod[k];
        sum_prod += avg_prod[k];
    }
    index = (int)arg;
    if (sum_prod < lowest_cost)
    {
        lowest_cost = sum_prod;
        super_id = index;
    }

    if (pthread_mutex_unlock(&mutex[index - 1]) != 0)
    {
        perror("unlock not successful");
    }
    pthread_exit((void *)NULL);
}

int main()
{

    srand((unsigned)time(&t));
    fill_array();
    int i;
    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        pthread_cond_init(&sinal[i], NULL);
        pthread_mutex_init(&mutex[i], NULL);
    }
    pthread_t thread_id[NUMBER_OF_THREADS];
    int position[NUMBER_OF_INITIAL_THREADS];

    for (i = 0; i < NUMBER_OF_INITIAL_THREADS; i++)
    {
        position[i] = i + 1;
        pthread_create(&thread_id[i], NULL, fill_vec, (void *)position[i]);
    }

    /*pthread_create(&thread_id[0], NULL, fill_vec1, (void *)NULL);
    pthread_create(&thread_id[1], NULL, fill_vec2, (void *)NULL);
    pthread_create(&thread_id[2], NULL, fill_vec3, (void *)NULL);*/

    int position2[NUMBER_OF_THREADS - NUMBER_OF_INITIAL_THREADS];
    for (i = NUMBER_OF_INITIAL_THREADS; i < NUMBER_OF_THREADS; i++)
    {
        position2[i - NUMBER_OF_INITIAL_THREADS] = i - NUMBER_OF_INITIAL_THREADS + 1;
        pthread_create(&thread_id[i], NULL, compute_vec, (void *)position2[i - NUMBER_OF_INITIAL_THREADS]);
    }
    /*pthread_create(&thread_id[i], NULL, compute_vec1, (void *)NULL);
    pthread_create(&thread_id[i], NULL, compute_vec2, (void *)NULL);
    pthread_create(&thread_id[i], NULL, compute_vec3, (void *)NULL);*/

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(thread_id[i], (void *)NULL);
    }

    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        pthread_cond_destroy(&sinal[i]);
        pthread_mutex_destroy(&mutex[i]);
    }

    printf("The supermarket with the lowest prices is %d and the avg price is %d\n", super_id, lowest_cost);

    return 0;
}