#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "globals_daemons_consts.h"
#include "server_consts.h"
#include "server.h"

int main(void){
    greet_user();
    int fifoFD  = initialize_fifo();
    wait_for_next_question(fifoFD);
    closeServer(fifoFD);
}

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_HEADER, SERVER_VERSION);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", SERVER_HEADER);
}

int initialize_fifo(){
    int fifo_fd = mkfifo(FIFO_RQST_NAME, 0666);
    switch(fifo_fd){
        case -1:
          fprintf(stderr, "%s Fifo creation failed. Initialisation aborted.\n", SERVER_HEADER);
          perror("Unknown SHM");
          exit(EXIT_FAILURE);
        default:
          return fifo_fd;
    }
}

int wait_for_next_question(int fifoFD){
    char buf[512];
    printf("%s Waiting for a client question\n", SERVER_HEADER);
    while(-1==read(fifoFD, buf, 512));
    return 0; // temporaire je met ça juste pour que ça compile...
}

int str_to_request(struct Request* req, char* str) {
    //First we need to tokenize the str.
    //Then, we need to remove the ';' from the cmd_param
    //Of course, to validate that it is a good request, 
    //first we check that the last char of cmd_param is ';'
    char* token;
    if(str[(int)(strlen(str)-1)]!=';'){
        return -1;
    } else {
        str[(int)(strlen(str)-1)] = '\0';
    }
    int cpt=1;
    while( (token = strsep(&str, REQUEST_SEPARATOR))!=NULL ){
        switch(cpt){
            case 1:
              req->shm_linked = token;
              break;
            case 2:
              req->cmd_name   = token;
              break;
            case 3:
              req->cmd_param = token;
              break;
            default:
              return -1;
        }
        ++cpt;
    }
    return 0;
}


void closeServer(int fifo_fd) {
    if(fifo_fd==(int)NULL)
        return;
    if(close(fifo_fd)==-1){
        perror("close");
        exit(EXIT_FAILURE);
    }
}
