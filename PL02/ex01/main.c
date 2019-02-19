#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
    const int LEITURA = 0;
    const int ESCRITA = 1;
    
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
        pid_t pid_p = getpid();
        printf("pai: %d\n", pid_p);
        write(fd[ESCRITA], &pid_p, sizeof(pid_p)); 
        close(fd[ESCRITA]);
    }

    if(pid==0){
        close(fd[ESCRITA]);
        pid_t pid_parent=0;
        read(fd[LEITURA], &pid_parent, sizeof(pid_parent));
        close(fd[LEITURA]);
        printf("filho: %d\n", pid_parent);
    }
    return 0;
}
