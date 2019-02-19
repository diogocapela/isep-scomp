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

typedef struct {
    char name[80];
    int number;
} Student;

/*
 oflag: Creation options
 O_CREAT: creates, if not existing, or use existing
• Without O_CREAT tries to use existing (error if none)
 O_EXCL: (with O_CREAT) just creates, error if already exists
 O_TRUNC: eliminates contents already existing in the shared
memory area
 Access Flags
 O_RDONLY: open for reading only
 O_RDWR: open for reading and writing (no flag to just write)
*/

#define MY_SHARED_FILE "/ex03_shared_memory_file"

#define MY_SHM_OPEN_WRITER_OFLAG O_EXCL | O_RDWR | O_CREAT
#define MY_SHM_OPEN_WRITER_MODE S_IRUSR | S_IWUSR

#define MY_SHM_OPEN_READER_OFLAG O_EXCL | O_RDWR
#define MY_SHM_OPEN_READER_MODE S_IRUSR | S_IWUSR
