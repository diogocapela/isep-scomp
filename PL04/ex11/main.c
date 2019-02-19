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

#define NUMBER_OF_SEMAFOROS 4
#define MAX_CAPACITY 200
#define TIME_MEASURE 50

int main()
{
    //um semafora para numero de passageiros, outros 3, um para cada porta
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"SEM_01", "SEM_02", "SEM_03", "SEM_04"};
    //primeiro valor é numero total de passageiros, outros tres para mutex
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {20, 1, 1, 1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    //inicializar semaforos
    int i;
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //tempo durante o qual a simulacao vai correr
    int milli_seconds = 1000 * TIME_MEASURE; 
  
    // tempo inicial
    clock_t start_time = clock(); 

    int id = cria_filhos(3);

    if (id == 1)
    {   
        //semente para numero aleatorio 1 ou 2
        srand(time(NULL));
        while (clock() < start_time + milli_seconds)
        {
            int random = (rand() % 2) +1;
            //se for 1 vai entrar um passageiro
            if (random == 1)
            {
                //esperar que porta esteja disponivel
                __semWAIT(semaforos[1]);
                //aumentar numero de passageiros ou seja decrementar semaforo, timedwait para que nao haja deadlock
                __semTIMEDWAIT(semaforos[0], 1);
                //desbloquear porta
                __semPOST(semaforos[1]);
                printf("Entrou um passageiro na porta 1\n");
                usleep(100);
            }
            else
            {
                //bloquear porta
                __semWAIT(semaforos[1]);
                int val;
                //verificar se comboio nao esta vazio, nesse caso nao pode sair um passageiro pois este nao existe
                sem_getvalue(semaforos[0], &val);
                if(val==20){
                    printf("Nao saiu um passageiro na porta 1\n");
                    //desbloquear porta
                    __semPOST(semaforos[1]);
                    usleep(100);
                }else{
                    //decrementar numero de passageiros (incrementar semafora)
                __semPOST(semaforos[0]);
                //desbloquear porta
                __semPOST(semaforos[1]);
                printf("Saiu um passageiro na porta 1\n");
                usleep(100);
                }
            }
        }
        exit(EXIT_SUCCESS);
    }
    if (id == 2)
    {
        srand(time(NULL));
        while (clock() < start_time + milli_seconds)
        {
           
            int random = (rand() % 2) +1;
            if (random == 1)
            {
                __semWAIT(semaforos[2]);
                __semTIMEDWAIT(semaforos[0], 1);
                __semPOST(semaforos[2]);
                printf("Entrou um passageiro na porta 2\n");
                usleep(100);
            }
            else
            {
                __semWAIT(semaforos[2]);
                int val;
                sem_getvalue(semaforos[0], &val);
                if(val==20){
                    printf("Nao saiu um passageiro na porta 2\n");
                    __semPOST(semaforos[2]);
                    usleep(100);
                }else{
                __semPOST(semaforos[0]);
                __semPOST(semaforos[2]);
                printf("Saiu um passageiro na porta 2\n");
                usleep(100);
                }
            }
        }
        exit(EXIT_SUCCESS);
    }
    if (id == 3)
    {
        srand(time(NULL));
        while (clock() < start_time + milli_seconds)
        {
           
            int random = (rand() % 2) +1;
            if (random == 1)
            {
                __semWAIT(semaforos[3]);
                __semTIMEDWAIT(semaforos[0], 1);
                __semPOST(semaforos[3]);
                printf("Entrou um passageiro na porta 3\n");
                usleep(100);
            }
            else
            {
                __semWAIT(semaforos[3]);
                int val;
                sem_getvalue(semaforos[0], &val);
                if(val==20){
                    printf("Nao saiu um passageiro na porta 3\n");
                    __semPOST(semaforos[3]);
                    usleep(100);

                }else{
                __semPOST(semaforos[0]);
                __semPOST(semaforos[3]);
                printf("Saiu um passageiro na porta 3\n");
                usleep(100);
                }
            }
        }
        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    // Close all semaforos
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if (sem_close(semaforos[i]) == -1)
        {
            perror("Error at sem_close()!\n");
            exit(EXIT_FAILURE);
        }
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