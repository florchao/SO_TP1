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
    int slaves_pid[slaves];

    create_pipes(fd_read, fd_write);
    create_slaves(fd_read, fd_write, slaves_pid);
    sem_t * sem = create_sem();
    char * shm_ptr;
    int shm = create_shm(shm_ptr);
    shm_ptr = mmap(NULL, BUFFER_SIZE * (argc-1), PROT_WRITE, MAP_SHARED, shm, 0);
    if(shm_ptr == MAP_FAILED)
    {
        perror("Error al mapear la shm");
        exit(1);
    }
    
    printf("%d\n", argc-1);
    fflush(stdout);
    
    sleep(2);
    FILE * result = create_result();
    
    int filesToSend = argc - 1;
    int filesFetched = 0;
    fd_set fd_slaves_set;
    int max = 0, tIndex = 1;
    char buffer[BUFFER_SIZE] = {'\0'};
    char task[BUFFER_SIZE] = {'\0'};

    int i;
    for(i=0; i < slaves; i++)
    {
        if(filesToSend > 0)
        {
            strcpy(task, argv[tIndex++]);
            strcat(task, "\n");
            if( write(fd_write[i][STDOUT_FILENO], task, strlen(task)) < 0 )
            {
                perror("Error al enviar la tarea");
                exit(1);
            }
            cleanBuffer(task);
            filesToSend--;
        }
    }

    while (filesFetched < argc - 1)
    { 
        max = select_preparation(&fd_slaves_set, fd_read);
        int okay = select(max, &fd_slaves_set, NULL, NULL, NULL);
        if (okay < 0)
        {
            perror("Error en la syscall select");
            exit(1);
        }
        else if(okay)
        {
            int i;
            for (i = 0; i < slaves; i++)
            {
                if (FD_ISSET(fd_read[i][STDIN_FILENO], &fd_slaves_set))
                {
                    int n;
                    if( (n = read(fd_read[i][STDIN_FILENO], buffer, sizeof(buffer))) < 0 ) 
                    {
                        perror("Error al leer el resultado del esclavo");
                        exit(1);
                    }
                    if( n != fwrite(buffer,sizeof(char),n,result) ) 
                    {
                        perror("Error al escribir en el archivo resultado");
                        exit(1);
                    }
                    fflush(result);
                    
                    int size = strlen(buffer);
                    memcpy(shm_ptr,buffer, size);
                    shm_ptr += size;

                    if(sem_post(sem) < 0) 
                    {
                        perror("Error en el post del semaforo");
                        exit(1);
                    }
                    filesFetched++;
                    cleanBuffer(buffer);
                
                    if(filesToSend > 0)
                    {
                        strcpy(task, argv[tIndex++]);
                        strcat(task, "\n");
                        if( write(fd_write[i][STDOUT_FILENO], task, strlen(task)) < 0 )
                        {
                            perror("Error al enviar la tarea");
                            exit(1);
                        }
                        cleanBuffer(task);
                        filesToSend--;
                    }
                }
            }
        }
    }
   
    close(shm);
    munmap(shm_ptr,SHM_SIZE);
     if (shm_unlink(SHM_NAME) < 0)
    {
        perror("Error al hacer el unlink de la shm");
        exit(1);    
    }

    if (sem_close(sem) < 0 || sem_unlink(SEM_NAME) < 0)
    {
        perror("Eror al cerrar o unlink el semaforo");
        exit(1);
    }

    close_pipes(fd_read, fd_write);
    fclose(result);
    int k;
    for(k = 0; k < slaves; k++) 
    {
        waitpid(slaves_pid[k], NULL, 0);
    }

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

void create_slaves(int fd_read[SLAVES][2], int fd_write[SLAVES][2], int * slaves_pid)
{
    /* basado en https://github.com/mit-pdos/xv6-riscv/blob/riscv/user/sh.c */
    int i;
    for (i = 0; i < slaves; i++)
    {
        if ((slaves_pid[i] = fork()) == 0)
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
        else if (slaves_pid[i] < 0)
        {
            perror("Error al realizar fork");
            exit(1);
        }
    }
}

sem_t * create_sem()
{
    sem_t * sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED)
    {
        perror("Error al crear el semaforo");
        exit(1);
    }
    return sem;
}

/* codigo de https://github.com/WhileTrueThenDream/ExamplesCLinuxUserSpace/blob/master/sm_create.c 
             https://github.com/WhileTrueThenDream/ExamplesCLinuxUserSpace/blob/master/sm_write.c  */
int create_shm(char * ptr)
{
    int fd;
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 00700);
    if (fd == -1)
    {
        perror("Error al crear la shm");
        exit(1);
    }
    if (-1 == ftruncate(fd, SHM_SIZE))
    {
        perror("Error al modificar el tamaño de la shm");
        exit(1);
    }
    return fd;
}

FILE * create_result()
{
    FILE *file;
    file = fopen(FILE_NAME, "w+");
    if (file == NULL)
    {
        perror("Error al crear el archivo");
        exit(1);
    }
    return file;
}

int select_preparation(fd_set * fd_slaves_set, int fd_read[SLAVES][2])
{
    FD_ZERO(fd_slaves_set);
    int max = 0;

    int i = 0;
    for (; i < slaves ; i++)
    {
        FD_SET(fd_read[i][STDIN_FILENO], fd_slaves_set); 
        if (fd_read[i][STDIN_FILENO] >max)
        {
            max = fd_read[i][STDIN_FILENO] + 1;
        }
    }
    return max;
}

void cleanBuffer(char *buffer)
{
    int j = 0;
    while (buffer[j])
    {
        buffer[j++] = 0;
    }
}