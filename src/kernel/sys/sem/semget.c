#include <sys/sem.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
PUBLIC int semget(unsigned key)
{
    if (key < 0 || key >= SEM_MAX)
        return -1;

    if (semTab[key].semid == -1)
    {
        semTab[key].semid = key;
        return key;
    }
    return -1;
}
