// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/slave.h"

int main()
{
    char buffer[BUFFER_SIZE] = {'\0'};
    char command[BUFFER_SIZE*2] = {'\0'};
    char result[BUFFER_SIZE] = {'\0'};
    int i;

    while ((i = read(STDIN_FILENO, buffer, sizeof(buffer))) != 0)
    {
        int idx1 = 0;
        int idx2 = 0;
        char bufferAux[BUFFER_SIZE];
        
        while (buffer[idx1] != '\0')
        {
            if (buffer[idx1] != '\n')
                bufferAux[idx2++] = buffer[idx1++];
            else
            {
                bufferAux[idx2] = 0;
                idx1++;
                if (i < 0)
                {
                    printf("PID: %d - ", getpid());
                    perror("Error en el read");
                    exit(1);
                }

                bufferAux[idx2] = '\0';
                if (sprintf(command, "%s %s | %s", MINISAT, bufferAux, GREP) < 0)
                {
                    printf("PID: %d - ", getpid());
                    perror("Error en el sprintf");
                    exit(1);
                }

                FILE *stream = popen((const char *)command, "r");
                if (stream == NULL)
                {
                    printf("PID: %d - ", getpid());
                    perror("Error en esclavo al querer abrir el pipe");
                    exit(1);
                }

                if (fgets(result, BUFFER_SIZE, stream) == NULL)
                {
                    printf("PID: %d - ", getpid());
                    perror("Error en fgets");
                    exit(1);
                }

                int ret = pclose(stream);
                if (ret < 0)
                {
                    printf("PID: %d - ", getpid());
                    perror("Error en esclavo al querer cerrar el pipe");
                    exit(1);
                }

                printf("Nombre: %s Esclavo PID: %d %s", bufferAux, getpid(), result);
                fflush(stdout);
                cleanBuffer(bufferAux);
                cleanBuffer(command);
                idx2 = 0;
            }
        }
        cleanBuffer(buffer);
    }
    return 0;
}

void cleanBuffer(char *buffer)
{
    int j = 0;
    while (buffer[j])
    {
        buffer[j++] = 0;
    }
}