// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/view.h"

int main(int argc, char const *argv[])
{
    int fd, i, files;
    char auxBuffer[BUFFER_SIZE] = {'\0'};

    //----------------------OBTENER INFO PARA ABRIR SHMEM-----------------------------
    if (argc == 2)
    {
        if (argv[1] == NULL)
        {
            perror("Argumento null");
            exit(1);
        }
        files = atoi(argv[1]);
        if(files == 0)
        {
            perror("Argumento invalido");
            exit(1);
        }
    }
    else if (argc == 1)
    {
        if ((i = read(STDIN_FILENO, auxBuffer, sizeof(auxBuffer))) == -1)
        {
            perror("error en read");
            exit(1);
        }
        files = atoi(auxBuffer);
    }
    else
    {
        perror("cantidad de argumentos invalida");
        exit(1);
    }
    //----------------------------------------------------------------------------

    char * ptr = ropen_shm(&fd);
    sem_t * sem = open_sem();
    
    int offset = 0;
    while (files)
    {
        wait_sem(sem);
        offset += printf("%s", ptr + offset);
        files--;
    }

    close_sem(sem);
    close_shm(fd,ptr);
    return 0;
}
