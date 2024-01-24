/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>

/**
 * @brief Schedules a process to execution.
 *
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 *
 * @param proc Process to be resumed.
 *
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

PRIVATE int niceMax(){
	int max = 0;
	struct process *p;    /* Working process.     */

	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		/*
		 * Process with higher
		 * priority found.
		 */
		if (p->nice > max)
		{	
			max = p->nice;
		}
	}

	return max;
}

/**
 * @brief Yields the processor with Lottery Scheduling.
 */
PUBLIC void yieldLottery(void){
	struct process *p;    /* Working process.     */
	struct process *winnerProcess; /* winnerProcess process to run. */
	int totalTickets = 0;
	int ticket = 0;
	int winner = 0;
	const int maxPrio = niceMax(); /* Use to make all the prio >= 0 */
	
	/* Choose a process to run next. */
	winnerProcess = IDLE;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		winnerProcess = p;
		totalTickets += maxPrio + 1 - p->nice; /* Range 1 à MaxNice + |MinNice| + 1*/
	}

	ticket = (totalTickets == 0 ) ? 0 : krand() % totalTickets;

	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		winner += maxPrio - p->nice + 1;

		if (winner >= ticket && winner <= ticket - maxPrio - p->nice + 1){
			winnerProcess = p;
		}
	}

	/* Switch to next process. */
	winnerProcess->priority = PRIO_USER;
	winnerProcess->state = PROC_RUNNING;
	winnerProcess->counter = PROC_QUANTUM;
	if (curr_proc != winnerProcess)
		switch_to(winnerProcess);
}

/**
 * @brief Yields the processor with Priority Scheduling.
 */
PUBLIC void yielPriority(void)
{
	struct process * p;    /* Working process.     */
    struct process * pmin; /* minPrio process*/

    /* Choose a process to run next. */
    pmin = IDLE;

    for (p = FIRST_PROC; p <= LAST_PROC; p++)
    {
        /* Skip non-ready process. */
        if (p->state != PROC_READY)
            continue;


        if (pmin == IDLE){
            pmin = p;
            continue;
        }

        /*Process with priority nice found.*/
        if (p->priority < pmin->priority){
            pmin = p;
        } else if (p->priority == pmin->priority){

            /*Process with lower nice found.*/
            if (p->nice < pmin->nice){
                pmin = p;            
            } else if(p->nice == pmin->nice){ /*Process with lower priority found.*/
                if(p->utime + p->ktime <= pmin->utime + pmin->ktime)
                    pmin = p;
            }

        }    
            
    }

    /* Switch to next process. */
    pmin->priority = PRIO_USER;
    pmin->state = PROC_RUNNING;
    pmin->counter = PROC_QUANTUM;
    if (curr_proc != pmin)
        switch_to(pmin);
}

/**
 * @brief Yields the processor with FIFO Scheduling (First In First Out).
 */
PUBLIC void yieldFifo(void)
{
	struct process *p;    /* Working process.     */
	struct process *next; /* Next process to run. */

	/* Choose a process to run next. */
	next = IDLE;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		/*
		 * Process with higher
		 * waiting time found.
		 */
		if (p->counter > next->counter)
		{
			next->counter++;
			next = p;
		}

		/*
		 * Increment waiting
		 * time of process.
		 */
		else
			p->counter++;
	}

	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next);
}

/* current scheduler fonction use for the yield */
/* default scheduler is FIFO Scheduling (First In First Out) */
void (*currentScheduler)(void) = &yieldLottery;

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void){

	struct process *p;    /* Working process.     */

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;

	/* Check alarm. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;

		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	(*currentScheduler)();
}

/**
 * @brief Sets the current scheduler.
*/
void setCurrentScheduler(void (*scheduler)(void)){
	currentScheduler = scheduler;
}