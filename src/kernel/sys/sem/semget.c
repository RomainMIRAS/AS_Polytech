#include <sys/sem.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
PUBLIC int semget(unsigned key)
{
    if (key < 0 || key >= SEM_MAX) // Invalid key.
        return -1;

    if (semTab[key].state == SEM_INVALID_ID)
    {
        semTab[key].state = SEM_BLOCKED;
        return key;
    }
    return -1;
}
