#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FIRST_READ_SIZE 4
#define MESSAGE_SIZE 80

int main() {
    const int LEITURA = 0;
    const int ESCRITA = 1;

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
        close(fd[LEITURA]);
        char firstMessage[MESSAGE_SIZE] = "Hello World";
        char secondMessage[MESSAGE_SIZE] = "“Goodbye!";
        write(fd[ESCRITA], firstMessage, MESSAGE_SIZE);
        write(fd[ESCRITA], secondMessage, MESSAGE_SIZE);
        close(fd[ESCRITA]);
    }

    // If we are at the child process
    if (pid == 0) {
        printf("Filho criado com o PID = %d\n", getpid());
        close(fd[ESCRITA]);
        char firstMessage[MESSAGE_SIZE];
        char secondMessage[MESSAGE_SIZE];
        read(fd[LEITURA], firstMessage, MESSAGE_SIZE);
        read(fd[LEITURA], secondMessage, MESSAGE_SIZE);
        close(fd[LEITURA]);
        printf("firstMessage: %s\n", firstMessage);
        printf("secondMessage: %s\n", secondMessage);
        exit(EXIT_SUCCESS);
    }

    // Wait for the process to finish and grap the status
    int status;
    pid_t pidExit = wait(&status);
    int exitStatus = WEXITSTATUS(status);
    printf("O processo com o PID = %d terminou com valor = %d\n", pidExit, exitStatus);

    exit(EXIT_SUCCESS);
}