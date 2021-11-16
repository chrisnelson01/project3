#include "shared.h"
#include <string.h>

int main(int argc, char *argv[])
{
    int fd = shm_open(SHMPATH, O_RDWR, 0);
    if (fd == -1)
    {
        errExit("shm_openP2");
    }
    struct shmbuf *shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE | MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        errExit("mmapP2");
    if (sem_wait(&shmp->sem1) == -1)
    {
        errExit("sem_wait");
    }
    
}