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
    int fifoFD  = initialize_fifo();
    char* next_request;
    next_request = wait_user_input(fifoFD);
    printf("%s\n", next_request);
    return 0;
}

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n", CLIENT_HEADER, CLIENT_VERSION);
    printf("%s Available commands are 'proc {PID}'' 'useru {UID}' 'usern {UserName}' and 'exit'\n", CLIENT_HEADER);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
}

int initialize_fifo(){
    int fifo_fd = open(FIFO_RQST_NAME, 0666);
    switch (fifo_fd) {
        case -1:
          fprintf(stderr, "%s Fifo open failed. Initialisation aborted.\n", CLIENT_HEADER);
          perror("Unknown FIFO");
          exit(EXIT_FAILURE);
        default:
          return fifo_fd;
    }
}

char* wait_user_input(int fifo_fd){
    unsigned int usr_or_proc_id;
    char* user_name = malloc(256); //256 is the max size of a username...

    //We have to find a solution to reduce this buffer cause' it is vulnerable 
    //to buffer overflow...

    //Init of the "char[]" equivalent of the FIFO fd.
    char buf_fifo_fd[256];
    sprintf(buf_fifo_fd, "%d", fifo_fd);

    for (;;) {
        char cmd[CMD_SIZE];
        printf("Command ?> ");
        scanf("%s",cmd);
        if ((strcmp(to_lower(cmd),"exit"))==0) {
            close_client(fifo_fd);
        }
        else if ((strcmp(to_lower(cmd),"proc"))==0) {
            usr_or_proc_id=0;
            concat(cmd, ",");
            printf("PID ?>");
            scanf("%u",&usr_or_proc_id);
            if (usr_or_proc_id>PID_MAX) {
                printf("Invalid PID value.\n");
                continue;
            }
            char str[6];
            sprintf(str, "%d", usr_or_proc_id);
            concat(cmd,str);
            concat(cmd,",");
            //NUL
            concat(cmd,buf_fifo_fd);
            concat(cmd,";");
            return cmd;

        } else if ((strcmp(to_lower(cmd),"useru"))==0) {
            concat(cmd, ",");
            char str[6];
            printf("UID ?>");
            scanf("%u",&usr_or_proc_id);
            sprintf(str, "%d", usr_or_proc_id);
            concat(cmd,str);
            concat(cmd,",");
            //NUL
            concat(cmd,buf_fifo_fd);
            concat(cmd,";");
            return cmd;

        } else if ((strcmp(to_lower(cmd),"usern"))==0) {
            concat(cmd, ",");
            printf("Username ?>");
            scanf("%s",user_name);
            if ((strpbrk(user_name,"/\\")) != NULL){
                printf("Don't try to buff overflow my prgm pls :'(\n");
                continue;
            }
            concat(cmd,user_name);
            concat(cmd,",");
            //NUL
            concat(cmd,buf_fifo_fd);
            concat(cmd,";");
            return cmd;
        }
        else {
            printf("Unknown command !\n");
            continue;
        }
    } // for(;;)
}

int str_to_request(request *req, char* str) {
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

void close_client(int fifo_fd){
    int unlinkstate = unlink(fifo_fd);
    if (unlinkstate==-1) {
        fprintf(stderr, "%s Fifo destruction failed. please look at /tmp/ to delete the unlinkable fifo\n", CLIENT_HEADER);
        exit(EXIT_FAILURE);
    }
    printf("Goodbye !\n");
    exit(EXIT_SUCCESS);
}
