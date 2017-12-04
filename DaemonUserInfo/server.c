#include "server.h"

void greet_user(){
    printf("%s Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_HEADER, SERVER_VERSION);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", SERVER_HEADER);
}

int initialize_fifo() {
    int fifo_fd = mkfifo(FIFO_RQST_NAME, 0666);
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

void treatment_request(int fifoFD) {
    request *r = NULL;
    if (read(fifoFD, r, sizeof(*r)) < sizeof(*r)) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    // + appel à la fonction demandée
    // + lecture du résultat
    // + envoie du résultat
}

void wait_for_next_question(int fifoFD, sem_t *sem) {
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
                treatment_request(fifoFD);
        }
    }
}

void close_server(int fifo_fd) {
    if (fifo_fd != 0) {
        if (close(fifo_fd) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }
    // Suppression du fifo ?
    if (unlink(FIFO_RQST_NAME) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
}

// IL faut que l'on gère les signaux entrant sur le processus,
//    Ces signaux utiliseront close_server()

int main(void) {
    greet_user();

    int fifoFD = initialize_fifo();
    sem_t *sem = initialize_sem(SEM_RQST_NAME, 0);

    wait_for_next_question(fifoFD, sem);

    return EXIT_SUCCESS;
}
