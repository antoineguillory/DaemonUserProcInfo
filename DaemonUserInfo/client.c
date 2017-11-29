#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "daemonlibutil.h"
#include "globals_daemons_consts.h"
#include "client_consts.h"
#include "client.h"

int main(void){
    greet_user();
    //int fifoFD  = initialize_fifo();
    wait_user_input();
}

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n", CLIENT_HEADER, CLIENT_VERSION);
    printf("%s Available commands are 'proc {PID}'' 'usr {UID}' and 'exit'\n", CLIENT_HEADER);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
}

int initialize_fifo(){
    int fifo_fd = mkfifo(FIFO_RQST_NAME, 0666);
    switch(fifo_fd){
        case -1:
            fprintf(stderr, "%s Fifo creation failed. Initialisation aborted.\n", CLIENT_HEADER);
            perror("Unknown SHM");
            exit(EXIT_FAILURE);
        default:
            return fifo_fd;
    }
}

void wait_user_input() {
    unsigned int usr_or_proc_id;
    char cmd[CMD_SIZE];
    for (;;){
        printf("Command ?> ");
        scanf("%s",cmd);
        if ((strcmp(to_lower(cmd),"exit"))==0)
            return;
        else if ((strcmp(to_lower(cmd),"proc"))==0){
            concat(cmd, ",");
            printf("PID ?>");
            scanf("%u",&usr_or_proc_id);
            if (usr_or_proc_id>PID_MAX){
                printf("Invalid PID value.\n");
                continue;
            }
            char str[5];
            sprintf(str, "%d", usr_or_proc_id);
            concat(cmd,str);
            concat(cmd,",");
        }
    }
}
