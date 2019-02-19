#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void* printPrimeNumbers(void* arg) {
    int number = *((int*)arg);

    printf("Prime numbers below %d:\n", number);
    fflush(stdout);

    int i = 3;
    int count;
    int c;

    if (number >= 1) {
        printf("2\n");
    }

    for (count = 2; count <= number;) {
        for (c = 2; c <= i - 1; c++) {
            if (i % c == 0)
                break;
        }
        if (c == i) {
            if (i < number) {
                printf("%d\n", i);
            }
            count++;
        }
        i++;
    }

    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
    pthread_t thread;
    unsigned int number;

    printf("Enter max number to print prime numbers:\n");
    scanf("%u", &number);
    fflush(stdout);

    pthread_create(&thread, NULL, printPrimeNumbers, (void*)&number);
    pthread_join(thread, NULL);

    return EXIT_SUCCESS;
}
