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

#define SIZE_OF_ARRAY 1000
#define NUMBER_OF_THREADS 3

#define MIN_RANDOM_BALANCE 0
#define MAX_RANDOM_BALANCE 15000

typedef struct {
    int* array;
    int value;
    char type[255];
} Data;

int getRandomIntBetween(int min, int max) {
    return (rand() % max) + min;
}

void* getLowestBalance(void* arg) {
    Data* argumento = (Data*)arg;
    int* array = argumento->array;

    int lowest = MAX_RANDOM_BALANCE;
    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++) {
        int balance = array[i];
        if (balance < lowest) {
            lowest = balance;
        }
    }

    argumento->value = lowest;
    pthread_exit((void*)NULL);
}

void* getHigestBalance(void* arg) {
    Data* argumento = (Data*)arg;
    int* array = argumento->array;

    int highest = MIN_RANDOM_BALANCE;
    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++) {
        int balance = array[i];
        if (balance > highest) {
            highest = balance;
        }
    }

    argumento->value = highest;
    pthread_exit((void*)NULL);
}

void* getAverageBalance(void* arg) {
    Data* argumento = (Data*)arg;
    int* array = argumento->array;
    int sum = 0;

    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++) {
        sum = sum + array[i];
    }

    int average = sum / SIZE_OF_ARRAY;
    argumento->value = average;
    pthread_exit((void*)NULL);
}

int main(int argc, char* agrv[]) {
    // Randomize Seed
    srand(time(NULL));
    // Threads
    pthread_t threads[NUMBER_OF_THREADS];
    // Threads Data
    Data threadsData[NUMBER_OF_THREADS];
    // Array to be searched
    int balancesArray[SIZE_OF_ARRAY];

    // Fill the array with unique balances
    int i;
    for (i = 0; i < SIZE_OF_ARRAY; i++) {
        balancesArray[i] = getRandomIntBetween(MIN_RANDOM_BALANCE, MAX_RANDOM_BALANCE);
    }

    // Fill the thread data with the right info
    int j;
    for (j = 0; j < NUMBER_OF_THREADS; j++) {
        threadsData[j].array = balancesArray;
    }
    strcpy(threadsData[0].type, "lowest");
    strcpy(threadsData[1].type, "highest");
    strcpy(threadsData[2].type, "average");

    // Get Lowest
    pthread_create(&threads[0], NULL, getLowestBalance, (void*)&threadsData[0]);
    // Get Highest
    pthread_create(&threads[1], NULL, getHigestBalance, (void*)&threadsData[1]);
    // Get Average
    pthread_create(&threads[2], NULL, getAverageBalance, (void*)&threadsData[2]);

    // Get the return values from the array
    int k;
    for (k = 0; k < NUMBER_OF_THREADS; k++) {
        pthread_join(threads[k], NULL);
        printf("The thread to find the %s finished with the value of: %d\n", threadsData[k].type, threadsData[k].value);
    }

    return EXIT_SUCCESS;
}
