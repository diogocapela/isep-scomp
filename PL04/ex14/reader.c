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


typedef struct {
    char string[80];
    int current_readers;
    int total_readers;
    int total_writters;
} shared_data;


int main()
{

    //um semaforo para numero de passageiros, outros 3, um para cada porta
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"BARREIRA", "NO_USE", "MUTEX"};
    //primeiro valor é numero total de passageiros, outros tres para mutex
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {1,1,1};
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
    
    
    //BARREIRA PARA GARANTIR QUE WRITTER TEM PRIORIDADE
    __semWAIT(semaforos[BARREIRA]);
    __semPOST(semaforos[BARREIRA]);
    
    
    //VAMOS MEXER NA MEMORIA POR ISSO MAIS NINGUEM PODE ESTAR A MEXER
    __semWAIT(semaforos[MUTEX]);

    
    //AUMENTAR NUMERO DE READERS CURRENTES E DE TOTAIS
    shared_data1->current_readers++;
    shared_data1->total_readers++;

    
    //SE FORMOS O UNICO READER VAMOS ATIVAR O SEMAFORO DE NO USE PARA NINGUEM PODER ESCREVER AO MESMO TEMPO
    if( shared_data1->current_readers==1){
        __semWAIT(semaforos[NO_USE]);
    }
    
    
    //LIBERTAR SEMAFORO POIS NAO ESTAMOS A ESCREVER
    __semPOST(semaforos[MUTEX]);
    
    //imprimir string
    printf("%s", shared_data1->string);
    
    
    //VAMOS ESCREVER NOVAMENTE POR ISSO VAMOS BLOQUEAR SEMAFORO DE EXCLUSAO
    __semWAIT(semaforos[MUTEX]);
     shared_data1->current_readers--;

    
    //SE NAO HOUVER LEITORES DAMOS POST NO SEMAFORO DE NO USE PARA QUE POSSO HAVER ESCRITA
    if( shared_data1->current_readers==0){
        __semPOST(semaforos[NO_USE]);
    }
    
    
    //LIBERTAR SEMAFORO DE EXCLUSAO
    __semPOST(semaforos[MUTEX]);

    return 0;
}