#include "../include/view.h"

int main(int argc, char const *argv[])
{
    int fd, i, files;
    char *ptr;
    struct stat shmobj_st;
    char auxBuffer[BUFF_SIZE] = {'\0'};

    //-------------------OBTENER INFO PARA ABRIR SHMEM-----------------------------
    if (argc == 2)
    {
        if (argv[1] == NULL)
        {
            perror("Argumento null");
            exit(1);
        }
        files = atoi(argv[1]);
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

    fd = shm_open(SHM_NAME, O_RDONLY, 00400);
    if (fd == -1)
    {
        perror("Error, la shm no existe");
        exit(1);
    }

    if (fstat(fd, &shmobj_st) == -1)
    {
        perror("Error en fstat");
        exit(1);
    }
    ptr = mmap(NULL, shmobj_st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED)
    {
        perror("Error en el mapeo de memoria");
        exit(1);
    }

    sem_t * sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED)
    {
        perror("Error, el semaforo no existe\n");
        exit(1);
    }

    int offset = 0;

    while (files)
    {
        sem_wait(sem);
        offset += printf("%s", ptr + offset);
        files--;
    }

    if (sem_close(sem) < 0)
    {
        perror("Error al cerrar el semaforo");
        exit(1);
    }

    if (munmap(ptr, shmobj_st.st_size) < 0)
    {
        perror("Error al hacer el unmap de memoria");
        exit(1);
    }

    close(fd);
    return 0;
}
