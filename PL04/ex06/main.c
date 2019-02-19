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

int main(int argc, char *agrv[]) {
    const int NUMBER_OF_MESSAGES = 15;
    const int NUMBER_OF_SEMAFOROS = 2;
    const char SEMAFOROS_NAME[2][80] = {"FATHER_SEM", "CHILD_SEM"};
    const int SEMAFOROS_INITIAL_VALUE[2] = {0, 1};
    const int SEM_FATHER = 0;
    const int SEM_CHILD = 1;
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    printf("Exercício 06\n");

    // Open semaforos with corresponent initial value
    int i;
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // For a new process
    pid_t pid = fork();

    // If we are at the father process
    if (pid > 0) {
        int k;
        for (k = 0; k < NUMBER_OF_MESSAGES / 2; k++) {
            if (sem_wait(semaforos[SEM_FATHER]) == -1) {
                perror("Error at sem_wait()!");
                exit(EXIT_FAILURE);
            }
            printf("I'm the father!\n");
            if (sem_post(semaforos[SEM_CHILD]) == -1) {
                perror("Error at sem_post()!");
             
                exit(EXIT_FAILURE);
            }
        }
    }

    // If we are at the child process
    if (pid == 0) {
        int k;
        for (k = 0; k < (NUMBER_OF_MESSAGES / 2) + 1; k++) {
            if (sem_wait(semaforos[SEM_CHILD]) == -1) {
                perror("Error at sem_wait()!");
                exit(EXIT_FAILURE);
            }
            printf("I'm the child!\n");
            if (sem_post(semaforos[SEM_FATHER]) == -1) {
                perror("Error at sem_post()!");
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);
    }

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