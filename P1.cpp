
#include <ctype.h>
#include "shared.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/stat.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file = argv[1];
    shm_unlink(SHMPATH);
    int fd = shm_open(SHMPATH, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        errExit("shm_open");
    }
    if (ftruncate(fd, sizeof(struct shmbuf)) == -1) 
    {
        errExit("ftruncate");
    }
    struct shmbuf* shmp = (shmbuf*)mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
    {
        errExit("mmap");
    }
    
    if(sem_init(&shmp->sem1, 1, 0) == -1)
    {
        errExit("sem_init-sem1");
    }
    if(sem_init(&shmp->sem2, 1, 0) == -1)
    {
        errExit("sem_init-sem2");
    }
    FILE *fptr = fopen(file,"r");
    if(fptr==NULL)
    {
        errExit("file Null");
    }
    struct stat st;
    size_t fsize = st.st_size;

    size_t* pfsize = (size_t*)shmp;
    char* p = (char*) (pfsize+1);
    size_t nread = fread(p,1, fsize, fptr);
    *pfsize = nread;
    p[nread] = '\n';
    p[nread+1] = 0;
    

    shm_unlink(SHMPATH);

    exit(EXIT_SUCCESS);
    return 0;
}