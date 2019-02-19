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
#include <pthread.h>

#define INDEX 5
#define NUMBER_TO_FIND 157
#define NUMBER_OF_THREADS 5

int MATRIX1[INDEX][INDEX];
int MATRIX2[INDEX][INDEX];
int FINAL_MATRIX[INDEX][INDEX];



void *fill_matrix1(void *arg)
{
    int i;
    int k;
    for (i = 0; i < INDEX; i++)
    {
        for(k=0; k<INDEX; k++){
            MATRIX1[i][k]=i+1;
        }
    }
    pthread_exit((void *)NULL);
}
void *fill_matrix2(void *arg)
{
    int i;
    int k;
    for (i = 0; i < INDEX; i++)
    {
        for(k=0; k<INDEX; k++){
            MATRIX2[i][k]=i+2;
        }
    }
    pthread_exit((void *)NULL);
}
void *multiply_row(void *arg)
{
    int *line = (int *)arg;

    int i = *line;
    int j;
    int k;
    for (j = 0; j < INDEX; j++)
    {
        for (k = 0; k < INDEX; k++)
        {
            FINAL_MATRIX[i][j] += MATRIX1[i][k] * MATRIX2[k][j];
        }
    }
    pthread_exit((void *)NULL);
}

void print_matrix1()
{
    int i;
    int k;
    for (i = 0; i < INDEX; i++)
    {
        for (k = 0; k < INDEX; k++)
        {
            printf("%d ", MATRIX1[i][k]);
        }
        printf("\n");
    }
}

void print_matrix2()
{
    int i;
    int k;
    for (i = 0; i < INDEX; i++)
    {
        for (k = 0; k < INDEX; k++)
        {
            printf("%d ", MATRIX2[i][k]);
        }
        printf("\n");
    }
}

void print_final_matrix()
{
    int i;
    int k;
    for (i = 0; i < INDEX; i++)
    {
        for (k = 0; k < INDEX; k++)
        {
            printf("%d ", FINAL_MATRIX[i][k]);
        }
        printf("\n");
    }
}

int main()
{
    pthread_t thread_id[2];

    pthread_create(&thread_id[0], NULL, fill_matrix1, (void *)NULL);

    pthread_create(&thread_id[1], NULL, fill_matrix2, (void *)NULL);

    pthread_join(thread_id[0], NULL);

    pthread_join(thread_id[1], NULL);

    print_matrix1();
    printf("\n");
    print_matrix2();

    pthread_t thread_mul[INDEX];

    int i;
    int position[5];
    for (i = 0; i < INDEX; i++)
    {
        position[i] = i;
        pthread_create(&thread_mul[i], NULL, multiply_row, (void *)&position[i]);
    }

    for (i = 0; i < INDEX; i++)
    {
        pthread_join(thread_mul[i], NULL);
    }

    print_final_matrix();
    return 0;
}