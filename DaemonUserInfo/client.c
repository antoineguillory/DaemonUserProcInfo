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

#include "global_server.h"
#include "util.h"

#define PID_UID_MAX 32768

/* GLOBALS, nous avons besoin de ses états dans
 * l'appel des signaux.
 */
int fifo_fd;
char *shm_name;
char *sem_name;

int main(void) {
    manage_client_signals();

    char *id = rdmnb_to_str(NB_NUMBER_OF_SHM_SEM_NAME);
    init_client(id);
    greet_user(id);
    free(id);
    
    char *shm = project_new_shm(shm_name, SIZE_FIXE_SHM);
    sem_t *sem = init_sem(sem_name, 0);

    while (1) {
        request *r = extract_request();
        if (r == NULL) {
            break;
        }
        if (write(fifo_fd, r, sizeof(*r)) == -1) {
            perror("write");
            close_client();
            exit(EXIT_FAILURE);
        }
        free(r);
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            close_client();
            exit(EXIT_FAILURE);
        }
        printf("%s %s\n%s", CLIENT_HEADER, r->cmd_param, shm);
    }
    close_client();
    printf("%s See u later !\n", CLIENT_HEADER);
    return EXIT_SUCCESS;
}

void init_client(char *id) {
    fifo_fd = open(FIFO_SERVER_NAME, O_WRONLY | O_APPEND);
    if (fifo_fd == -1) {
        fprintf(stderr, "Server doesn't exist or closed.\n");
        exit(EXIT_FAILURE);
    }
    shm_name = concat(SHM_NAME, id);
    sem_name = concat(SEM_NAME, id);
}

request *extract_request() {
    char cmd[SIZE_CMD];
    char param[SIZE_PARAM];
    printf("Command ?> ");
    scanf("%s",cmd);
    if ((strcmp(to_lower(cmd), CMD_EXIT)) == 0) {
        return NULL;
    }
    else if ((strcmp(to_lower(cmd), CMD_PROC))==0) {
        uid_t id = 0;
        printf("PID ?>");
        scanf("%u",&id);
        if (id > PID_UID_MAX) {
            printf("Invalid PID value.\n");
            return NULL;
        }
        sprintf(param, "%u", id);
    } else if ((strcmp(to_lower(cmd), CMD_USER_UID)) == 0) {
        uid_t id = 0;
        printf("UID ?>");
        scanf("%u", &id);
        if (id > PID_UID_MAX) {
            printf("Invalid PID value.\n");
            return NULL;
        }
        sprintf(param, "%u", id);

    } else if ((strcmp(to_lower(cmd), CMD_USER_NAME)) == 0) {
        printf("Username ?>");
        scanf("%s", param);
        if ((strpbrk(param,"/\\")) != NULL){
            fprintf(stderr, "Don't try to buffer overflow my prgm pls :'(\n");
            return NULL;
        }
    } else if ((strcmp(to_lower(cmd), CMD_HELP)) == 0) {
        print_help();
        return extract_request();
    } else {
        printf("Unknown command ! Use 'help'.\n");
        return extract_request();
    }

    request *result = malloc(sizeof(*result));
    if (result == NULL) {
        perror("malloc");
        close_client();
        exit(EXIT_FAILURE);
    }
    strncpy(result->shm_linked, shm_name, SIZE_SHM_SEM_NAME);
    strncpy(result->sem_linked, sem_name, SIZE_SHM_SEM_NAME);
    strncpy(result->cmd_name, cmd, SIZE_CMD);
    strncpy(result->cmd_param, param, SIZE_PARAM);
    result->client_pid = getpid();
    return result;
}

void print_help(void) {
    printf("%s Help for the newbies.\n", CLIENT_HEADER);
    printf("%s Available commands are %s, %s, %s, %s and %s\n", CLIENT_HEADER, CMD_PROC, CMD_USER_UID, CMD_USER_NAME, CMD_HELP, CMD_EXIT);
}

void greet_user(char *id){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n",
      CLIENT_HEADER, CLIENT_VERSION);
    printf("%s You are client number %s\n", CLIENT_HEADER, id);
    printf("%s Please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
    print_help();
}

void close_client() {
    if (close(fifo_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(sem_name) == -1) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
    free(shm_name);
    free(sem_name);
}

static void handle_sigclient(int signum) {
    if (signum == SIGUSR1) {
        printf("\n%s Internal server error, client exited.\n", CLIENT_HEADER);
        close_client();
        exit(EXIT_FAILURE);
    }
    if (signum == SIGINT) {
        printf("\n%s See u later !\n", CLIENT_HEADER);
        close_client();
        exit(EXIT_SUCCESS);
    }
}

void manage_client_signals(void) {
    struct sigaction sigusr1act;
    sigusr1act.sa_handler = handle_sigclient;
    if (sigaction(SIGUSR1, &sigusr1act, NULL) < 0) {
        printf("%s Cannot manage SIGUSR1\n", CLIENT_HEADER);
        close_client();
    }
    struct sigaction sigintact;
    sigintact.sa_handler = handle_sigclient;
    if (sigaction(SIGINT, &sigintact, NULL) < 0) {
        printf("%s Cannot manage SIGINT\n", CLIENT_HEADER);
        close_client();
    }
}
