#include <sys/sem.h>

PUBLIC int sys_semget(unsigned key)
{
    struct proc *caller = proc_table + who_p;
    int i;
    for (i = 0; i < SEM_NUMBER; i++)
    {
        if (semaphores[i].sem_key == key)
        {
            caller->semaphore = i;
            return i;
        }
    }
    return -1;
}
