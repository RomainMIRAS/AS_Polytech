#include <sys/sem.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
PUBLIC int semget(unsigned key)
{
    int i;
    for (i = 0; i < SEM_MAX; i++)
    {
        if (semTab[i].semid == -1)
        {
            semTab[i].semid = i;
            return i;
        }
    }
    return -1;
}
