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


typedef struct {
    int maximum_numbers[10];
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

int main(void){
    
    int vec[1000];
    time_t t; //for srand
    ///* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    int i;
    for (i = 0; i < 1000; i++)
    {
        vec[i] = rand() % (1001);
    }
    
    //data to be shared
    shared_data_type *shared_data;

    //size of data
    int size = sizeof(shared_data);

    // Creates and opens a shared memory area
    int fd = shm_open("/ex5", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    shared_data = (shared_data_type *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int id = spawn_childs(10);

    if(id>0){
        int start = (id-1) * 100;
        int finish = start + 100;
        int max = vec[start];
        int i;
        for (i = start; i < finish; i++)
        {
            if(vec[i]> max){
                max = vec[i];
            }
        }
        shared_data->maximum_numbers[id-1]= max;
        printf("filho %d, max é %d\n", id, shared_data->maximum_numbers[id-1]);
    }

    if(id==0){
        int i;
        int status;
        for(i =0; i<10; i++){
            wait(&status);
        }
        int max = shared_data->maximum_numbers[0];
        for(i = 0; i<10; i++){
            if(max<shared_data->maximum_numbers[i]){
                max = shared_data->maximum_numbers[i];
            }
        }
        printf("max number is %d\n", max);
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

    if(id==0){
           // Remove file from system
        if (shm_unlink("/ex5") < 0)
        {
            perror("Error at unlink");
            exit(EXIT_FAILURE);
        }
    }
    return 0;

}