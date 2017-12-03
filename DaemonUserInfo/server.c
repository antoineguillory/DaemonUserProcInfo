#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "globals_daemons_consts.h"
#include "server_consts.h"
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

void treatment_request(int fifoFD, sem_t *sem_rqst, sem_t *sem_treat) {
	request *r = NULL;
	if (read(fifoFD, r, sizeof(*r)) < sizeof(*r)) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	// traitement du parametre si info_user
	// + appel à la fonction demandée
	// + lecture du résultat
	// + envoie du résultat
}

void wait_for_next_question(int fifoFD) {
	while (1) {
		if (sem_wait(sem_rqst) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}
		if (sem_wait(sem_treat) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
		}
		treatment_request(fifoFD);
		if (sem_post(sem_treat) == -1) {
			perror("sem_wait");
			exit(EXIT_FAILURE);
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
}

// IL faut que l'on gère les signaux entrant sur le processus,
// 	Ces signaux utiliseront close_server()

int main(void) {
    greet_user();
    
    int fifoFD = initialize_fifo();
    sem_t *sem_rqst = initialize_sem(SEM_RQST_NAME, 0);
    sem_t *sem_treat = initialize_sem(SEM_TREAT_NAME, 1);
    
    wait_for_next_question(fifoFD, sem_rqst, sem_treat);
    
    return EXIT_SUCCESS;
}
