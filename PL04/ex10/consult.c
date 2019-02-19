#include "includes.h"

int main(int argc, char *agrv[]) {
    printf("Consult One Record\n");
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
    SharedDataStruct *sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR /* | O_EXCL | O_CREAT*/);

    int number;

    printf("Insert the number of the record you want to consult:\n");
    scanf("%d", &number);

    __semWAIT(semaforo);
    int found = 0;
    int i;
    for (i = 0; i < NUMBER_OF_RECORDS; i++) {
        if (sharedData->records[i].number == number) {
            printf("Record was found!");
            printf("------------------------------------\n");
            printf("Number: %d\n", sharedData->records[i].number);
            printf("Name: %s\n", sharedData->records[i].name);
            printf("Address: %s\n", sharedData->records[i].address);
            printf("------------------------------------\n");
            found = 1;
            break;
        }
    }

    if(found == 0) {
        printf("Sorry, no record with that number was found... =(\n");
    }
    __semPOST(semaforo);

    // Close and unmap shared memory
    __shmCLOSE(sharedData, fd);

    // Close the semaforo
    __semaforo__CLOSE(semaforo);

    return EXIT_SUCCESS;
}