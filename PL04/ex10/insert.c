#include "includes.h"

int main(int argc, char* agrv[]) {
    printf("Insert\n");
    printf("------------------------------------\n");

    // Declare semaforo
    sem_t *semaforo;

    // Create and open semaforo
    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, 0)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Open shared memory
    int fd;
    SharedDataStruct* sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR /* | O_EXCL | O_CREAT*/);

    // TODO: Add semaforos

    int number;
    char name[255];
    char address[255];

    printf("Insert number:\n");
    scanf("%d", &number);

    printf("Insert name:\n");
    scanf("%s", name);

    printf("Insert address:\n");
    scanf("%s", address);

    __semWAIT(semaforo);
    int k;
    for(k = 0; k < NUMBER_OF_RECORDS; k++) {
        if(sharedData->records[k].number == number) {
            printf("A record with the number %d already exists! =(\n", number);
            break;
        }
    }

    int i;
    for (i = 0; i < NUMBER_OF_RECORDS; i++) {
        if (sharedData->records[i].number == 0) {

            sharedData->records[i].number = number;
            strcpy(sharedData->records[i].name, name);
            strcpy(sharedData->records[i].address, address);

            break;

        }
    }
    printf("Record added with success!\n");
    __semPOST(semaforo);

    // Close and unmap shared memory
    __shmCLOSE(sharedData, fd);

    // Close the semaforo
    __semaforo__CLOSE(semaforo);

    return EXIT_SUCCESS;
}