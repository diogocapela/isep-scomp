#include "main.h"

int main() {

    // Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_READER_OFLAG, MY_SHM_OPEN_READER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Defines the size
    ftruncate(fd, MY_DATA_SIZE);

    // Get a pointer to the data
    Student *sharedData = (Student *)mmap(NULL, MY_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Print the data
    printf("Student Number: %d\n", sharedData->number);
    printf("Student Name: %s\n", sharedData->name);

    // Undo mapping
    if (munmap((void *)sharedData, MY_DATA_SIZE) < 0) {
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
