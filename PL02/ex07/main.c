#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
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

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE = 100;

    int vec1[1000];
    int vec2[1000];
    time_t t; //for srand
    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    int i;
    for (i = 0; i < 1000; i++)
    {
        vec1[i] = rand() % (256);
    }
    for (i = 0; i < 1000; i++)
    {
        vec2[i] = rand() % (256);
    }
    
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    int id = spawn_childs(5);

    if (id == 0)
    {
        close(fd[ESCRITA]);
        int final = 0;
        int tmp;
        for (i = 0; i < 5; i++)
        {
            int status;
            wait(&status);
            if (WIFEXITED(status))
            {
                read(fd[LEITURA], &tmp, 4);
                final += tmp;
            }
        }
        printf("Final result is %d\n", final);
    }
    if (id > 0)
    {
        close(fd[LEITURA]);
        int tmp = 0;
        int start = id * 200;
        int finish = start + 200;
        int i;
        for (i = start; i < finish; i++)
        {
            tmp = tmp + vec1[i] + vec2[i];
        }
        write(fd[ESCRITA], &tmp, 4);
        close(fd[ESCRITA]);
    }
    return 0;
}
