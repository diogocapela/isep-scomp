#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    const int LEITURA = 0;
    const int ESCRITA = 1;
    const int BUFFER_SIZE = 100;
    char message[BUFFER_SIZE];

    int up[2];
    int down[2];

    if (pipe(up) == -1)
    {
        perror("Pipe failed");
        return 1;
    }
    if (pipe(down) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }

    if (pid > 0)
    {
        close(down[LEITURA]);
        close(up[ESCRITA]);
        int len = read(up[LEITURA], message, BUFFER_SIZE);
        close(up[LEITURA]);
        message[len] = '\0';
        int i;
        for(i = 0; i<len; i++){
            if(message[i]>64 && message[i]<91)
                message[i] = message[i] + 32;
            else if(message[i]>96 && message[i]<123)
                message[i] = message[i] - 32;
        }
        write(down[ESCRITA], message, len);
        close(down[ESCRITA]);
    }
    else
    {
        close(up[LEITURA]);
        close(down[ESCRITA]);       
        printf("Insert message\n");
        scanf("%s", message);
        int len = strlen(message);
        write(up[ESCRITA], message, len);
        close(up[ESCRITA]);
        read(down[LEITURA], message, len);
        close(down[LEITURA]);
        printf("filho recebeu: %s\n", message);
    }
    return 0;
}
