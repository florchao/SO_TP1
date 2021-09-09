#include "./include/master.h"

int main(int argc, char const *argv[])
{
    int fd_read[SLAVES][2];  // pipe para recibir resultados de slave a master
    int fd_write[SLAVES][2]; // pipe para enviar tareas de master a slave

    check_files(argc, argv);
    create_pipes(fd_read, fd_write);
    create_slaves();
    create_sem();
    create_shm();
    create_result();

    /////////////////
    int files = argc - 1;
    while (files)
    {
        select
    }

    /////////////////

    return 0;
}

///////////////////////////FUNCIONES//////////////////////////////////////////

void check_files(int cant_files, char const *files[])
{
    if (cant_files < 2)
    {
        perror("Cantidad de argumentos inválida");
        exit(1);
    }

    for (int i = 0; i < cant_files; i++)
    {
        if (strstr(files[i], EXT) == NULL)
        {
            perror("Solo se admiten archivos .cnf");
            exit(1);
        }
    }
}

void create_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    for (int i = 0; i < SLAVES; i++)
    {
        if (pipe(fd_read[i]) < 0 || pipe(fd_write[i]) < 0)
        {
            perror("Error al crear los pipes");
            exit(1);
        }
    }
}

void close_pipes(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    for (int i = 0; i < SLAVES; i++)
    {
        if (close(fd_read[i][STDIN_FILENO]) < 0 || close(fd_read[i][STDOUT_FILENO]) < 0 || 
            close(fd_write[i][STDIN_FILENO]) < 0 || close(fd_write[i][STDOUT_FILENO]) < 0 )
        {
            perror("Error al cerrar los pipes");
            exit(1);
        }
    }
}

void create_slaves(int fd_read[SLAVES][2], int fd_write[SLAVES][2])
{
    for(int i = 0; i < SLAVES; i++)
    {
        if( fork() == 0 ) 
        {
            for(int j = 0; j < SLAVES; j++) 
            {
                if(j != i) 
                {
                    if(close(fd_read[j][STDIN_FILENO]) < 0 || close(fd_read[j][STDOUT_FILENO]) < 0 ||
                       close(fd_write[j][STDIN_FILENO]) < 0 || close(fd_write[j][STDOUT_FILENO]) < 0)
                    {
                        perror("Error al cerrar los pipes ajenos al hijo");
                        exit(1);
                    }
                }
            }


        }
        else
        {

        }
    }

}

