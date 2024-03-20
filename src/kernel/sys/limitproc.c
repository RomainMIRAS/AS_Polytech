#include <nanvix/limitsproc.h>

PUBLIC struct limitsproc limitsUserProcess[MAX_USER_ID];

/**
 * @brief Initializes the user process limits.
 */
PUBLIC void initLimitsUserProcess()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        limitsUserProcess[i].numProcesses = 0;
        limitsUserProcess[i].limit = MAX_PROCESSES;
    }
}

/**
 * @brief Sets the limit of processes that a user can create.
 *
 * @param uid   User ID.
 * @param limit Limit.
 */
PUBLIC void setLimitsUserProcess(uid_t uid, int limit)
{  
    limitsUserProcess[uid].limit = limit;
}

/**
 * @brief Gets the limit of processes that a user can create.
 *
 * @param uid User ID.
 *
 * @returns The limit of processes that a user can create.
 */
PUBLIC int getLimitsUserProcess(uid_t uid)
{
    return limitsUserProcess[uid].limit;
}

/**
 * @brief Checks if a user can create a process.
 *
 * @param uid User ID.
 *
 * @returns Non zero if the user can create a process, and zero otherwise.
 */
PUBLIC int canCreateProcess(uid_t uid)
{
    if (!PROTECTION_ON) return TRUE;
    return (limitsUserProcess[uid].numProcesses < limitsUserProcess[uid].limit) ? TRUE : FALSE;
}

/**
 * @brief Increments the number of processes created by a user.
 *
 * @param uid User ID.
 */
PUBLIC void incrementNumProcesses(uid_t uid)
{
    if (!PROTECTION_ON) return;
    limitsUserProcess[uid].numProcesses++;
}

/**
 * @brief Decrements the number of processes created by a user.
 *
 * @param uid User ID.
 */
PUBLIC void decrementNumProcesses(uid_t uid)
{
    if (!PROTECTION_ON) return;
    limitsUserProcess[uid].numProcesses--;
}




