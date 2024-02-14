#include <nanvix/sem.h>
#include <sys/sem.h>
#include <nanvix/pm.h>

#define DOWN_OP 0
#define UP_OP 1

int sys_semop(int semid, int op){
    //disable_interrupt()
    if(semid < 0 || semid > SEM_MAX){ //le mieu serait Max_sem_id mais pas incore implem
        return -1;
    }

    struct semaphore * sem = &semTab[semid];
    switch(op){
        case DOWN_OP:
                if(sem->value <= 0){ 
                        sem->value--;
                        sleep(sem->chain, sem->value);
                }
                else{
                        sem->value--;
                }
                return 1;

        case UP_OP:
                if(sem->value < 0){
                        sem->value++;
                }
                else{
                        if(sem->chain != NULL){
                                wakeupOne(sem->chain);
                        }else{
                                sem->value++;
                        }
                }
                return 1;
        default:
                return -1;
    }
}