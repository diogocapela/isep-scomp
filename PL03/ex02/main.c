#include <fcntl.h> /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define BILLION 1000000000L

// argc = numer of arguments
// argv = actual arguments

int main(int argc, char** argv) {
    struct timespec start;
    struct timespec end;

    // Run the start clock
    if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("Clock failed!");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    // If we are at the child process
    if (pid == 0) {
        execlp(argv[1], argv[1], (char*)NULL);
        perror("Clock failed!");
        exit(EXIT_FAILURE);
    }

    // Wait for the child process to finish
    wait(NULL);

    // Run the end clock
    if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
        perror("Clock failed!");
        exit(EXIT_FAILURE);
    }

    // Calculate the duration (1s = 1 000 000 000 nano seconds)
    double duration = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / BILLION;

    printf("Duration of %s = %f seconds\n\n", argv[1], duration);

    return EXIT_SUCCESS;
}