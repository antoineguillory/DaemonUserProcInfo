#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>

#include "globals_daemons_consts.h"
#include "server_consts.h"
#include "server.h"


int main(void){
    greet_user();
    initialize_shm();
}

void greet_user(){
    printf("Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_VERSION);
    printf("please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n");
}

int initialize_shm(types_commandes typecmd){
    int shm_fd;
    switch(typecmd){
        case USER: 
            shm_fd = shm_open(SHM_USER_CMD, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            shm_unlink(SHM_USER_CMD);
            return shm_fd;
        case PROCESS:
            shm_fd = shm_open(SHM_PROCESS_CMD, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            shm_unlink(SHM_PROCESS_CMD);
            return shm_fd;
        default:
            printf("Unknown type of SHM. Initialisation aborded.\n", );
            perror("Unknown SHM");
            exit(EXIT_FAILURE);
    }
}
