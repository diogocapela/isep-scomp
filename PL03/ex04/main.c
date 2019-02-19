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

int main() {
    char *SHARED_FILE = "/ex04_shared_memory_file";

    const int INITIAL_VALUE = 100;
    const int DATA_SIZE = sizeof(int);
    const int NUMBER_OF_OPERATIONS = 1000000;

    // 1. Creates and opens a shared memory area
    int fd = shm_open(SHARED_FILE, /*O_EXCL |*/ O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Defines the size
    ftruncate(fd, DATA_SIZE);

    // 3. Get a pointer to the data
    int *sharedData = (int *)mmap(NULL, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // 4. Initialize the data
    *sharedData = INITIAL_VALUE;

    // Create a new process
    pid_t pid = fork();

    // Both father and child
    int i;
    for (i = 0; i < NUMBER_OF_OPERATIONS; i++) {
        *sharedData = *sharedData + 1;
        *sharedData = *sharedData - 1;
    }

    // If we are at the father
    if (pid > 0) {
        printf("FATHER Value: %d\n", *sharedData);

        wait(NULL);

        // Remove file from system
        if (shm_unlink(SHARED_FILE) < 0) {
            printf("Error at shm_unlink()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // If we are at the child
    if (pid == 0) {
        printf("CHILD Value: %d\n", *sharedData);
    }

    // Undo mapping
    if (munmap((void *)sharedData, DATA_SIZE) < 0) {
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
