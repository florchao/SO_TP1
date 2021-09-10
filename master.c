#include "./include/master.h"

int slaves = SLAVES;
int main(int argc, const char *argv[])
{
    check_files(argc, argv);
    if (argc - 1 < SLAVES)
    {
        slaves = argc - 1;
    }

    int fd_read[slaves][2];
    int fd_write[slaves][2];
    create_pipes(fd_read, fd_write);
    create_slaves(fd_read, fd_write);
    sem_t *sem = create_sem();
    int shm = create_shm();
    FILE *result = create_result();

    /*
    /////////////////
    int files = argc - 1;
    while (files)
    {
       // select
    }

    */
    //sleep(30);

    close_pipes(fd_read, fd_write);

    return 0;
}

///////////////////////////FUNCIONES//////////////////////////////////////////

void check_files(int cant_files, const char *files[])
{
    if (cant_files < 2)
    {
        perror("Cantidad de argumentos inválida");
        exit(1);
    }

    struct stat stats;
    int i;

    for (i = 1; i < cant_files; i++)
    {
        if (strstr(files[i], EXT) == NULL)
        {
            perror("Solo se admiten archivos .cnf");
            exit(1);
        }

        if (stat(files[i], &stats) == -1)
        {
            perror("El archivo no existe");
            exit(1);
        }
    }
}

void create_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    int i;
    for (i = 0; i < slaves; i++)
    {
        if (pipe(fd_read[i]) < 0 || pipe(fd_write[i]) < 0)
        {
            perror("Error al crear los pipes");
            exit(1);
        }
    }
}

void nclose_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int n)
{
    int i;
    for (i = 0; i < slaves; i++)
    {
        if (i != n)
        {
            if (close(fd_read[i][STDIN_FILENO]) < 0 || close(fd_read[i][STDOUT_FILENO]) < 0 ||
                close(fd_write[i][STDIN_FILENO]) < 0 || close(fd_write[i][STDOUT_FILENO]) < 0)
            {
                perror("Error al cerrar los pipes");
                exit(1);
            }
        }
    }
}

void close_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    nclose_pipes(fd_read, fd_write, -1);
}

void create_slaves(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    /* basado en https://github.com/mit-pdos/xv6-riscv/blob/riscv/user/sh.c */

    int i;
    for (i = 0; i < slaves; i++)
    {
        int pid;
        if ((pid = fork()) == 0)
        {
            nclose_pipes(fd_read, fd_write, i);
            close(fd_write[i][STDOUT_FILENO]);
            close(fd_read[i][STDIN_FILENO]);

            if (dup2(fd_write[i][STDIN_FILENO], STDIN_FILENO) < 0)
            {
                perror("Error en el dup 1");
                exit(1);
            }

            if (dup2(fd_read[i][STDOUT_FILENO], STDOUT_FILENO) < 0)
            {
                perror("Error en el dup 2");
                exit(1);
            }

            char *const paths[] = {"./slave", NULL};
            if (execv(paths[0], paths) < 0)
            {
                perror("Error al ejecutar el esclavo");
                exit(1);
            }
        }
        else if (pid < 0)
        {
            perror("Error al realizar fork");
            exit(1);
        }
    }
}

sem_t create_sem()
{
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED)
    {
        perror("Error al crear el semaforo");
        exit(1);
    }
    return sem;
}

/* codigo de https://github.com/WhileTrueThenDream/ExamplesCLinuxUserSpace/blob/master/sm_create.c*/
int createSHM()
{
    int fd;
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 00700);
    if (fd == -1)
    {
        perror("Error al crear la shm");
        exit(1);
    }
    if (-1 == ftruncate(fd, SIZEOF_SMOBJ))
    {
        perror("Error al modificar el tamaño de la shm");
        exit(1);
    }

    return fd;
}

FILE *create_result()
{
    FILE *file;
    file = fopen(FILE_NAME, S_IWRITE | S_IROTH);
    if (file == NULL)
    {
        perror("Error al crear el archivo");
        exit(1);
    }
    return file;
}