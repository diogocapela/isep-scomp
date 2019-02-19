#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
int main (){
    int numbers[ARRAY_SIZE];/* array to lookup */
    int n;/* the number to find */
    time_t t;/*  needed  to  init.  therandom  number  generator  (RNG) */
    int i;/* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));/* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand () % 100;
    }
    /* initialize n */  
    n = rand () % 100;
    int count = 0;
    pid_t pid = fork();
    if(pid == 0){
        for(i = 0; i< ARRAY_SIZE/2; i++){
            if(numbers[i] == n){count++;}
        }
        exit(count);
    }
    for(i=ARRAY_SIZE/2; i<ARRAY_SIZE;i++){
        if(numbers[i] == n){count++;}
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
           count += WEXITSTATUS(status);
    }   
    printf("%d\n", count); 
    return 0;
}