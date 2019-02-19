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

#define NUMERO_WRITTERS 5
#define NUMERO_READERS 10

#define NUMBER_OF_SEMAFOROS 3
#define MUTEX 0
#define NO_USE 1
#define BARREIRA 2


typedef struct {
    char string[80];
    int current_readers;
    int total_readers;
    int total_writters;
} shared_data;

int main(){
    

    int fd;
	shared_data* shared_data1;
	
					/* OPEN SHARED MEMORY*/
	if((fd = shm_open("/ex14shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
		perror("shm_open error");
		exit(1);
	}
	
		/* TUNCATE SHARED MEMORY SIZE */
	if(ftruncate(fd,sizeof(shared_data1)) == -1){
		perror("ftruncate error");
		exit(1);
	}
	
	/* Map da shared memory */
	if(( shared_data1 = (shared_data*) mmap(NULL, sizeof(shared_data1), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}

    shared_data1->current_readers=0;
	shared_data1->total_readers=0;
    shared_data1->total_writters=0;

    //um semaforo para numero de passageiros, outros 3, um para cada porta
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"BARREIRA", "NO_USE", "MUTEX"};
    //primeiro valor é numero total de passageiros, outros tres para mutex
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {1, 1, 1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    //inicializar semaforos
    int i;

    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++)
    {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED)
        {
            perror("Error at sem_open()seller!\n");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid;
    for(i=0; i<NUMERO_WRITTERS; i++){
        pid=fork();
		if(pid==0){
			execlp("./writter","./writter",(char*)NULL);
            printf("continuo aqui\n");
		}
    }

    for(i=0; i<NUMERO_READERS; i++){
        pid=fork();
		if(pid==0){
			execlp("./reader","./reader",(char*)NULL);
            printf("continuo aqui\n");
		}
    }

    for(i=0; i<(NUMERO_READERS+NUMERO_WRITTERS); i++){
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

    // Undo mapping
    if (munmap(shared_data1, sizeof(shared_data)) == -1)
    {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    // Remove file from system
    if (shm_unlink("/ex14shm") < 0)
    {
        perror("Error at unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}