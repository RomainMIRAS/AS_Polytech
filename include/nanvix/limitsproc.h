
#ifndef NANVIX_LIMITSPROC_H_
#define NANVIX_LIMITSPROC_H_

    #include <nanvix/const.h>
    #include <nanvix/pm.h>
    
    #define MAX_PROCESSES 10
    #define MAX_USER_ID 10

    // Protection flag to enable or disable the limits
    #define PROTECTION_ON 0

    #ifndef _ASM_FILE_
        struct limitsproc
        {
            int numProcesses;
            int limit;
        };
        

        EXTERN struct limitsproc limitsUserProcess[10];
    #endif

    PUBLIC void setLimitsUserProcess(uid_t uid, int limit);

    PUBLIC int getLimitsUserProcess(uid_t uid);

    PUBLIC int canCreateProcess(uid_t uid);

    PUBLIC void incrementNumProcesses(uid_t uid);

    PUBLIC void decrementNumProcesses(uid_t uid);

    PUBLIC void initLimitsUserProcess();

#endif /* NANVIX_LIMITSPROC_H_ */
