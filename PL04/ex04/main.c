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
#include <errno.h>

#define SIZE_OF_DATA 50
#define NUMBER_OF_CHILDREN 51

typedef struct
{
    char str[SIZE_OF_DATA][80];
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
    int fd = shm_open("/ex04", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("error shm open");
        exit(EXIT_FAILURE);
    }
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shared_data->writen = 0;

    sem_t *sem;

    if ((sem = sem_open("/sem_ex04", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    int id = spawn_childs(NUMBER_OF_CHILDREN);

    if (id > 0)
    {
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        {
            printf("clock_gettime failed");
            exit(EXIT_FAILURE);
        }
        ts.tv_sec += 15;
        int s;
        /*        while ((s = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
            continue;*/
        s = sem_timedwait(sem, &ts);

        /* Check what happened */

        if (s == -1)
        {
            if (errno == ETIMEDOUT)
            {
                printf("sem_timedwait() timed out\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                perror("sem_timedwait");
                exit(EXIT_FAILURE);
            }
        }

        printf("sem_timedwait() succeeded\n");

        if (SIZE_OF_DATA == shared_data->writen)
        {
            printf("shared memory is full, do you want to overwrite the last record? yes/no\n");
            char answer[4];
            fgets(answer, 4, stdin);
            answer[strlen(answer)] = '\0';
            printf("answer is \"%s\"\n", answer);
            fflush(stdout);  
            if (strcmp(answer, "yes") == 0)
            {
                int index = 0;
                printf("where do you want to write? chose between 0 and %d\n", SIZE_OF_DATA);
                scanf("%d", &index);
                printf("index is \"%d\"\n", index);

                if (index > SIZE_OF_DATA)
                {
                    printf("number too large, write not executed\n");
                }
                else
                {
                    sprintf(shared_data->str[index], "i am process with id %d", id);
                    usleep(100);
                }
            }
            sem_post(sem);
        }
        else
        {
            sprintf(shared_data->str[shared_data->writen], "i am process with id %d", id);
            shared_data->writen++;
            usleep(100);
            sem_post(sem);
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
        //close semaphore
        if (sem_close(sem) < 0)
        {
            perror("Error at sem close son");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    int i;
    for (i = 0; i < NUMBER_OF_CHILDREN; i++)
    {
        wait(NULL);
    }

    for (i = 0; i < shared_data->writen; i++)
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

    //so PAI é que da unlink
    // Remove file from system
    if (shm_unlink("/ex04") < 0)
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
    if (sem_unlink("/sem_ex04") < 0)
    {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }
    return 0;
}
