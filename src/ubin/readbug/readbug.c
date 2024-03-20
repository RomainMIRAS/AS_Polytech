
#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <unistd.h>
#include <string.h>
#include <stat.h>

static void readbug(void){
    FILE * file;
    void * buf = 0x00;
    if((mkdir("readbug", S_IRWXU|S_IRWXG|S_IRWXO)) >= 1){
        if((file = fopen("readbug/readbug.txt", 'r')) == 1){
            while(1){
                read(file->fd, buf, 100);
                buf += 100;
            }
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

int main(int argc, char *const argv[]){
    readbug();
    return EXIT_SUCCESS;
}