#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER =  100;
    
    char str_input[BUFFER];
    int i_input;
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
        printf("PAI:Insira o numero\n");
        scanf("%d", &i_input);
        printf("PAI:Insira a string\n");
        scanf("%s", str_input);
        write(fd[ESCRITA], &i_input, sizeof(i_input)); 
        write(fd[ESCRITA], &str_input, sizeof(str_input)+1); 
        close(fd[ESCRITA]);
    }

    if(pid==0){
        close(fd[ESCRITA]);
        read(fd[LEITURA], &i_input, sizeof(i_input));
        read(fd[LEITURA], &str_input, BUFFER);
        close(fd[LEITURA]);
        printf("filho: numero %d\n", i_input);
        printf("filho: string %s\n", str_input);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
