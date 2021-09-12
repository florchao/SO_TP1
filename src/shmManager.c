// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/shmManager.h"

/* codigo basado en
 https://github.com/WhileTrueThenDream/ExamplesCLinuxUserSpace/blob/master/sm_create.c 
  https://github.com/WhileTrueThenDream/ExamplesCLinuxUserSpace/blob/master/sm_write.c  */

int create_shm()
{
    shm_unlink(SHM_NAME);
    int fd;
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 00700);
    if (fd == -1)
    {
        perror("Error al crear la shm");
        exit(1);
    }
    if (-1 == ftruncate(fd, SHM_SIZE))
    {
        perror("Error al establecer el tamaño de la shm");
        exit(1);
    }
    return fd;
}

char * mmap_shm(int fd) // aca hay que mandarle argc - 1
{
    char * shm_ptr = mmap(NULL, SHM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("Error al mapear la shm");
        exit(1);
    }
    return shm_ptr;
}

int write_shm(char * shm_ptr, char * buffer)
{
    int size = strlen(buffer);
    memcpy(shm_ptr, buffer, size);
    return size;
}

char * ropen_shm(int * fd)
{
    struct stat shmobj_st;
    *fd = shm_open(SHM_NAME, O_RDONLY, 00400);
    if (*fd == -1)
    {
        perror("Error, la shm no existe");
        exit(1);
    }
    if (fstat(*fd, &shmobj_st) == -1)
    {
        perror("Error en fstat");
        exit(1);
    }
    char * ptr = mmap(NULL, shmobj_st.st_size, PROT_READ, MAP_SHARED, *fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Error en el mapeo de memoria");
        exit(1);
    }
    return ptr;
}

void close_shm(int shm_fd, char * shm_ptr)
{
    if(close(shm_fd) < 0)
    {
        perror("Error al cerrar la shm");
        exit(1);
    }
    if(munmap(shm_ptr, SHM_SIZE) < 0)
    {
        perror("Error al unmapear la shm");
        exit(1); 
    }
}

void unlink_shm()
{
    if (shm_unlink(SHM_NAME) < 0)
    {
        perror("Error al hacer el unlink de la shm");
        exit(1);
    }
}
