#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

    printf("I'm...\n");
    pid_t pid = fork();

    if(pid>0){
        printf("the..\n");
        pid = fork();
    }

    if(pid>0){
        printf("father!\n");
         pid = fork();
    }

    if(pid==0){
        printf("i will never join you\n");
    }
    return 0;
}