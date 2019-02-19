#include "includes.h"

int main() {

    const int ARRAY_SIZE = 10;
    const int SIZE = ARRAY_SIZE * sizeof(int);

    // Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_READER_OFLAG, MY_SHM_OPEN_READER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Defines the size
    ftruncate(fd, SIZE);

    // Get a pointer to the data
    int *sharedData = (int *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);



    // Calculate the average

    int total = 0;

    int i;
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("NUM %d\n", sharedData[i]);
        total = total + sharedData[i];
    }

    int average = total / ARRAY_SIZE;



    printf("Average of the Shared Array: %d\n", average);



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

    // Remove file from system
    if (shm_unlink(MY_SHARED_FILE) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
