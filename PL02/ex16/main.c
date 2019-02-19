#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 80

int main() {
    const int READ = 0;
    const int WRITE = 1;

	// File descriptor matrix
    int fd[2][2];

    // Create 2 pipes
    if (pipe(fd[0]) == -1 || pipe(fd[1]) == -1) {
        perror("Pipe failed!");
        exit(EXIT_FAILURE);
    }

    // For a new process
    pid_t pid1 = fork();

    // Check for erros
    if (pid1 == -1) {
        perror("Fork failed!");
        exit(EXIT_FAILURE);
    }

    // If we are at the child process 1
    if (pid1 == 0) {
        close(fd[0][READ]);
        dup2(fd[0][WRITE], 1);
        close(fd[0][WRITE]);

        execlp("ls", "ls", "-la", (char*)NULL);

        perror("Exec error!");
        exit(EXIT_FAILURE);
    }

	// For a new process
    pid_t pid2 = fork();

	// Check for erros
    if (pid2 == -1) {
        perror("Fork failed!");
        exit(EXIT_FAILURE);
    }

	// If we are at the child process 2
    if (pid2 == 0) {
        // Input from fd[0]
        close(fd[0][WRITE]);
        dup2(fd[0][READ], 0);
        close(fd[0][READ]);

        // Output to fd[1]
        close(fd[1][READ]);
        dup2(fd[1][WRITE], 1);
        close(fd[1][WRITE]);

        execlp("sort", "sort", (char*)NULL);

        perror("Exec error!");
        exit(EXIT_FAILURE);
    }

    // Close all unused file descriptors
    close(fd[0][WRITE]);
    close(fd[0][READ]);

    // Input from fd[1][READ]
    close(fd[1][WRITE]);
    dup2(fd[1][READ], 0);
    close(fd[1][READ]);

    execlp("wc", "wc", "-l", (char*)NULL);
    perror("Exec error!");
    exit(EXIT_FAILURE);
}
