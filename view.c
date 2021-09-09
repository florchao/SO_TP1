#include "./include/view.h"

int main(int argc, char const *argv[])
{
    int fd, i, files;
    char *ptr;
    struct stat shmobj_st;
    char auxBuffer[BUFF_SIZE] = {'\0'};
    char shm_name[SIZE] = {'\0'};
    char sem_name[SIZE] = {'\0'};

    //-------------------OBTENER INFO PARA ABRIR SHMEM-----------------------------
    if (argc == 4)
    {
        if (argv[1] == NULL || argv[2] == NULL)
        {
            perror("Argumentos null");
            exit(1);
        }
        strcpy(shm_name, argv[1]);
        strcpy(sem_name, argv[2]);
        files = atoi(argv[3]);
    }
    else if (argc == 1)
    {
        if ((i = read(STDIN_FILENO, auxBuffer, sizeof(auxBuffer))) == -1)
        {
            perror("error en read");
            exit(1);
        }
        char *token;
        token = strtok(auxBuffer, "-");
        strcpy(shm_name, token);
        token = strtok(NULL, "-");
        strcpy(sem_name, token);
        token = strtok(NULL, "-");
        files = atoi(token);
    }
    else
    {
        perror("cantidad de argumentos invalida");
        exit(1);
    }

    printf("%s, %s, %d\n", shm_name, sem_name, files); // DEBUG

    //----------------------------------------------------------------------------

    fd = shm_open(shm_name, O_RDONLY, 00400);
    if (fd == -1)
    {
        printf("Error, la shm no existe\n");
        exit(1);
    }

    if (fstat(fd, &shmobj_st) == -1)
    {
        printf("Error en fstat\n");
        exit(1);
    }
    ptr = mmap(NULL, shmobj_st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (ptr == MAP_FAILED)
    {
        printf("Error en el mapeo de memoria\n");
        exit(1);
    }

    sem_t *my_semaphore = sem_open(sem_name, 0);
    if (my_semaphore == SEM_FAILED)
    {
        printf("Error, el semaforo no existe\n");
        exit(1);
    }

    int offset = 0;

    while (files)
    {
        sem_wait(my_semaphore);
        offset += printf("%s \n", ptr + offset);
        files--;
    }

    if (sem_close(my_semaphore) < 0)
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
