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

    struct S1 {
        int i;
        char str[BUFFER-4];
    };
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
        struct S1 s_pai;
        s_pai.i=i_input;
        strcpy(s_pai.str, str_input);
        write(fd[ESCRITA], &s_pai, sizeof(s_pai)); 
        close(fd[ESCRITA]);
    }

    if(pid==0){
        close(fd[ESCRITA]);
        struct S1 s_filho;
        read(fd[LEITURA], &s_filho, BUFFER);
        close(fd[LEITURA]);
        printf("filho: numero %d\n", s_filho.i);
        printf("filho: string %s\n", s_filho.str);
        exit(EXIT_SUCCESS);
    }
    return 0;
}
