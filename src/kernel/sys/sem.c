#ifndef SEM_H_
#define SEM_H_

#include <sys/sem.h>

#endif /* SEM_H_ */

/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/

int semget(unsigned key)
{
    for (int i = 0; i < SEM_MAX; i++)
    {
        if (semTab[i].state == SEM_INVALID_ID)
        {
            semTab[i].state = SEM_CREATED;
            semTab[i].key = key;
            // semTab[i].chain = NULL;
            semTab[i].value = 1;
            return i;
        }
    }

    return -1;
}

int semctl(int semid, int cmd, int val)
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


int semop(int semid, int op)
{

        if (semid < 0 || semid >= SEM_MAX)
            return -1;

        //disable_interrupts();

        struct semaphore sem = semTab[semid];
        

        if(sem.state != SEM_CREATED)
            return -1;


        switch (op)
        {
        case 0:
                if (sem.value > 0)
                {
                        sem.value--;
                        //enable_interrupts();
                        return 1;
                }
                else
                {
                        //enable_interrupts();
                        return 0;
                }
        case 1:
                sem.value++;
                //enable_interrupts();
                return 1;
        default:
                //enable_interrupts();
                return -1;
        }
}


