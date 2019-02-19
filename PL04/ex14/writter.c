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

#define NUMBER_OF_SEMAFOROS 3
#define MUTEX 0
#define NO_USE 1
#define BARREIRA 2

typedef struct
{
    char string[80];
    int current_readers;
    int total_readers;
    int total_writters;
} shared_data;

int main()
{

    //um semaforo para numero de passageiros, outros 3, um para cada porta
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"mutex", "read", "barreira"};
    //primeiro valor é numero total de passageiros, outros tres para mutex
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {1, 1, 1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    //inicializar semaforos
    int i;

    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }
    shared_data *shared_data1;
    int fd;

    /* OPEN SHARED MEMORY*/
    if ((fd = shm_open("/ex14shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
    {
        perror("shm_open error");
        exit(1);
    }

    /* TUNCATE SHARED MEMORY SIZE */
    if (ftruncate(fd, sizeof(shared_data1)) == -1)
    {
        perror("ftruncate error");
        exit(1);
    }

    /* Map da shared memory */
    if ((shared_data1 = (shared_data *)mmap(NULL, sizeof(shared_data1), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }

    __semWAIT(semaforos[BARREIRA]);
    shared_data1->total_writters++;
    __semWAIT(semaforos[NO_USE]);

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("WRITTER, number of writters is %d and number of readers is %d\n", shared_data1->total_writters, shared_data1->total_readers);

    sprintf(shared_data1->string, "pid is %d, time is %s", getpid(), asctime (timeinfo));

    __semPOST(semaforos[BARREIRA]);
    __semPOST(semaforos[NO_USE]);


    return 0;
}