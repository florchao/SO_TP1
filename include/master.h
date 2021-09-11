#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SLAVES 5
#define EXT ".cnf"
#define SEM_NAME "sem"
#define SHM_NAME "shm"
#define FILE_NAME "result"
#define SHM_SIZE 512
#define BUFFER_SIZE 256

void check_files(int cant_files, char const * files[]);

void create_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);

void nclose_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int n);

void close_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);

void create_slaves(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int * slaves_pid);

sem_t * create_sem();

int create_shm();

FILE * create_result();

int select_preparation(fd_set * fd_slaves_set, int fd_read[SLAVES][2]);

void cleanBuffer(char *buffer);