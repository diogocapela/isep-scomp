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

#define NUMERO_CLIENTES 10

typedef struct {
    int clientes;
    int bilhetes;
} shared_data;

int main(){
    

    int fd;
	shared_data* shared_data1;
	
					/* OPEN SHARED MEMORY*/
	if((fd = shm_open("/ex12shm", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR)) == -1){
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

    shared_data1->bilhetes=0;
	shared_data1->clientes=0;

    int i;
    pid_t pid;
    for(i=0; i<NUMERO_CLIENTES; i++){
        pid=fork();
		if(pid==0){
			execlp("./cliente","./cliente",(char*)NULL);
            printf("continuo aqui\n");
		}
    }
    sleep(1);
    
    execlp("./seller","./seller",(char*)NULL);


    return 0;
}