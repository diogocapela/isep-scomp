#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int ARRAY_SIZE = 1000;
int NUMBER_OF_CHILDREN = 5;

int cria_filhos(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}
int find_maximum(int *numbers, int start, int finish)
{
    int max = 0;
    int i;
    for (i = start; i < finish; i++)
    {
        if (numbers[i] > max)
        {
            max = numbers[i];
        }
    }
    return max;
}

int main()
{
    int numbers[ARRAY_SIZE];
    int n; //number to find

    time_t t; //for srand

    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    int i;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        numbers[i] = rand() % (256);
    }
    int id = cria_filhos(NUMBER_OF_CHILDREN);
    if (id > 0)
    {
        int start = (id - 1) * 200;
        int finish = start + 200;
        exit(find_maximum(numbers, start, finish));
    }
    int max=0;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            if (max < WEXITSTATUS(status))
            {
                max = WEXITSTATUS(status);
            }
        }
    }
    int result;
    pid_t pid = fork();
    if (pid == 0)
    {
        for (i = 0; i < 500; i++)
        {
            result = (int)((numbers[i]*100) / max);
            printf("%d, %d\n", i, result);
        }
    }
    else
    {
        int status;
        wait(&status);
        if (WIFEXITED(status))
        {
            for (i = 500; i < 1000; i++)
            {
                result = (int)((numbers[i]*100) / max);
                printf("%d, %d\n", i, result);
            }
        }
    }

    return 0;
}
