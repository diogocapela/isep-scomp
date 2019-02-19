#include "includes.h"

#define CAN_READ 0
#define CAN_WRITE 1
#define MUTEX 2

int main(int argc, char *agrv[]) {
    int i;
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"CAN_READ", "CAN_WRITE", "MUTEX"};
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {0, 10, 1};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    // Open semaforos with corresponent initial value
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Open shared memory
    int fd;
    SharedDataStruct *sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR | O_CREAT);

    // Initialize the buffer
    for (i = 0; i < BUFFER_SIZE; ++i) {
        sharedData->circularBuffer.buffer[i] = 0;
    }
    sharedData->circularBuffer.head = 0;
    sharedData->circularBuffer.tail = 0;
    sharedData->circularBuffer.isFull = 0;
    printBuffer(&sharedData->circularBuffer);


    int id = cria_filhos(NUMBER_OF_CHILDREN);

    if (id > 0) {
        printf("CHILD\n");
        for(i = 0; i < 15; i++) {
            __semWAIT(semaforos[CAN_WRITE]);
            __semWAIT(semaforos[MUTEX]);
            int head = sharedData->circularBuffer.head;

            sharedData->circularBuffer.buffer[head] = getRandomIntBetween(0, 9);

            if(head == BUFFER_SIZE - 1) {
                sharedData->circularBuffer.head = 0;
            } else {
                sharedData->circularBuffer.head = head + 1;
            }

            __semPOST(semaforos[MUTEX]);
            __semPOST(semaforos[CAN_READ]);
        }

        exit(EXIT_SUCCESS);

    } else {
        printf("FATHER\n");

        for(i = 0; i < 30; i++) {
            __semWAIT(semaforos[CAN_READ]);
            __semWAIT(semaforos[MUTEX]);
            int tail = sharedData->circularBuffer.tail;

            int readData = sharedData->circularBuffer.buffer[tail];

            if(tail == BUFFER_SIZE - 1) {
                sharedData->circularBuffer.head = 0;
            } else {
                sharedData->circularBuffer.head = tail + 1;
            }

            printBuffer(&sharedData->circularBuffer);


            __semPOST(semaforos[MUTEX]);
            __semPOST(semaforos[CAN_WRITE]);

        }


    }

    // Wait for all the processes
    for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
        wait(NULL);
    }
    
    // Close and unmap shared memory
    __shmCLOSE(sharedData, fd);

    // Delete shared memory
    __shmDELETE(SHM_FILE_NAME);

    // Close all semaforos
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
        if (sem_close(semaforos[i]) == -1) {
            perror("Error at sem_close()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Remove semaforos from system
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
        if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
            perror("Error at sem_unlink()!\n");
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}