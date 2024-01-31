#include <../include/sys/sem.h>
#include <sys/sem.c>
#include <sys/sem/semget.c>

PUBLIC int semctl(int semid, int cmd, int val)
{
    struct semaphore sem = semTab[sys_semget(semid)];
    switch (cmd)
    {
    case GETVAL:
        return sem;
    case SETVAL:
        sem.semid = val;
        return 1;
    case IPC_RMID:
        sem.semid = -1;
        return 0;
    default:
        return -1;
    }
}