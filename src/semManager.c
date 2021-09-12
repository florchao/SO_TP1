// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/semManager.h"

sem_t * create_sem()
{
    sem_unlink(SEM_NAME);
    sem_t * sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED)
    {
        perror("Error al crear el semaforo");
        exit(1);
    }
    return sem;
}

void post_sem(sem_t * sem)
{
    if (sem_post(sem) < 0)
    {
        perror("Error en el post del semaforo");
        exit(1);
    }
}

void close_sem(sem_t * sem) 
{
    if (sem_close(sem) < 0)
    {
        perror("Error al cerrar el semaforo");
        exit(1);
    }
}

void unlink_sem()
{
    if(sem_unlink(SEM_NAME) < 0)
    {
        perror("Error al hacer el unlink del semaforo");
        exit(1);
    }
}

sem_t * open_sem()
{
    sem_t * sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED)
    {
        perror("Error, el semaforo no existe");
        exit(1);
    }
    return sem;
}

void wait_sem(sem_t * sem)
{
    if(sem_wait(sem) < 0)
    {
        perror("Error en el wait del semaforo");
        exit(1);
    }
}