#include <sys/sem.h>


PUBLIC int semop(int semid, int op)
{
        //disable_interrupts();

        struct semaphore sem = semTab[semid];
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
                        sem.state = SEM_BLOCKED;
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

