#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SEM_NAME "sem"
#define SHM_NAME "shm"
#define SIZE 40
#define BUFF_SIZE 256 