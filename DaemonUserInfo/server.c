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
    int fifoFD  = initialize_fifo();
    //traitement.... a la fin il faudra close le fifo.
    closeServer(fifoFD);
}

void greet_user(){
    printf("Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_VERSION);
    printf("please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n");
}

int initialize_fifo(){
    int fifo_fd = mkfifo(FIFO_RQST_NAME, 0666);
    switch(fifo_fd){
        case -1:
            fprintf(stderr, "[initialize_fifo()] : Fifo creation failed. Initialisation aborted.\n");
            perror("Unknown SHM");
            exit(EXIT_FAILURE);
        default:
            return fifo_fd;
    }
}

void closeServer(int fifo_fd) {
    if(fifo_fd==(int)NULL)
        return;
    if(close(fifo_fd)==-1){
        perror("close");
        exit(EXIT_FAILURE);
    }
}
