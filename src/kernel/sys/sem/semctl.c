#include <../include/sys/sem.h>
#include <sys/sem/semget.c>

PUBLIC int semctl(int semid, int cmd, int val)
{
    struct semaphore sem = semTab[semid];

    if(sem.state != SEM_CREATED)
        return -1;

    switch (cmd)
    {
    case GETVAL:
        return sem.value;
    case SETVAL:
        sema.value = val;
        return 1;
    case IPC_RMID:
        sem.semid = -1;
        sem.chain = NULL;
        return 1;
    default:
        return -1;
    }
}