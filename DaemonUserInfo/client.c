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
char *shm_name;
char *sem_name;

int main(void) {
    manage_client_signals();
    fifo_fd = open(FIFO_SERVER_NAME, O_WRONLY);
    if (fifo_fd == -1) {
        printf("Server doesn't exist or closed.");
        exit(EXIT_FAILURE);
    }
    char *id = rdmnb_to_str(NB_NUMBER_NAME_SHM_SEM);
    shm_name = concat(SHM_NAME, id);
    char *shm = init_shm(shm_name, SIZE_FIXE_SHM);
    sem_name = concat(SEM_NAME, id);
    sem_t *sem = init_sem(sem_name, 0);
    
    greet_user(id);
    while (1) {
        request *r = extract_request(shm_name, sem_name);
        if (r == NULL) {
            break;
        }
        if (write(fifo_fd, r, sizeof(*r)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        free(r);
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        printf("%s %s", CLIENT_HEADER, shm);
    }
    close_client();
    printf("%s See u later !\n", CLIENT_HEADER);
    return EXIT_SUCCESS;
}

request *extract_request(char *shm_name, char *sem_name) {
    char cmd[SIZE_CMD];
    char param[256];
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
    } else {
        printf("Unknown command !\n");
        return extract_request(shm_name, sem_name);
    }

    request *result = malloc(sizeof(*result));
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(result->shm_linked, shm_name, SIZE_NAME_SHM_SEM);
    strncpy(result->sem_linked, sem_name, SIZE_NAME_SHM_SEM);
    strncpy(result->cmd_name, cmd, SIZE_CMD);
    strncpy(result->cmd_param, param, SIZE_PARAM);
    result->client_pid = getpid();
    return result;
}

void greet_user(char *id){
    printf("%s Welcome to DaemonUserInfo Client (Version : %s)\n",
      CLIENT_HEADER, CLIENT_VERSION);
    printf("%s Your are client number %s\n", CLIENT_HEADER, id);
    printf("%s Available commands are '%s {PID}'' '%s {UID}''%s {UserName}' and '%s'\n", CLIENT_HEADER, CMD_PROC, CMD_USER_UID, CMD_USER_NAME, CMD_EXIT);
    printf("%s Please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", CLIENT_HEADER);
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
