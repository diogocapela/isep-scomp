#include "includes.h"

int menu() {
    int option;
    printf("1. Insert\n");
    printf("2. Consult One Record\n");
    printf("3. Consult All Records\n");
    printf("0. Exit\n");
    scanf("%d", &option);
    return option;
}

void runProgram(char *program) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(program, program, (char *)NULL);
        exit(EXIT_FAILURE);
    }
    while (wait(NULL) > 0) {
        // wait until all children are dead
    };
}

int main(int argc, char *agrv[]) {

    // Declare semaforo
    sem_t *semaforo;

    // Create and open semaforo
    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Open shared memory
    int fd;
    SharedDataStruct *sharedData = __shmOPEN(SHM_FILE_NAME, &fd, O_RDWR | O_CREAT);

    // Reset all records
    __semWAIT(semaforo);
    int i;
    for(i = 0; i < NUMBER_OF_RECORDS; i++) {
        sharedData->records[i].number = 0;
    }
    __semPOST(semaforo);

    int option;

    do {
        option = menu();

        switch (option) {
            case 1:
                runProgram("./insert.bin");
                break;
            case 2:
                runProgram("./consult.bin");
                break;
            case 3:
                runProgram("./consult_all.bin");
                break;
            case 0:
                return EXIT_SUCCESS;
            default:
                option = menu();
        }
    } while(option != 0);

    // Close and unmap shared memory
    __shmCLOSE(sharedData, fd);

    // Delete shared memory
    __shmDELETE(SHM_FILE_NAME);

    // Close the semaforo
    __semaforo__CLOSE(semaforo);

    // Delete semaforo for file system
    __semaforo__UNLINK(SEMAFORO_FILE_NAME);

    return EXIT_SUCCESS;
}