#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>
#include <pthread.h>

#include "globals_daemons_consts.h"
#include "server_consts.h"
#include "server.h"


int main(void){
    greet_user();

    enum types_commandes usr, proc;
    usr  = USER;
    proc = PROCESS;
    int shm_usr = initialize_shm(usr);
    if(shm_usr==-1){
        fprintf(stderr, "[main()] : Initialisation of user SHM failed. Initialisation aborted.\n");
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    int shm_proc = initialize_shm(proc);
    if(shm_proc==-1){
        fprintf(stderr, "[main()] : Initialisation of process SHM failed. Initialisation aborted.\n");
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
}

void greet_user(){
    printf("Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_VERSION);
    printf("please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n");
}

int initialize_shm(enum types_commandes typecmd){
    int shm_fd;
    switch(typecmd){
        case USER: 
            shm_fd = shm_open(SHM_USR_CMD, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            shm_unlink(SHM_USR_CMD);

            return shm_fd;
        case PROCESS:
            shm_fd = shm_open(SHM_PROCESS_CMD, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            shm_unlink(SHM_PROCESS_CMD);
            return shm_fd;
        default:
            fprintf(stderr, "[initialize_shm()] : Unknown type of SHM. Initialisation aborted.\n");
            perror("Unknown SHM");
            exit(EXIT_FAILURE);
    }
}
