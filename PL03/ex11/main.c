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
#define SIZE_OF_BUFFER 10
#define TOTAL_REPETITIONS 3
#define ESCREVI 1
#define LI 0
#define LEITURA 0
#define ESCRITA 1

typedef struct
{
    int buffer[SIZE_OF_BUFFER];
    int written;
    int read;
} circular_buffer;

int check_diff(int written, int read)
{
    return written - read;
}

int main(void)
{

    int fd_pipe[2][2];
    //data to be shared
    circular_buffer *circular_buffer1;

    //size of data
    int size = sizeof(circular_buffer);

    // Creates and opens a shared memory area
    int fd = shm_open("/ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    // Defines the size
    ftruncate(fd, size);

    // Get a pointer to the data
    circular_buffer1 = (circular_buffer *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //inicializar numeros escritos e lidos
    circular_buffer1->written = 0;
    circular_buffer1->read = 0;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    //codigo do pai
    if (pid > 0)
    {
        close(fd_pipe[ESCREVI][LEITURA]);
        close(fd_pipe[LI][ESCRITA]);
        int flag;
        int i;
        int count = 0;
        for (i = 0; i < SIZE_OF_BUFFER; i++)
        {
            
            //se chegou a fim de buffer
            if (i == SIZE_OF_BUFFER-1)
            {   
               //aumentar count para podermos escrever valores incrementais 
               count++;
               //regressar ao inicio do buffer
                i = 0;
            }
            //escrever numero no buffer, p.ex count =1 e i=2, escreve 12
            circular_buffer1->buffer[i] = (count * SIZE_OF_BUFFER) + i;
            printf("escrevi o numero %d\n", (count * SIZE_OF_BUFFER) + i);
            circular_buffer1->written++;
            
            //se count for 3 ja escrevemos 30 inteiros
            if (count == TOTAL_REPETITIONS)
                break;

            write(fd_pipe[ESCREVI][ESCRITA], &i, sizeof(i));
            read(fd_pipe[LI][LEITURA], &flag, sizeof(flag));   
        }

        close(fd_pipe[LI][LEITURA]);
        close(fd_pipe[ESCREVI][ESCRITA]);

        // Undo mapping
        if (munmap(circular_buffer1, size) == -1)
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
        //esperar que filho acabe de ler
        wait(NULL);
    }
    else
    {   
        close(fd_pipe[LI][LEITURA]);
        close(fd_pipe[ESCREVI][ESCRITA]);
        int flag;
        int i;
        int temp;
        for (i = 0; i < SIZE_OF_BUFFER; i++)
        {
            
            read(fd_pipe[ESCREVI][LEITURA], &flag, sizeof(flag));
           
            //se estivermos no fim do buffer começar a ler outra vez
            if (i == SIZE_OF_BUFFER-1)
            {
                i = 0;
            }//guardar variavel lida e aumentar read
            temp = circular_buffer1->buffer[i];
            circular_buffer1->read++;
            printf("li o numero %d\n", temp);
            write(fd_pipe[LI][ESCRITA], &i, sizeof(i));

            //Se ja estivermos no ultimo dar break pois nao queremos ler mais
            if (circular_buffer1->read == SIZE_OF_BUFFER*TOTAL_REPETITIONS-1)
                break;
        }

        close(fd_pipe[ESCREVI][LEITURA]);
        close(fd_pipe[LI][ESCRITA]);

        // Undo mapping
        if (munmap(circular_buffer1, size) == -1)
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
        if (pid == 0)
        {
            // Remove file from system
            if (shm_unlink("/ex10") < 0)
            {
                perror("Error at unlink");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}