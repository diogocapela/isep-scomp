#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>



int main(void){

    struct user_data{
        char username[20];
        char password[20];
    };

    const int LEITURA = 0;
    const int ESCRITA = 1;

    char username[20];
    char password[20];
    printf("insert username\n");
    scanf("%s", username);
    printf("insert password\n");
    scanf("%s", password);
   
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid_t pid = fork();

    if(pid==0){
        close(fd[ESCRITA]);
        dup2(fd[LEITURA], 0);
        close(fd[LEITURA]);
        execlp("./validate","./validate", (char*)NULL);
    }
    close(fd[LEITURA]);
    
    username[strlen(username)+1]= 0;
    username[strlen(password)+1]= 0;
    struct user_data data;
    strcpy(data.username, username);
    strcpy(data.password, password);
    write(fd[ESCRITA], &data, sizeof(data));
    //write(fd[ESCRITA], username, 20);
    //write(fd[ESCRITA], password, 20);
    close(fd[ESCRITA]);

    int status;
    int exit_status;
    wait(&status);
    exit_status = WEXITSTATUS(status);
   if(exit_status==0){
       printf("Password verified\n");
   }
   if(exit_status==1){
       printf("invalid password\n");
   }
   if(exit_status==255){
       printf("No such user\n");
   }

    return 0;
}