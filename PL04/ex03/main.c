#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct
{
    char str[50][80];
    int writen;
} shared_data_type;

int spawn_childs(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

int main()
{

    shared_data_type *shared_data;

    int size = sizeof(shared_data_type);

    // Creates and opens a shared memory area
    int fd = shm_open("/ex03", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shared_data->writen = 0;

    sem_t *sem;

    if ((sem = sem_open("/sem_ex03", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    int id = spawn_childs(50);

    if (id > 0)
    {
        sem_wait(sem);
        printf("writing\n");
        sprintf(shared_data->str[shared_data->writen], "i am process with id %d", id);
        shared_data->writen++;
        sleep(1);
        sem_post(sem);
        // Undo mapping
        if (munmap(shared_data, size) == -1)
        {
            perror("munmap failed");
            exit(EXIT_FAILURE);
        }
        // Close file descriptor
        if (close(fd) == -1)
        {
            perror("close failed");
            exit(EXIT_FAILURE);
        }
        //close semaphore
        if (sem_close(sem) < 0)
        {
            perror("Error at sem close");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    int i;
    for (i = 0; i < 50; i++)
    {
        wait(NULL);
    }
    for (i = 0; i < 50; i++)
    {
        printf("%s\n", shared_data->str[i]);
    }
    // Undo mapping
    if (munmap(shared_data, size) == -1)
    {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    //so filho é que da unlink

    // Remove file from system
    if (shm_unlink("/ex03") < 0)
    {
        perror("Error at unlink share");
        exit(EXIT_FAILURE);
    }
    //close semaphore
    if (sem_close(sem) < 0)
    {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
    //unlink semaphote
    if (sem_unlink("/sem_ex03") < 0)
    {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }

    return 0;
}
