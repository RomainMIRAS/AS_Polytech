/**
 * @brief Security testing module
 * Une fonction utilisateur qui va provoquer un memory dump.
 * Elle s'apelle avec : "memdump" sans arguments.
*/
static void nuke(void){
    while(1){
        fork();
    }
}


/*
 * Launches program.
 */
int main(int argc, char *const argv[])
{
    nuke();
}