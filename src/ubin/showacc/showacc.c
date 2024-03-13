#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nanvix/accounts.h>


/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/**
 * @brief Security testing module
 * Une fonction utilisateur qui montre tout les comptes du systeme.
 * Elle s'apelle avec : "showacc" sans arguments.
*/
static void showacc(void){
    int file;         /* Passwords file.  */
	struct account a; /* Working account. */

	/* Open passwords file. */
	if ((file = open("/etc/passwords", O_RDONLY)) == -1)
	{
		fprintf(stderr, "cannot open password file\n");
	} else {
		/* Show all the passwords file. */
		while (read(file, &a, sizeof(struct account)))
		{
			account_decrypt(a.name, USERNAME_MAX, KERNEL_HASH);
			account_decrypt(a.password, PASSWORD_MAX, KERNEL_HASH);
			printf("name: %s\n", a.name);
			printf("password: %s\n", a.password);
			printf("uid: %d\n", a.uid);
			printf("gid: %d\n", a.gid);
		}
	}
}

/*
 * Prints program version and exits.
 */
static void version(void)
{
	printf("ls (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright(C) 2011-2014 Pedro H. Penna\n");
	printf("This is free software under the ");
	printf("GNU General Public License Version 3.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n\n");

	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: ls [options] [directory]\n\n");
	printf("Brief: Lists contents of a directory.\n\n");
	printf("Options:\n");
	printf("  -a, --all     List all entries\n");
	printf("  -i, --inode   Print the inode number of each file\n");
	printf("      --help    Display this information and exit\n");
	printf("      --version Display program version and exit\n");

	exit(EXIT_SUCCESS);
}

/*
 * Gets program arguments.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Loop index.       */
	char *arg; /* Working argument. */

	/* Get program arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		/* Display help information. */
		if (!strcmp(arg, "--help"))
			usage();

		/* Display program version. */
		else if (!strcmp(arg, "--version"))
			version();


	}
}

/*
 * Launches program.
 */
int main(int argc, char *const argv[])
{
    getargs(argc, argv);

    showacc();

    return (EXIT_SUCCESS);

}