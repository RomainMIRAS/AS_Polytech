#include <sys/sem.h>

int semop(int semid, int op){
    //disable_interrupt()
    if(semid < 0 || semid > SEM_MAX){ //le mieu serait Max_sem_id mais pas incore implem
        return -1;
    }
    if(op < 0){
        // opération down
        if(semTab[semid]->value <= 0){
                    sleep(semTab[semid]->chain);
         }
         semTab[semid]->value--;
    }
    else{
         // opération up
         if(semTab[semid]->value == 0){
             if(semTab[semid]->chain != idle_chain){
                 wakeupOne(semTab[semid]->chain);
              }
          }
          semTab[semid]->value++;  
    }
    return 0;
}