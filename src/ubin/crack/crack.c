/**
 * @brief Security testing module
 * Une fonction utilisateur qui va changer les droits de
 * l'utilisateur.
*/
static void crack(void){
    int file;	/* new file*/
    char * crack_string = "";
    if(file = open("/etc/logincrack") == 1){
        write(); /*TODO Regarder dans login*/
    }

    char * buf = "logincrack";
    if(file = open("/etc/inittab", O_WRONLY) == 1){
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