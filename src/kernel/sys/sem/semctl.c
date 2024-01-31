#include <../include/sys/sem.h>
#include <sys/sem.c>
#include <sys/sem/semget.c>

PUBLIC int sys_semctl(int semid, int cmd, int val)
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
        return destroy(semid);
    default:
        return -1;
    }
}