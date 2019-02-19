#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main() 
{
    int i;
    int j;
    pid_t pid[10];
    for(i=0; i<10; i++){
        pid[i] = fork();
        if(pid[i]==0){
            int inicio = 100*i+1;
            int final = 100*(i+1);
            for(j=inicio; j<final; j++){
                printf("%d ", j);
            }
            printf("\n");
            exit(0);
        }
    }
    for(i=0; i<10; i++){
        waitpid(pid[i], NULL, 0);
    }
}