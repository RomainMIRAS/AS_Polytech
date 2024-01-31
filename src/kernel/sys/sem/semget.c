#include <sys/sem.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
PUBLIC int sys_semget(unsigned key)
{
    int i;
    for (i = 0; i < SEM_MAX; i++)
    {
        if (semTab[i].semid == key)
        {
            return i;
        }
    }
    return -1;
}
