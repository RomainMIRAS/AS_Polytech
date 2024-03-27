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

#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <unistd.h>
#include <string.h>

#include <sys/utsname.h>

#include <nanvix/accounts.h>
#include <nanvix/config.h>
#include <dev/tty.h>

#if (MULTIUSER == 1)

#define BRUTE_FORCE_ON 1

/**
 * @brief Authenticates a user in the system.
 *
 * @param name     User name.
 * @param password User's password.
 *
 * @returns One if the user has authentication and zero otherwise.
 */
static int authenticate(const char *name, const char *password)
{
	int ret;          /* Return value.    */
	int file;         /* Passwords file.  */
	struct account a; /* Working account. */

	ret = 1;

	/* Open passwords file. */
	if ((file = open("/etc/passwords", O_RDONLY)) == -1)
	{
		fprintf(stderr, "cannot open password file\n");
		return (0);
	}

	/* Search in the  passwords file. */
	while (read(file, &a, sizeof(struct account)))
	{
		account_decrypt(a.name, USERNAME_MAX, KERNEL_HASH);

		/* No this user. */
		if (strcmp(name, a.name))
			continue;

		account_decrypt(a.password, PASSWORD_MAX, KERNEL_HASH);

		/* Found. */
		if (!strcmp(password, a.password))
		{
			setgid(a.gid);
			setuid(a.uid);
			goto found;
		}
	}

	ret = 0;
	if (!BRUTE_FORCE_ON)
		fprintf(stderr, "\nwrong login or password\n\n");
	

found:

	/* House keeping. */
	close(file);

	return (ret);
}

/**
 * @brief Attempts to login.
 *
 * @returns One if successful login and false otherwise.
 */
static int login(void)
{
	char name[USERNAME_MAX];
	char password[PASSWORD_MAX];

	printf("login: ");
	fgets(name, USERNAME_MAX, stdin);
	printf("password: ");
	fgets(password, PASSWORD_MAX, stdin);

	return (authenticate(name, password));
}

#endif

/**
 * @brief increment one letter of the string passed as parameter
 * ensures every string can be a possible password/login (takes only miniscules and number)
 *
 * @returns 1 if all the possibilities for a string of length caracter have been tested
 * 			0 otherwise
 */
int inc_letter(char * string, int * offset, int max_size)
{

	for(int k = *offset; k > 0; k--){						
		if(string[k] == 123)	{							// all possibilities from k to length have been tested
			string[k-1]++;									// trying with a different k-1 letter
			string[k] = 48;									// reinitialize k letter to minimal caracter possible
		}								
			
	}

	for (int k = 0; k < *offset + 1; k++)					
	{
		if(string[k] == 58)									// incrementation based on ascii table, a gap exists between number and minuscules
			string[k] = 97;									// if last number reached skip to first minuscule
	}

	if(string[0] >= 123){									// all possibilities for offset characters have been tested
		(*offset)++;											//then add one letter 
		for (int i = 0; i < *offset+ 1; i++)
			{
				string[i] = 48;								// set 0 to all caracter
			}
	}

	if(*offset >= max_size){									//all possibilities for max_size character have been tested 
		
		*offset = 0;										// reinitilaize variables to start all over again
		for (int i = 0; i < max_size; i++)
		{
			string[i] = '0';
		}
		return 1;											
	}

	string[*offset]++;										// inc last letter for next call

	return 0;

}

/*
 * Logins a user.
 */
int main(int argc, char *const argv[])
{
	char *arg[2];        /* Shell arguments.    */
	struct utsname name; /* System information. */

	((void)argc);
	((void)argv);

	arg[0] = "-";
	arg[1] = NULL;

	(void) setvbuf(stdin, NULL, _IOLBF, 0);
	(void) setvbuf(stdout, NULL, _IONBF, 0);

	ioctl(fileno(stdout), TTY_CLEAR);

	/* Get system information. */
	if (uname(&name) != 0)
	{
		fprintf(stderr, "cannot uname()");
		return (EXIT_FAILURE);
	}

	printf("%s %s on %s\n\n", name.sysname, name.version, name.nodename);

#if (MULTIUSER == 1)

	if (!BRUTE_FORCE_ON){
		// Normal way to login
		while (!login());
	}else{
		//brute force by testing all possible passwords
		printf("Forcing password .... \n");

		char login[USERNAME_MAX];				//Initialize login variables
		int login_offset = 0;
		login[login_offset] = 48;

		char password[PASSWORD_MAX];			// iniltialize password variables
		int password_offset = 0;
		password[password_offset] = 48;

		int finished = 0;						

		// test all passwords for one login
		do{
			 do
			 {
				// printf("login: %s\n", login);
				// printf("password: %s\n", password);
				finished = authenticate(login, password);
			 } while (!inc_letter(password, &password_offset, PASSWORD_MAX) && !finished); 
		 }while(!inc_letter(login, &login_offset, USERNAME_MAX) && !finished);

		 if (!finished)
		 {
			fprintf(stderr, "\nwrong login or password\n\n");
		 }
		
	}
	

#endif

	ioctl(fileno(stdout), TTY_CLEAR);

	printf("Nanvix - A Free Educational Operating System\n\n");
	printf("The programs included with Nanvix system are free software\n");
	printf("under the GNU General Public License Version 3.\n\n");
	printf("Nanvix comes with ABSOLUTELY NO WARRANTY, to the extent\n");
	printf("permitted by applicable law.\n\n");

	execve("/bin/tsh", (char *const*)arg, (char *const*)environ);

	return (EXIT_SUCCESS);
}
