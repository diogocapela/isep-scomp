#include "includes.h"

#define MUTEX 0
#define TOTAL_SPECTATORS 1
#define NO_VIPS_ARE_WAITING 2
#define NO_SPECIALS_ARE_WAITING 3

int main(int argc, char *agrv[]) {
    int i;
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"MUTEX", "TOTAL_SPECTATORS", "NO_VIPS_ARE_WAITING", "NO_SPECIALS_ARE_WAITING"};
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {1, 5, 0, 0};
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
    sharedData->cinema.normals = 0;
    sharedData->cinema.specials = 0;
    sharedData->cinema.vips = 0;
    sharedData->cinema.specialsWaiting = 0;
    sharedData->cinema.vipsWaiting = 0;

    // NORMAL
    for (i = 0; i < 40; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            printf("Normal waiting to enter...\n");
            __semWAIT(semaforos[MUTEX]);
            int specialsWaiting = sharedData->cinema.specialsWaiting;
            int vipsWaiting = sharedData->cinema.vipsWaiting;
            __semPOST(semaforos[MUTEX]);

            if (specialsWaiting > 0) {
                __semWAIT(semaforos[NO_SPECIALS_ARE_WAITING]);
            }
            if (vipsWaiting > 0) {
                __semWAIT(semaforos[NO_VIPS_ARE_WAITING]);
            }

            __semWAIT(semaforos[TOTAL_SPECTATORS]);
            printf("Normal entered!!!!!!!!!!!!!!!!!\n");

            sleep(1);
            __semPOST(semaforos[TOTAL_SPECTATORS]);
            printf("Normal left!\n");

            __shmCLOSE(sharedData, fd);
            for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(EXIT_FAILURE);
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    // SPECIAL
    for (i = 0; i < 20; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            printf("Special waiting to enter...\n");
            __semWAIT(semaforos[MUTEX]);
            sharedData->cinema.specialsWaiting++;
            int vipsWaiting = sharedData->cinema.vipsWaiting;
            __semPOST(semaforos[MUTEX]);

            if (vipsWaiting > 0) {
                __semWAIT(semaforos[NO_VIPS_ARE_WAITING]);
            }

            __semWAIT(semaforos[TOTAL_SPECTATORS]);

            __semWAIT(semaforos[MUTEX]);
            sharedData->cinema.specialsWaiting--;
            int specialsWaiting = sharedData->cinema.specialsWaiting;
            __semPOST(semaforos[MUTEX]);

            if (specialsWaiting == 0) {
                __semPOST(semaforos[NO_SPECIALS_ARE_WAITING]);
            }

            printf("Special entered!!!!!!!!!!!!!!!!!\n");

            sleep(1);

            __semPOST(semaforos[TOTAL_SPECTATORS]);
            printf("Special left!\n");

            __shmCLOSE(sharedData, fd);
            for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(EXIT_FAILURE);
                }
            }
            exit(EXIT_SUCCESS);
        }
    }

    // VIP
    for (i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            printf("VIP waiting to enter...\n");

            __semWAIT(semaforos[MUTEX]);
            sharedData->cinema.vipsWaiting++;
            __semPOST(semaforos[MUTEX]);

            __semWAIT(semaforos[TOTAL_SPECTATORS]);

            __semWAIT(semaforos[MUTEX]);
            sharedData->cinema.vipsWaiting--;
            int vipsWaiting = sharedData->cinema.vipsWaiting;
            __semPOST(semaforos[MUTEX]);

            if (vipsWaiting == 0) {
                __semPOST(semaforos[NO_VIPS_ARE_WAITING]);
            }

            printf("VIP entered!!!!!!!!!!!!!!!!!\n");

            sleep(1);

            __semPOST(semaforos[TOTAL_SPECTATORS]);
            printf("VIP left!\n");

            __shmCLOSE(sharedData, fd);
            for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(EXIT_FAILURE);
                }
            }
            exit(EXIT_SUCCESS);
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