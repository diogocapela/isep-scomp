#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
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

#define NUMBER_OF_ELEMENTS 5

typedef struct {
    int number;
    char name[255];
    char address[255];
} Data;

void* printContents(void* arg) {
    Data* argumento = (Data*)arg;
    
    printf("Number: %d\n", argumento->number);
    printf("Name: %s\n", argumento->name);
    printf("Address: %s\n", argumento->address);
    printf("\n");

    fflush(stdout);

    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
    Data dataArray[NUMBER_OF_ELEMENTS];
    pthread_t threads[NUMBER_OF_ELEMENTS];

    int i;
    for (i = 0; i < NUMBER_OF_ELEMENTS; i++) {
        dataArray[i].number = i;
        strcpy(dataArray[i].name, "Diogo");
        strcpy(dataArray[i].address, "Porto");
    }

    for (i = 0; i < NUMBER_OF_ELEMENTS; i++) {
        pthread_create(&threads[i], NULL, printContents, (void*)&dataArray[i]);
    }

    for (i = 0; i < NUMBER_OF_ELEMENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
