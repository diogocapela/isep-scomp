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

    // Get the number fro stdin
    int numero;
    scanf("%d", numero);
    close(fd[READ]);

    // Compute the factorial

    // Redirect to stdout
    dup2(fd[WRITE], 1);

    // Write t stdout
    write(fd[WRITE], numero, BUFFER_SIZE);
    close(fd[WRITE]);

    exit(EXIT_SUCCESS);
}
