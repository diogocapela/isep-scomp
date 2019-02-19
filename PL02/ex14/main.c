#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main() {
    const int READ = 0;
    const int WRITE = 1;

    const int BUFFER_SIZE = 80;

    // File descriptor (0 stdin, 1 stdout, 2 stderr)
    int fd[2];

    // Criar o pipe (0 em sucesso, -1 em erro) (estrutura FIFO)
    if (pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(EXIT_FAILURE);
    }

    // Fork a new process
    pid_t pid = fork();

    // Check for errors
    if (pid < 0) {
        perror("Fork failed!");
        exit(EXIT_FAILURE);
    }

    // If we are at the father process
    if (pid > 0) {
        printf("Processo pai tem o PID = %d\n", getpid());

        int i;
        char buffer[BUFFER_SIZE];

        close(fd[READ]);
        for (i = 0; i < 100; i++) {
            // Add the string to the buffer
            sprintf(buffer, "Linha %d\n", i);
            // Write the string to the pipe
            write(fd[WRITE], buffer, strlen(buffer) * sizeof(char));
        }
        close(fd[WRITE]);

        wait(NULL);
    }

    // If we are at the child process
    if (pid == 0) {
        printf("Filho criado com o PID = %d\n", getpid());

        close(fd[WRITE]);
        dup2(fd[READ], 0);
        close(fd[READ]);

        execlp("more", "more", "+2", (char*)NULL);

        perror("Error during exec");
        exit(EXIT_FAILURE);
    }

    // Wait for the process to finish and grap the status
    /*
    int status;
    pid_t pidExit = wait(&status);
    int exitStatus = WEXITSTATUS(status);
    printf("O processo com o PID = %d terminou com valor = %d\n", pidExit, exitStatus);
    */

    exit(EXIT_SUCCESS);
}
