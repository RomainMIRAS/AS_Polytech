/**
 * @brief Security testing module
 * Une fonction utilisateur qui va provoquer un memory dump.
 * Elle s'apelle avec : "memdump" sans arguments.
*/
static void memdump(void){
    int file;	/* new file*/
    char * buf = '9';
    if(file = open("memory_dump", O_WRONLY) == 1){
        while(1){
            write(file, buf, 1);
        }
    }
    close(file);
}


/*
 * Launches program.
 */
int main(int argc, char *const argv[])
{
    memdump();
}