#include <sys/time.h>
#include <time.h>

#include "includes.h"

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);                                          // get current time
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;  // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

int main() {
    const int READ = 0;
    const int WRITE = 1;

    int array[MY_ARRAY_SIZE];

    const int BUFFER_SIZE = sizeof(array);

    struct timeval writeTime;
    struct timeval readTime;


    // Create a file descriptor and a pipe
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Pipe faied!");
        exit(EXIT_FAILURE);
    }

    // For a new process
    pid_t pid = fork();

    // If we are at the father process
    if (pid > 0) {
        close(fd[READ]);
        printf("Filling an array with %d elements...\n", MY_ARRAY_SIZE);
        int i;
        for (i = 0; i < MY_ARRAY_SIZE; i++) {
            array[i] = i;
        }

        gettimeofday(&writeTime, NULL);
        printf("TIME AT WRITE: %ld\n", writeTime.tv_usec);



        write(fd[WRITE], array, BUFFER_SIZE);
    }

    // If we are at the child process
    if (pid == 0) {
        close(fd[WRITE]);
        int readArray[MY_ARRAY_SIZE];
        read(fd[READ], &readArray, BUFFER_SIZE);


        gettimeofday(&readTime, NULL);
        printf("TIME AT READ : %ld\n", readTime.tv_usec);

        printf("readArray[6]: %d\n", readArray[6]);


        printf("Diferenca: %lu\n", readTime.tv_usec - writeTime.tv_usec);
        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
