#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void main()
{
    int i;
    int count=0;
    pid_t pid = getpid();

    for (i = 0; i < 4; i++)
    {
        if(pid == getpid()){
            fork();
            count+=1;
        } 
    }
    printf("This is the end.\n");
    printf("%d\n", count);
    exit(count);
}