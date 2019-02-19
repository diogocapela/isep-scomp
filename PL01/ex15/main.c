#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main()
{
    int exit_status = 0;

    char filename[32];
    while (exit_status == 0)
    {
        printf("Insert filename\n");
        scanf("%s", filename);
        pid_t pid = fork();
        if (pid == 0)
        {
            execlp("ls", "ls", filename, NULL);
        }
        int status;
        wait(&status);
        exit_status = WEXITSTATUS(status);
        printf("%d\n", exit_status);
    }
    printf("Nao encontrou\n");
    return 0;
}
