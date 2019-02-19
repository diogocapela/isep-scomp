#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int ARRAY_SIZE = 2000;
int NUMBER_OF_CHILDREN = 10;

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
        numbers[i] = rand() % 100;
    }
    /* initialize n */
    n = rand() % 100;
    int id = cria_filhos(NUMBER_OF_CHILDREN);

    if (id > 0)
    {
        int start = id * 100;
        int finish = (id + 2) * 100;
        for (i = start; i < finish; i++)
        {
            if (numbers[i] == n)
                exit(i - start);
        }
        exit(255);
    }
    for (i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        int status;
        pid_t pid =wait(&status);
        if (WIFEXITED(status))
        {
            printf("O filho com pid %d terminou com valor = %d\n", pid, WEXITSTATUS(status));
        }
    }
    return 0;
}
