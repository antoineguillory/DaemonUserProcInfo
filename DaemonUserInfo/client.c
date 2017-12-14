
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#include "client.h"

#include "util.h"
#include "global_server.h"


int fifo_fd;
char* shm_name;

int main(void){
    greet_user();
    manage_client_signals();
    fifo_fd  = open_fifo(FIFO_SERVER_NAME);
    shm_name = concat(SHM_NAME, rdmnb_to_str(NB_NUMBER_FOR_NAME_SHM));
    char *shm = initialize_shm();
    if (shm == NULL) {
        handle_error_main();
    }
    while (1) {
        request *r = extract_request();
        if (r == NULL) {
            break;
        }
        //printf("%s,%s,%s;\n", r->shm_linked, r->cmd_name, r->cmd_param);
        if (send_request(r) == -1 ) {
            free(r);
            handle_error_main();
        }
        free(r);
        if (wait_reponse() == -1) {
            handle_error_main();
        }
        printf("%s\n", shm);
    }
    printf("%s See u later !\n", CLIENT_HEADER);
    close_client();
    return EXIT_SUCCESS;
}

void handle_error_main() {
    fprintf(stderr, "%s Error in the matrix.\n", CLIENT_HEADER);
    close_client();
    exit(EXIT_FAILURE);
}

void print_help() {
    printf("%s Available commands are '%s {PID}'' '%s {UID}''%s {UserName}' and '%s'\n", CLIENT_HEADER, CMD_PROC, CMD_USER_UID, CMD_USER_NAME, CMD_EXIT);
}

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n",
      CLIENT_HEADER, CLIENT_VERSION);
    print_help();
    printf("%s Please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
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

void close_fifo(void) {
    if (close(fifo_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
}

char *initialize_shm(void) {
    int fd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
        return NULL;
    }
    if (ftruncate(fd, SIZE_FIXE_SHM) == -1) {
        perror("ftruncate");
        return NULL;
    }
    char *mem = mmap(NULL, SIZE_FIXE_SHM,
    PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return mem;
}

request *extract_request(void) {
    char cmd[CMD_SIZE];
    char param[256];
    printf("Command ?> ");
    scanf("%s",cmd);
    if ((strcmp(to_lower(cmd), CMD_EXIT)) == 0) {
        return NULL;
    }
    else if ((strcmp(to_lower(cmd),CMD_PROC))==0) {
        uid_t id = 0;
        printf("PID ?>");
        scanf("%u",&id);
        if (id > PID_UID_MAX) {
            printf("Invalid PID value.\n");
        }
        sprintf(param, "%u", id);
    } else if ((strcmp(to_lower(cmd), CMD_USER_UID)) == 0) {
        uid_t id = 0;
        printf("UID ?>");
        scanf("%u",&id);
        if (id > PID_UID_MAX) {
            printf("Invalid PID value.\n");
        }
        printf("%s : ", param);
        sprintf(param, "%u", id);
        printf("\n%u : %s\n", id, param);

    } else if ((strcmp(to_lower(cmd), CMD_USER_NAME)) == 0) {
        printf("Username ?>");
        scanf("%s", param);
        if ((strpbrk(param,"/\\")) != NULL){
            printf("Don't try to buffer overflow my prgm pls :'(\n");
        }
    }
    else {
        printf("Unknown command !\n");
        return extract_request();
    }

    request *result = malloc(sizeof(*result));
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    result->client_pid = getpid();
    result->shm_linked = shm_name;
    result->cmd_name = cmd;
    result->cmd_param = param;
    return result;
}

int send_request(request *r) {
    sem_t *sem = sem_open(SEM_RQST_NAME, O_WRONLY);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }
    if (write(fifo_fd, r, sizeof(*r)) == -1) {
        perror("write");
        return -1;
    }
    if (sem_post(sem) == -1) {
        perror("sem_post");
        return -1;
    }
    return 0;
}

void manage_client_signals(void){
    struct sigaction sigusr1act;
    sigusr1act.sa_handler = handle_sigclient;
    if (sigaction(SIGUSR1, &sigusr1act, NULL) < 0) {
        printf("%s Cannot manage SIGUSR1\n",CLIENT_HEADER);
        close_client();
    }
}

static void handle_sigclient(int signum) {
    if(signum==SIGUSR1){
        //TODO Meilleure gestion...
        printf("%s Internal server error, client exited.\n", CLIENT_HEADER);
        close_client();
    }
}

int wait_reponse(void) {
    sem_t *sem = sem_open(SEM_REPONSE_NAME, O_RDONLY);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }
    if (sem_wait(sem) == -1) {
        perror("sem_wait");
        return -1;
    }
    return 0;
}

void close_client(void){
    free(shm_name);
    close_fifo();
}
