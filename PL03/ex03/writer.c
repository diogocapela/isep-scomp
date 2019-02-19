#include "includes.h"

int getRandomIntBetween(int min, int max) {
    int randomNumber = (rand() % max) + min;  // random number between 1 and 5
    return randomNumber;
}

int main() {

    const int ARRAY_SIZE = 10;
    const int SIZE = ARRAY_SIZE * sizeof(int);

    /* needed to init. the random number generator (RNG) */
    time_t t;
    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned) time(&t));

    // Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_WRITER_OFLAG, MY_SHM_OPEN_WRITER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Defines the size
    ftruncate(fd, SIZE);

    // Get a pointer to the data
    // sharedData[10]
    int *sharedData = (int*) mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);


    int i;
    for(i = 0; i < ARRAY_SIZE; i++) {
        sharedData[i] = getRandomIntBetween(0, 20);
        printf("Number #%d: %d\n", i, sharedData[i]);
    }



    printf("From Writer:\n");







    // Undo mapping
    if (munmap((void *)sharedData, SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
