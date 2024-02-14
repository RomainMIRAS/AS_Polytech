#include <nanvix/sem.h>
#include <nanvix/config.h>
#include <nanvix/const.h>

/**
 * @brief Semaphore table.
 */
PUBLIC struct semaphore semTab[SEM_MAX];

/**
 * @brief Initializes the semaphore system.
 */
PUBLIC void sem_init(void){
	int i;             /* Loop index.      */
	/* Initializes the semaphore table. */
	for (i = 0; i < SEM_MAX; i++)
	{
		semTab[i].state = SEM_INVALID_ID;
	}
}