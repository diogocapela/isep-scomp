#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int my_exec(char *command){
    pid_t pid = fork();
    char* relative_path = "/bin/";
    char *path = malloc(strlen(relative_path) + strlen(command) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(path, relative_path);
    strcat(path, command);
    if(pid==0){
        execl(path, command, NULL);
    }
    int status;
    wait(&status);
    return (WEXITSTATUS(status));
}

int main()
{   
    int saida = my_exec("ls");
    if(saida  ==0 ){
        printf("\n"); 
        saida = my_exec("ps");
    }
    if(saida  ==0){  
        printf("\n"); 
        saida = my_exec("who");
    }
    if(saida==0){
        printf("ok\n");   
    }    
    return 0;
}

