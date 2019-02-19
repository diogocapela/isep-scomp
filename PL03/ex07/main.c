#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_CHILDREN 10

#define NUMER_OF_ITEMS_IN_ARRAY 1000

void fillArray(int *vec) {
    srand((unsigned)time(NULL));

    int i;
    for (i = 0; i < NUMER_OF_ITEMS_IN_ARRAY; i++) {
        vec[i] = rand() % NUMER_OF_ITEMS_IN_ARRAY;
    }
}

int main() {
    const int READ = 0;
    const int WRITE = 1;

    int array[NUMER_OF_ITEMS_IN_ARRAY];
    int arrayMaximum[10];

    fillArray(array);

    // Create a pipe shared by all process
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(EXIT_FAILURE);
    }

    // Fork all the children
    pid_t pid;
    int i;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++) {
        pid = fork();

        if (pid == 0) {
            break;
        }

        // Check for errors
        if (pid < 0) {
            perror("Fork failed!");
            exit(EXIT_FAILURE);
        }
    }

    // If we are at the father process
    //===============================================
    if (pid > 0) {
        // Close the write side of the pipe
        close(fd[WRITE]);

        // Wait and read the result from all the child process
        int status;
        int k;
        for (k = 0; k < NUMBER_OF_CHILDREN; k++) {
            wait(&status);
            if (!WIFEXITED(status)) {
                perror("Error at a child process\n");
                exit(EXIT_FAILURE);
            }
            read(fd[READ], &arrayMaximum[k], sizeof(int));
        }

        // Close the read side of the pipe
        close(fd[READ]);


    }

    // If we are at the child processes
    //===============================================
    if (pid == 0) {
        // Close the read side of the pipe
        close(fd[READ]);


        // TODO: Acabar


        // Close the write side of the pipe
        close(fd[WRITE]);
    }
    return 0;
}
