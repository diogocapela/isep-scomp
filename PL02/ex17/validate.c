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

    const int NUMBER_OF_USERS=3;
    struct user_data data[NUMBER_OF_USERS];
    strcpy(data[0].username,"user0");
    strcpy(data[0].password, "pass0");
    strcpy(data[1].username,"user1");
    strcpy(data[1].password, "pass1");
    strcpy(data[2].username,"user2");
    strcpy(data[2].password, "pass2");

    char username[20];
    char password[20]="\0";
    struct user_data input_data;
    //scanf("%s", input_data.username);
    read(0, &input_data, 40);
    //scanf("%s", input_data.password);
    printf("%s\n", input_data.username);
    printf("%s\n", input_data.password);

    int i;
    int exit_value =-1;
    for(i=0; i<NUMBER_OF_USERS; i++){
       
        if(strcmp(input_data.username,data[i].username) == 0){
            exit_value=1;
            if(strcmp(input_data.password, data[i].password)==0){
                exit_value=0;
            }
            break;
        }
    }

    exit(exit_value);

    return exit_value;
}