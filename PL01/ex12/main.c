#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int spawn_childs(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main()
{
    int id = spawn_childs(6);
    if(id>0){
        exit(id*2);
    }
    int status;
    int i;
    for(i=0; i<6; i++){
        wait(&status);
        if(WIFEXITED(status)){
            printf("%d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}