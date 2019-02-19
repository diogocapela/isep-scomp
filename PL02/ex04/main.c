#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE =  100;
    const char* filename = "teste.txt";
    ssize_t read_s;
    size_t tam = 0;
    
    char *linha = NULL;
    FILE *fp;

    int fd[2];

    if(pipe(fd)==-1){ 
        perror("Pipe failed");
        return 1; 
    }
    
    pid_t pid = fork();
    if(pid==-1){
        perror("fork failed");
        return 1;
    }

    if(pid>0){
        close(fd[LEITURA]);
        fp = fopen(filename, "r");        
        if(fp == NULL)
            exit(EXIT_FAILURE);
        while ((read_s = getline(&linha, &tam, fp)) != -1){
            write(fd[ESCRITA], linha, read_s); 
        }
        fclose(fp);
        close(fd[ESCRITA]);
        if(linha!=NULL)
            free(linha);
        wait(NULL);
    
    }else{
        close(fd[ESCRITA]);
        char buffer[BUFFER_SIZE];
        int n;
        printf("filho recebeu:\n");
        while((n=read(fd[LEITURA], buffer, BUFFER_SIZE-1))){
            buffer[n]= '\0';
            printf("%s", buffer);
        }
        close(fd[LEITURA]);
    }
    return 0;
}
