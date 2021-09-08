#include "./include/slave.h"

int main()
{

    char buffer[BUFFER_SIZE] = {'\0'};
    char command[BUFFER_SIZE] = {'\0'};
    char result[BUFFER_SIZE] = {'\0'};
    int i;

    while ((i = read(STDIN_FILENO, buffer, sizeof(buffer))) != 0)
    {
        if (i < 0)
        {
            printf("PID: %d - ", getpid());
            perror("Error en el read");
            abort();
        }

        buffer[i - 1] = '\0';
        if (sprintf(command, " %s %s | %s", MINISAT, buffer, GREP) < 0)
        {
            printf("PID: %d - ", getpid());
            perror("Error en el sprintf");
            abort();
        }

        FILE *stream = popen((const char *)command, "r");
        if (stream == NULL)
        {
            printf("PID: %d - ", getpid());
            perror("Error en esclavo al querer abrir el pipe");
            abort();
        }

        if( fgets(result, BUFFER_SIZE, stream) == NULL ) {
            printf("PID: %d - ", getpid());
            perror("Error en fgets");
            abort();
        }

        int ret = pclose(stream);
        if (ret < 0)
        {
            printf("PID: %d - ", getpid());
            perror("Error en esclavo al querer cerrar el pipe");
            abort();
        }

        printf("Nombre: %s\nEsclavo PID: %d\n%s\n", buffer, getpid(), result);
        cleanBuffer(buffer);
    }
}

void cleanBuffer(char *buffer)
{
    int j = 0;
    while (buffer[j])
    {
        buffer[j++] = 0;
    }
}