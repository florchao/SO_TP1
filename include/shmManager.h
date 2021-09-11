#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SHM_NAME "shm"
#define SHM_SIZE 2048
#define BUFFER_SIZE 256

int create_shm();
char * mmap_shm(int files);
int write_shm(char * shm_ptr, char * buffer);
void close_shm(int shm_fd);
char * ropen_shm();