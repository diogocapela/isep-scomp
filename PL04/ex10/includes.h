#include <errno.h>
#include <fcntl.h>
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

#define NUMBER_OF_RECORDS 100

#define SHM_FILE_NAME "/ex10_shm_file_name"

#define SEMAFORO_FILE_NAME "semaforo_ex_10"

typedef struct {
    char name[255];
    char address[255];
    int number;
} Record;

typedef struct {
    Record records[NUMBER_OF_RECORDS];
} SharedDataStruct;

// Semaforos Utils
//===============================================================================================================

void __semWAIT(sem_t *semaforo) {
    if (sem_wait(semaforo) == -1) {
        perror("Error at sem_wait()!");
        exit(EXIT_FAILURE);
    }
}

void __semPOST(sem_t *semaforo) {
    if (sem_post(semaforo) == -1) {
        perror("Error at sem_post()!");
        exit(EXIT_FAILURE);
    }
}

// ONLY FOR THOSE WHO CREATE THE SEMAFORO
void __semaforo__CREATE_AND_OPEN(sem_t *semaforo, char* fileName, int initialValue) {
    if ((semaforo = sem_open(fileName, O_CREAT | O_EXCL, 0644, initialValue)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
}

// ONLY FOR THOSE WHO OPEN THE SEMAFORO
// no caso dos filhos, so quererem abrir o semaforo temos q fazer apenas -> sem_open(fileName, 0)
void __semaforo__OPEN(sem_t *semaforo, char* fileName) {
    if ((semaforo = sem_open(fileName, 0)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
}

void __semaforo__CLOSE(sem_t *semaforo) {
    if (sem_close(semaforo) < 0) {
        perror("Error at sem_close()!\n");
        exit(EXIT_FAILURE);
    }
}

void __semaforo__UNLINK(char* fileName) {
    if (sem_unlink(fileName) != 0) {
        int err = errno;
        printf("Error: %s", strerror(err));
        printf("Error at shm_unlink() of semaforo!\n");
        exit(EXIT_FAILURE);
    }
}

// Shared Memory Utils
//===============================================================================================================

// int oflag = O_RDWR | O_EXCL | O_CREAT

SharedDataStruct *__shmOPEN(char *fileName, int *fd, int oflag) {
    // 1. Creates and opens a shared memory area
    *fd = shm_open(fileName, oflag, S_IRUSR | S_IWUSR);
    if (*fd == -1) {
        perror("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Defines the size
    ftruncate(*fd, sizeof(SharedDataStruct));

    // 3. Get a pointer to the data
    SharedDataStruct *sharedData = (SharedDataStruct *)mmap(NULL, sizeof(SharedDataStruct), PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);

    return sharedData;
}

void __shmCLOSE(SharedDataStruct *sharedData, int fd) {
    // Undo mapping
    if (munmap((void *)sharedData, sizeof(SharedDataStruct)) < 0) {
        perror("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }
    // Close file descriptor
    if (close(fd) < 0) {
        perror("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
}

void __shmDELETE(char *fileName) {
    // Remove file from system
    if (shm_unlink(fileName) < 0) {
        perror("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }
}

// Fork Utils
//===============================================================================================================

int cria_filhos(int n) {
    pid_t pid;
    int i;
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            return -1;
        } else if (pid == 0) {
            return i + 1;
        }
    }
    return 0;
}

// Random Utils
//===============================================================================================================

int getRandomIntBetween(int min, int max) {
    srand(time(NULL) * getpid() << 16);
    int randomNumber = (rand() % max) + min;
    return randomNumber;
}

void printMessage(char *message) {
    printf("%s", message);
    fflush(stdout);
}