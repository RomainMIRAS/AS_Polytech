#include <../include/sys/sem.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
PUBLIC int semget(unsigned key)
{
    for (int i = 0; i < SEM_MAX; i++)
    {
        if (semTab[i].state == SEM_INVALID_ID)
        {
            semTab[i].state = SEM_CREATED;
            semTab[i].key = key;
            semTab[i].chain = NULL;
            semTab[i].value = 1;
            return i;
        }
    }

    return -1;
}
