#include <sys/sem.h>

int sys_semop(int semid, int op)
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

