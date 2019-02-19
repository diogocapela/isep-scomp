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

#define NUMBER_OF_THREADS 3
#define NUMBER_OF_SIGNALS 1
#define NUMBER_OF_PROVAS 20
#define FINISH_PROVA 0

typedef struct
{
    int nota1;
    int nota2;
    int nota3;
    int notaFinal;
} prova;

prova provas[NUMBER_OF_PROVAS];

int new_prova = 0;
int current_index = 0;
int pos;
int neg;
time_t t;
pthread_mutex_t mutex[NUMBER_OF_SIGNALS];
pthread_cond_t sinal[NUMBER_OF_SIGNALS];

void *fill_provas(void *arg)
{
    int i;
    for (i = 0; i < NUMBER_OF_PROVAS; i++)
    {
        if (pthread_mutex_lock(&mutex[FINISH_PROVA]) != 0)
        {
            perror("lock not successful");
        }
        provas[i].nota1 = (rand() % 100);
        provas[i].nota2 = (rand() % 100);
        provas[i].nota3 = (rand() % 100);
        new_prova = 1;

        printf("escrevi a prova %d\n",i);
        pthread_cond_signal(&sinal[FINISH_PROVA]);
        if (pthread_mutex_unlock(&mutex[FINISH_PROVA]) != 0)
        {
            perror("unlock not successful");
        }
    }
    pthread_exit((void *)NULL);
}

void *check_grade(void *arg)
{
    int sum;
    int final_grade;
    while (current_index != NUMBER_OF_PROVAS)
    {
        if (pthread_mutex_lock(&mutex[FINISH_PROVA]) != 0)
        {
            perror("lock not successful");
        }
        while (new_prova != 1)
        {
            pthread_cond_wait(&sinal[FINISH_PROVA], &mutex[FINISH_PROVA]);
        }
        sum = provas[current_index].nota1 + provas[current_index].nota2 + provas[current_index].nota3;
        final_grade = sum / 3;
        if (sum > 50)
        {
            pos++;
        }
        else
        {
            neg++;
        }
        provas[current_index].notaFinal = final_grade;
        printf("calculei a prova %d\n",current_index);
        new_prova = 0;
        current_index++;
        if (pthread_mutex_unlock(&mutex[FINISH_PROVA]) != 0)
        {
            perror("unlock not successful");
        }
    }
    pthread_exit((void *)NULL);
}

int main()
{

    srand((unsigned)time(&t));

    pos = 0;
    neg = 0;

    int i;
    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        pthread_cond_init(&sinal[i], NULL);
        pthread_mutex_init(&mutex[i], NULL);
    }
    pthread_t thread_id[NUMBER_OF_THREADS];

    pthread_create(&thread_id[0], NULL, fill_provas, (void *)NULL);
    pthread_create(&thread_id[1], NULL, check_grade, (void *)NULL);
    pthread_create(&thread_id[2], NULL, check_grade, (void *)NULL);

    for (i = 0; i < NUMBER_OF_THREADS; i++)
    {
        pthread_join(thread_id[i], (void *)NULL);
    }
    printf("threads acabaram\n");
    for (i = 0; i < NUMBER_OF_SIGNALS; i++)
    {
        pthread_cond_destroy(&sinal[i]);
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}