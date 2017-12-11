#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "server.h"

/*
 * Fifo_fd doit être globale. Nous avons besoin de son état dans
 * L'appel des signaux.
 */
int fifo_fd;

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_HEADER, SERVER_VERSION);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", SERVER_HEADER);
}

int initialize_fifo() {
    fifo_fd = mkfifo(FIFO_SERVER_NAME, 0666);
    if (fifo_fd == -1) {
        fprintf(stderr, "%s Fifo creation failed. Initialisation aborted.\n", SERVER_HEADER);
        perror("Unknown SHM");
        exit(EXIT_FAILURE);
    }
    return fifo_fd;
}

sem_t *initialize_sem(char *sem_name, unsigned int value) {
    sem_t *sem = sem_open(sem_name, O_CREAT, S_IRUSR | S_IWUSR, value);
    if (sem == SEM_FAILED) {
        fprintf(stderr, "%s sem creation failed. Initialisation aborted.\n", SERVER_HEADER);
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink(sem_name) == -1) {
        fprintf(stderr, "%s sem creation failed. Initialisation aborted.\n", SERVER_HEADER);
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void treatment_request(int fifo_fd) {
    request *r = NULL;
    if ((size_t)read(fifo_fd, r, sizeof(*r)) < sizeof(*r)) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    // + appel à la fonction demandée
    // + lecture du résultat
    // + envoie du résultat
}

void wait_for_next_question(int fifo_fd, sem_t *sem) {
    while (1) {
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
        switch (fork()) {
            case -1 :
                perror("fork");
                exit(EXIT_FAILURE);
            case 0 :
                break;
            default :
                treatment_request(fifo_fd);
        }
    }
}

void close_server(void) {
    if (fifo_fd != 0) {
        if (close(fifo_fd) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
    // Suppression du fifo ?
    if (unlink(FIFO_SERVER_NAME) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}

void manage_signals(void) {
    struct sigaction sigintact;
    sigintact.sa_handler = handle_sig;
    if (sigaction(SIGINT, &sigintact, NULL) < 0) {
        printf("%s Cannot manage SIGINT\n",SERVER_HEADER);
        close_server();
    }
}

static void handle_sig(int signum){
    if(signum==SIGINT){
        close_server();
        printf("%s Shutdown... Goodbye !\n", SERVER_HEADER);
        exit(EXIT_SUCCESS);
    }
}

// IL faut que l'on gère les signaux entrant sur le processus,
//    Ces signaux utiliseront close_server()

int main(void) {
    greet_user();
    int fifo_fd = initialize_fifo();
    sem_t *sem = initialize_sem(SEM_RQST_NAME, 0);
    //Initialisation of the new behaviour for SIGINT must be called 
    //after FIFO initialisation. Before that, the default behaviour 
    //of SIGINT is appropriated.
    manage_signals();
    wait_for_next_question(fifo_fd, sem);

    return EXIT_SUCCESS;
}
