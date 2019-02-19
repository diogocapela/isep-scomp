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

#define STR_SIZE 50
#define NR_DISC 10

typedef struct {
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
} Aluno;

typedef struct {
    Aluno aluno;
    int hasNewData;
} SharedDataStruct;

int main() {
    // Name the shared memory file
    char *SHARED_FILE = "/ex08_shared_memory_file";

    // Set the size of the shared memory data
    const int DATA_SIZE = sizeof(SharedDataStruct);

    // 1. Creates and opens a shared memory area
    int fd = shm_open(SHARED_FILE, /*O_EXCL |*/ O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Defines the size
    ftruncate(fd, DATA_SIZE);

    // 3. Get a pointer to the data
    SharedDataStruct *sharedData = (SharedDataStruct *)mmap(NULL, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Fork a new process
    pid_t pid = fork();

    // Check for errors
    if (pid < 0) {
        perror("Fork failed!");
        exit(EXIT_FAILURE);
    }

    // If we are at the father process
    //=============================================================
    if (pid > 0) {
        printf("FATHER PROCESS\n");
        printf("=========================================\n");

        // Get numero from input
        printf("Insira o numero do aluno:\n");
        scanf("%d", &(sharedData->aluno.numero));

        // Get nome from input
        printf("Insira o nome do aluno:\n");
        scanf("%s", sharedData->aluno.nome);

        // Get notas das disciplinas from input
        int i;
        for (i = 0; i < NR_DISC; i++) {
            printf("Insira a nota da disciplina #%d:\n", i);
            scanf("%d", &(sharedData->aluno.disciplinas[i]));
        }

        // Set the new data flag to true
        sharedData->hasNewData = 1;

        // Wait fot the child process to exit
        wait(NULL);

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

        // Remove file from system
        if (shm_unlink(SHARED_FILE) < 0) {
            printf("Error at shm_unlink()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // If we are at the child process
    //=============================================================
    if (pid == 0) {
        // Active waiting for the file to have new data
        while (sharedData->hasNewData == 0) {

        }

        printf("CHILD PROCESS\n");
        printf("=========================================\n");

        int highestGrade = sharedData->aluno.disciplinas[0];
        int lowestGrade = sharedData->aluno.disciplinas[0];
        int totalGrades = 0;
        int average = 0;

        // Loop on all the grades
        int i;
        for (i = 0; i < NR_DISC; i++) {
            int grade = sharedData->aluno.disciplinas[i];

            totalGrades = totalGrades + grade;

            if (grade > highestGrade) {
                highestGrade = grade;
            }

            if (grade < lowestGrade) {
                lowestGrade = grade;
            }
        }

        // Calculate average
        average = totalGrades / NR_DISC;

        // Print the results
        printf("Highest grade: %d\n", highestGrade);
        printf("Lowest grade: %d\n", lowestGrade);
        printf("Average grade: %d\n", average);

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

        exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
