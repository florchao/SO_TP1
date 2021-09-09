#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SLAVES 5
#define EXT ".cnf"
void create_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);
void close_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2]);
void create_slaves();
void check_files(int cant_files, char const * files[]);

