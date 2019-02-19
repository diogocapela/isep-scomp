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

#define NUMBER_OF_PROCESSES 3
#define NUMBER_OF_SEMAFOROS 3
#define SEM_01 0
#define SEM_02 1
#define SEM_03 2

int cria_filhos(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid == 0) {
            return i + 1;
        }
    }
    return 0;
}

void printMessage(char *message) {
    printf("%s", message);
    fflush(stdout);  // Sem isto nunca funcionava apesar de estar tudo certo!!!
}

void __semWAIT(sem_t *semaforo) {
    if (sem_wait(semaforo) == -1) {
        perror("Error at sem_wait()!");
        exit(EXIT_FAILURE);
    }
}

void __semPOST(sem_t *semaforo) {
    if (sem_post(semaforo) == -1) {
        perror("Error at sem_post()!");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *agrv[]) {
    const char SEMAFOROS_NAME[NUMBER_OF_SEMAFOROS][80] = {"SEM_01", "SEM_02", "SEM_03"};
    const int SEMAFOROS_INITIAL_VALUE[NUMBER_OF_SEMAFOROS] = {1, 0, 0};
    sem_t *semaforos[NUMBER_OF_SEMAFOROS];

    printf("Exercício 07\n");

    // Open semaforos with corresponent initial value
    int i;
    for (i = 0; i < NUMBER_OF_SEMAFOROS; i++) {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_INITIAL_VALUE[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create all the children
    int id = cria_filhos(NUMBER_OF_PROCESSES);

    // If we are at the child process #1
    if (id == 1) {
        __semWAIT(semaforos[SEM_01]);
        printMessage("Sistemas ");
        __semPOST(semaforos[SEM_02]);

        __semWAIT(semaforos[SEM_01]);
        printMessage("a ");
        __semPOST(semaforos[SEM_02]);

        exit(EXIT_SUCCESS);
    }

    // If we are at the child process #2
    if (id == 2) {
        __semWAIT(semaforos[SEM_02]);
        printMessage("de ");
        __semPOST(semaforos[SEM_03]);

        __semWAIT(semaforos[SEM_02]);
        printMessage("melhor ");
        __semPOST(semaforos[SEM_03]);

        exit(EXIT_SUCCESS);
    }

    // If we are at the child process #3
    if (id == 3) {
        __semWAIT(semaforos[SEM_03]);
        printMessage("Computadores - ");
        __semPOST(semaforos[SEM_01]);

        __semWAIT(semaforos[SEM_03]);
        printMessage("disciplina!\n");

        exit(EXIT_SUCCESS);
    }

    // Wait for all the processes
    for (i = 0; i < NUMBER_OF_PROCESSES; i++) {
        wait(NULL);
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