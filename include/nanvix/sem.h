/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NANVIX_SYS_SEM_H_
#define NANVIX_SYS_SEM_H_

#ifndef _ASM_FILE_

#include <nanvix/config.h>


	#define SEM_CREATED 1
	#define SEM_INVALID_ID 2

    struct semaphore
    {
        unsigned state;
        unsigned key;
        struct process ** chain;
        int value;
    };

	extern struct semaphore semTab[SEM_MAX];
    extern void sem_init(void);

#endif /* _ASM_FILE_ */

#endif /* NANVIX_SYS_SEM_H_ */
