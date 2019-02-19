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
    // Declare constants
    const char* SEMAFORO_FILE_NAME = "ex05";
    const int SEMAFORO_INITIAL_VALUE = 0;

    printf("Exercício 05\n");

    // Open new semaforo
    sem_t *semaforo;
    if ((semaforo = sem_open(SEMAFORO_FILE_NAME, O_CREAT | O_EXCL, 0644, SEMAFORO_INITIAL_VALUE)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }

    // For a new process
    pid_t pid = fork();

    // If we are at the father process
    if (pid > 0) {
        if (sem_wait(semaforo) == -1) {
            perror("Error at sem_wait()!");
            exit(EXIT_FAILURE);
        }
        printf("I'm the father!\n");
    }

    // If we are at the child process
    if (pid == 0) {
        printf("I'm the child!\n");
        if (sem_post(semaforo) == -1) {
            perror("Error at sem_post()!");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    // Close semaforo
    if (sem_close(semaforo) == -1) {
        perror("Error at sem_close()!\n");
        exit(EXIT_FAILURE);
    }

    // Remove semaforo from system
    if (sem_unlink(SEMAFORO_FILE_NAME) == -1) {
        perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}