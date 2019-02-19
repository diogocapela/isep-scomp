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
    const int BUFFER_SIZE = 50;
    const int NUMBER_OF_CHILDREN = 10;

    struct s1
    {
        char message[4];
        int round;
    };

    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    int id = spawn_childs(NUMBER_OF_CHILDREN);

    if (id == 0) //PAI
    {
        close(fd[LEITURA]);
        int i;

        for (i = 0; i < NUMBER_OF_CHILDREN; i++)
        {
            struct s1 s;
            strcpy(s.message, "WIN");
            s.round = i + 1;
            sleep(1);
            write(fd[ESCRITA], &s, sizeof(s));
        }

        close(fd[ESCRITA]);
    }

    if (id > 0)
    {
        close(fd[ESCRITA]);
        struct s1 s;
        read(fd[LEITURA], &s, BUFFER_SIZE);
        close(fd[LEITURA]);
        printf(s.message);
        printf(" round %d\n", s.round);
        exit(s.round);
    }

    int status;
    int i;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        pid_t pid = wait(&status);
        if (WIFEXITED(status))
        {
            int exit_round = WEXITSTATUS(status);
            printf("Child with pid number %d won the round number %d\n", pid, exit_round);
        }
    }
    return 0;
}
