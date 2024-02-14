#include <nanvix/sem.h>
#include <sys/sem.h>

int sys_semctl(int semid, int cmd, int val)
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
        sem.value = val;
        return 1;
    case IPC_RMID:
        sem.state = SEM_INVALID_ID;
        // sem.chain = NULL;
        return 1;
    default:
        return -1;
    }
}