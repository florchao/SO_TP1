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
            perror("error null argument");
            abort();
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
            abort();
        }
        strcpy(shm_name, strtok(auxBuffer,"-"));
        strcpy(sem_name, strtok(auxBuffer + strlen(shm_name) + 1 ,"-"));
        files = atoi(strtok(auxBuffer + strlen(shm_name) +2 + strlen(sem_name),"-"));
    }
    else
    {
        perror("cantidad de argumentos invalida");
        abort();
    }

    //----------------------------------------------------------------------------

    fd = shm_open(SMOBJ_NAME, O_RDONLY, 00400); 
    if (fd == -1)
    {
        printf("Error file descriptor %s\n", strerror(errno));
        exit(1);
    }

    if (fstat(fd, &shmobj_st) == -1)
    {
        printf(" error fstat \n");
        exit(1);
    }
    ptr = mmap(NULL, shmobj_st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed in read process: %s\n", strerror(errno));
        exit(1);
    }

    sem_t * my_semaphore = sem_open(sem_name,0); 
    if(my_semaphore == SEM_FAILED){
        printf("Error with the address of the semaphore %s\n", strerror(errno));
	    exit(1);
    }
    
    //falta ver como imprime.
    
    sem_close(sem_name);
    munmap(ptr,size);
    close(fd);
}
