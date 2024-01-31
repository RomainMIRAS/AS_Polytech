#include <../include/sys/sem.h>

PUBLIC int semctl(int semid, int cmd, int val)
{

    if (semid < 0 || semid >= SEM_MAX)
        return -1;

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
        sem.state = SEM_INVALID_ID;
        sem.chain = NULL;
        return 1;
    default:
        return -1;
    }
}