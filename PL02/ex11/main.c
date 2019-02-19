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
    const int NUMBER_OF_CHILDREN = 5;
    const int NUMBER_OF_PIPES = 6;

    int fd[NUMBER_OF_PIPES][2];

    int i;
    for (i = 0; i < NUMBER_OF_PIPES; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe failed");
            return 1;
        }
    }

    pid_t pid;
    int buffer, randomNumber;
    for(i=0; i<NUMBER_OF_CHILDREN; i++){
        pid=fork();
        if(pid==-1){
            perror("fork failed");
            exit(-1);
        }
        if(pid==0){
            srand((unsigned)time(NULL) * getpid());
            close(fd[i][ESCRITA]);
            read(fd[i][LEITURA], &buffer, sizeof(int));
            randomNumber = rand() % (1 + 500) + 1;
            printf("pid: %d, numero: %d\n", getpid(), randomNumber);
            
            close(fd[i][LEITURA]);
			close(fd[i+1][LEITURA]);
            if(randomNumber>buffer){
                write(fd[i+1][ESCRITA],&randomNumber,sizeof(int));  
            }else{
                write(fd[i+1][ESCRITA],&buffer,sizeof(int));
            }
            close(fd[i+1][ESCRITA]);
			exit(EXIT_SUCCESS);
        }
    }
    srand((unsigned)time(NULL) * getpid());
    randomNumber = rand() % (1 + 500) + 1;
    printf("pid: %d, numero: %d\n", getpid(), randomNumber);
    close(fd[0][LEITURA]);
	write(fd[0][ESCRITA],&randomNumber,sizeof(int));
	close(fd[0][ESCRITA]);
	close(fd[5][ESCRITA]);
	read(fd[5][LEITURA],&randomNumber,sizeof(int));
	close(fd[5][LEITURA]);
	printf("Maior numero é: %d\n",randomNumber);
    return 0;
}
