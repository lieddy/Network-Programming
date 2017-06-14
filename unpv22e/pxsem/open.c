#include "unpipc.h"

int main(int argc,char** argv)
{
    sem_t* semFD;
    semFD = Sem_open("wintone",O_CREAT|O_RDWR,0666,1);
    if (semFD == SEM_FAILED)
        perror("Sem_open: "),exit(-1);
    return 0;
}
