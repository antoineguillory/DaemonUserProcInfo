#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>

#include "server.h"

#include "global_server.h"
#include "util.h"

#define BUF_SIZE 128

#define EXIT_ERROR(r)                           \
    close_server();                             \
    if (kill(r->client_pid, SIGUSR1) == -1) {   \
        perror("kill");                         \
    }                                           \
    exit(EXIT_FAILURE);

/*
 * Fifo_fd doit être globale. Nous avons besoin de son état dans
 * l'appel des signaux.
 */
int fifo_fd;

int main(void) {
    greet_user();
    
    init_server();
    manage_server_signals();
    
    while (1) {
        request r;
        ssize_t n = read(fifo_fd, &r, sizeof(r));
        if (n == -1) {
            perror("read");
            close_server();
            exit(EXIT_FAILURE);
        }
        if (n == 0) {
            continue;
        }
        create_thread_request(&r);
    }
    return EXIT_SUCCESS;
}

void init_server() {
    if (file_exits(FIFO_SERVER_NAME)) {
        if (unlink(FIFO_SERVER_NAME) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
    }
    if (mkfifo(FIFO_SERVER_NAME, S_IRUSR | S_IWUSR) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    fifo_fd = open(FIFO_SERVER_NAME, O_RDONLY);
    if (fifo_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
}

char *read_response(int p_read) {
    char *buf = malloc(sizeof(*buf) * (BUF_SIZE + 1));
    ssize_t n;
    size_t len = BUF_SIZE;
    while ((n = read(p_read, (buf + len - BUF_SIZE), BUF_SIZE)) > 0) {
        len += (size_t) n;
        if (n == BUF_SIZE) {
            buf = realloc(buf, len);
            if (buf == NULL) {
                perror("realloc");
                return NULL;
            }
        }
    }
    if (n == -1) {
        perror("read");
        return NULL;
    }
    *(buf + len - 1) = '\0';
    return buf;
}

void exec_request(request *r) {
    if (strcmp(r->cmd_name, CMD_PROC) == 0) {
        execlp("./info_proc", "./info_proc", r->cmd_param, NULL);
        perror("execlp CMD_PROC");
    } else {
        char *cmd = "./info_user";
        if (strcmp(r->cmd_name, CMD_USER_NAME) == 0) {
            execlp(cmd, cmd, "-n", r->cmd_param, NULL);
            perror("execlp CMD_USER_NAME");
        } else if (strcmp(r->cmd_name, CMD_USER_UID) == 0) {
            execlp(cmd, cmd, "-u", r->cmd_param, NULL);
            perror("execlp CMD_USER_UID");
        } else {
            fprintf(stderr, "unknown cmd.");
        }
    }
    EXIT_ERROR(r);
}

void treatment_request(request *r) {
    int p[2];
    if (pipe(p) == -1) {
        perror("pipe");
        EXIT_ERROR(r);
    }

    char *response;
    switch (fork()) {
        case -1:
            perror("fork");
            EXIT_ERROR(r);
        case 0:
            if (close(p[0]) == -1) {
                perror("close");
                EXIT_ERROR(r);
            }
            if (dup2(p[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                EXIT_ERROR(r);
            }
            exec_request(r);
            fprintf(stderr, "exec_request");
            EXIT_ERROR(r);
        default:
            if(wait(NULL) == -1) {
                perror("wait");
                EXIT_ERROR(r);
            }
            if (close(p[1]) == -1) {
                perror("close");
                EXIT_ERROR(r);
            }
            response = read_response(p[0]);
            if (response == NULL) {
                EXIT_ERROR(r);
            }
            break;
    }
    
    char *shm;
    if ((shm = project_shm(r->shm_linked, SIZE_FIXE_SHM)) == NULL) {
        EXIT_ERROR(r);
    }
    
    strncpy(shm, response, SIZE_FIXE_SHM);
    free(response);
    
    sem_t *sem_client = sem_open(r->sem_linked, O_RDWR);
    if (sem_client == SEM_FAILED) {
        perror("sem_open");
        EXIT_ERROR(r);
    }
    if (sem_post(sem_client) == -1) {
        perror("sem_post");
        EXIT_ERROR(r);
    }
}

void create_thread_request(request *r) {
    pthread_attr_t attr;
    if (pthread_attr_init(&attr) != 0) {
        perror("pthread_attr_init");
        EXIT_ERROR(r);
    }
    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        EXIT_ERROR(r);
    }
    pthread_t th;    
    if (pthread_create(&th, &attr, (void *(*)(void *))treatment_request, r) != 0) {
        perror("pthread_create");
        EXIT_ERROR(r);
    }
}

void greet_user(void){
    printf("%s Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_HEADER, SERVER_VERSION);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", SERVER_HEADER);
}

void close_server(void) {
    if (fifo_fd != 0) {
        if (close(fifo_fd) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
    if (unlink(FIFO_SERVER_NAME) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}

static void handle_sigserver(int signum){
    if (signum == SIGINT){
        close_server();
        printf("%s Shutdown... Goodbye !\n", SERVER_HEADER);
        exit(EXIT_SUCCESS);
    }
}

void manage_server_signals(void) {
    struct sigaction sigintact;
    sigintact.sa_handler = handle_sigserver;
    if (sigaction(SIGINT, &sigintact, NULL) < 0) {
        printf("%s Cannot manage SIGINT\n",SERVER_HEADER);
        close_server();
    }
}
