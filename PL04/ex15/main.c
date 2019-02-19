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
#include "includes.h"

#define NUMBER_OF_SEMAFOROS 2   
#define MAX_CAPACITY 200
#define TIME_MEASURE 50
#define NUMBER_OF_CHILDREN 15
#define TIME_OF_SHOW 1

int main()
{
    //um semafora para numero de passageiros, outros 3, um para cada porta
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"SEM_01", "SEM_02"};
    //primeiro valor é numero total de passageiros, outros tres para mutex
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {5,1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    //inicializar semaforos
    int i;

    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error at sem_open()\n");
            exit(EXIT_FAILURE);
        }
    }

    int id = cria_filhos(NUMBER_OF_CHILDREN);

    if (id != 0)
    {

        while (1)
        {
            if (sem_trywait(semaforos[0]) == 0)
            {
                int show;
                sem_getvalue(semaforos[1], &show);
                printf("sou o id %d, Entrei no show %d\n", id, show);
                sleep(TIME_OF_SHOW);
                
                __semPOST(semaforos[0]);
                int count=0;
                sem_getvalue(semaforos[0], &count);
                 if (count == 4)
                {
                    __semPOST(semaforos[1]);
                }
                printf("sou o id %d, sai do show %d\n", id, show);
                break;
            }else{
                 sleep(TIME_OF_SHOW);
            }
           
        }
        exit(EXIT_SUCCESS);
    }

    for(i=0; i<NUMBER_OF_CHILDREN; i++){
        wait(NULL);
    }
    // Remove semaforos from system
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if (sem_unlink(SEMAFOROS_NAME[i]) == -1)
        {
            perror("Error at sem_unlink()!\n");
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}