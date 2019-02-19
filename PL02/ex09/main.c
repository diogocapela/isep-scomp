#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int customer_code;
    int product_code;
    int quantity;
} Sale;

int cria_filhos(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main() {
    const int LEITURA = 0;
    const int ESCRITA = 1;

    const int NUMBER_OF_PRODUCTS = 50000;
    const int NUMBER_OF_CHILDREN = 10;

    Sale sales[NUMBER_OF_PRODUCTS];

    const int BUFFER_SIZE = sizeof(Sale);

    // Preencher a array
    int i;
    for (i = 0; i < NUMBER_OF_PRODUCTS; i++) {
        Sale sale;
        sale.customer_code = i;
        sale.product_code = i;
        sale.quantity = 2;
        sales[i] = sale;
    }

    sales[40].quantity = 33;
    sales[478].quantity = 33;

    // File descriptor (0 stdin, 1 stdout, 2 stderr)
    int fd[2];

    // Create the pipe
    if (pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(EXIT_FAILURE);
    }

    int id = cria_filhos(NUMBER_OF_CHILDREN);

    // If we are at the father process
    if (id == 0) {
        close(fd[ESCRITA]);
        Sale sale;

        int n;
        while((n = read(fd[LEITURA], &sale, BUFFER_SIZE)) > 0) {
            printf("Produto com mais de 20 vendas: %d\n", sale.product_code);
        }

        close(fd[LEITURA]);
    }

    // If we are at the child processes
    if (id > 0) {
        close(fd[LEITURA]);
        int i;
        int inicio = (id - 1) * 5000;
        int fim = id * 5000;
        for (i = inicio; i < fim; i++) {
            int soldQuantity = sales[i].quantity;
            if (soldQuantity > 20) {
                write(fd[ESCRITA], &sales[i], BUFFER_SIZE);
            }
        }
        close(fd[ESCRITA]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
