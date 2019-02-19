#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
int main() 
{
    pid_t p;
    if(fork() == 0)
    {
        printf("PID = %d\n", getpid());
        exit(0);   
    }
    if((p = fork()) == 0)
    {
        printf("PID = %d\n", getpid());
        exit(0);
    }
    printf("Parent PID = %d\n", getpid());
    printf("Waiting... (for PID=%d)\n", p);
    waitpid(p, NULL, 0);
    printf("Enter Loop...\n");
    while(1); /* Infinite loop */
}