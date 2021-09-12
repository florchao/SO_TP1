#include "shmManager.h"
#include "semManager.h"
#include <sys/wait.h>

#define EXT ".cnf"
#define SEM_NAME "sem"
#define FILE_NAME "../result"
#define SLAVES 5

void check_files(int cant_files, char const * files[]);

void create_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);

void nclose_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int n);

void close_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);

void create_slaves(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int * slaves_pid);

FILE * create_result();

int select_preparation(fd_set * fd_slaves_set, int fd_read[SLAVES][2]);

void cleanBuffer(char *buffer);

int sendTasks(int filesToSend, int slave, const char * argv[], int tIndex, int fd_write[SLAVES][2]);