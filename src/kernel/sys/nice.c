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

#include <nanvix/const.h>
#include <nanvix/pm.h>
#include <errno.h>
#include <limits.h>

/*
 * Changes the nice value of the calling process.
 */
PUBLIC int sys_nice(int incr)
{
	/* Not authorized. */
	if ((incr < 0) && !IS_SUPERUSER(curr_proc))
		return (-EPERM);

	curr_proc->nice += incr;

	/* Do not exceed boundaries. */
	if (curr_proc->nice < 0)
		curr_proc->nice = 0;
	else if (curr_proc->nice >= 2*NZERO)
		curr_proc->nice = 2*NZERO - 1;

	/* Match the process with the correct scheduling class*/
	/*
	if(curr_proc->nice < -20){
		curr_proc->scheduling_class = 1;
	}
	else if((curr_proc->nice >= -20) && (curr_proc->nice < 0)) //Class 2
	{
		curr_proc->scheduling_class = 2;
	}
	else if((curr_proc->nice >= 0) && (curr_proc->nice < 20)){ //Class 3
		curr_proc->scheduling_class = 3;
	}
	else{	//Class 4
		curr_proc->scheduling_class = 4;
	}*/

	return (curr_proc->nice);
}

