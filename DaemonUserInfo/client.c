#include "client.h"

#define SIZE_FIXE 2048
#define SHM_NAME  "/shm_"
#define NB_NUMBER_FOR_NAME_SHM 10

int main(void){
    greet_user();
    int fifoFD = open_fifo(FIFO_RQST_NAME);
    char *shm_name = concat(SHM_NAME, rdmnb_to_str(NB_NUMBER_FOR_NAME_SHM));
    char *shm = initialize_shm(shm_name);
    if (shm == NULL) {
      return EXIT_FAILURE;
    }
    while (1) {
      request *r = extract_request();
      send_request(fifoFD, r, shm);
      printf("%s\n", shm);
    }
    return EXIT_SUCCESS;
}

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n", CLIENT_HEADER, CLIENT_VERSION);
    printf("%s Available commands are 'proc {PID}'' 'useru {UID}' 'usern {UserName}' and 'exit'\n", CLIENT_HEADER);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
}

int open_fifo(char *fifo_name){
    int fifo_fd = open(fifo_name, 0666);
    if (fifo_fd == -1) {
        fprintf(stderr, "%s Fifo open failed.\n", CLIENT_HEADER);
        perror("Unknown FIFO");
        exit(EXIT_FAILURE);
    }
    return fifo_fd;
}

char *initialize_shm(char *shm_name) {
  int fd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("shm_open");
    return NULL;
  }
  if (shm_unlink(shm_name) == -1) {
    perror("shm_unlink");
    return NULL;
  }
  if (ftruncate(fd, SIZE_SHM) == -1) {
    perror("ftruncate");
    return NULL;
  }
  char *mem = mmap(NULL, SIZE_SHM,
    PROT_WRITE | PROT_READ, MAP_SHARED, fd, NULL);
  if (mem == MAP_FAILED) {
    perror("mmap");
    return NULL;
  }
  return mem;
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
