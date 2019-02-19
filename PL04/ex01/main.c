#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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

int main(int argc, char* agrv[]) {
    const int TOTAL_CHILDREN = 8;
    const int NUMBERS_PER_CHILD = 200;

    

    char sem_names[][25] = {"/sem_input_01", "sem_input_02"};

    // remover os ficheiros txt q possam existir
    fazer UNLINK

        if (unlink(file_names[i]) == -1) if (errno != ENOENT){
            // no such file or directory
            exit(EXIT_FAILURE)}

    // criar o numbers.txt
    FILE* fp;
    fp = fopen("numbers.txt", "w");
    fclose(fp);

    fprintf - para escrever n

        // criar 2 semafotos, um de input (iniciado a 1) e outro de output (iniciado a 1)

        // criar 8 processos filho

        // cada filho le 200 numberos do number.txt e escreve no output.txt

        // fechar semaforos (todos os filhos tem q fechar)

        // ler ficheiro ::::
        // criar um array de apontadores para cada um das linhas char * array de 2000

        while (index < NUMBER_OF_LINES_PER_CHILD && (getline(&linha, &tamanho, fp) != -1)) {
        tamanho_linha = sizeof(char) * (strlen)
    }

    // usar o getline de forma dinamica (primeiro parametro a NULL)

    // avançar ate às linhas que se referem ao filho em execucao (1, 201, etc)
}
