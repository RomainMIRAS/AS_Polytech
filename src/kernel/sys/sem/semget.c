#include <nanvix/sem.h>
#include <sys/sem.h>
#include <nanvix/klib.h>
/**
 * @brief System call for getting a semaphore.
 * @return ID of the semaphore.
*/
int sys_semget(unsigned key)
{
    int first_invalid_id = -1;
    for (int i = 0; i < SEM_MAX; i++)
    {
        if(semTab[i].key == key){
            return i;
        }

        if(first_invalid_id == -1){
            if (semTab[i].state == SEM_INVALID_ID)
            {
                first_invalid_id = i;
            }   
        }
    }

    if(first_invalid_id == -1){
        return -1;
    }

    kprintf("[semget] Creating new semaphore with key %d\n", key);
    kprintf("[semget] First invalid id: %d\n", first_invalid_id);
    semTab[first_invalid_id].state = SEM_CREATED;
    semTab[first_invalid_id].key = key;
    semTab[first_invalid_id].chain = 0;
    semTab[first_invalid_id].value = 1;
    return first_invalid_id;
}
