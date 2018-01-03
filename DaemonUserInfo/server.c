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
#include <pthread.h>

#include "server.h"
#include "info_user.h"
#include "info_proc.h"

/*
 * Fifo_fd doit être globale. Nous avons besoin de son état dans
 * L'appel des signaux.
 */
int fifo_fd;

// IL faut que l'on gère les signaux entrant sur le processus,
//    Ces signaux utiliseront close_server()

int main(void) {
    greet_user();
    fifo_fd = initialize_fifo();
    //Initialisation of the new behaviour for SIGINT must be called 
    //after FIFO initialisation. Before that, the default behaviour 
    //of SIGINT is appropriated.
    manage_signals();
    
    while (1) {
        treatment_request();
    }
    return EXIT_SUCCESS;
}

void greet_user(void){
    printf("%s Welcome to DaemonUserInfo Server (Version : %s)\n", SERVER_HEADER, SERVER_VERSION);
    printf("%s please visit https://github.com/antoineguillory/DaemonUserProcInfo for other informations\n", SERVER_HEADER);
}

int initialize_fifo(void) {
    fifo_fd = mkfifo(FIFO_SERVER_NAME, 0666);
    if (fifo_fd == -1) {
        fprintf(stderr, "%s Fifo creation failed. Initialisation aborted.\n", SERVER_HEADER);
        perror("Unknown SHM");
        exit(EXIT_FAILURE);
    }
    return fifo_fd;
}

struct th_args {
  char cmd[SIZE_INFO_PROC];
  char option[3];
  char arg[SIZE_PARAM];
};

void *run(struct th_args *a) {
  // fork et exec
  
  free(a);
  return NULL;
}


void treatment_request() {
    request *r = NULL;
    if ((size_t)read(fifo_fd, r, sizeof(*r)) < sizeof(*r)) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    
    pthread_t th;
    struct th_args *a = malloc(sizeof(struct th_args));
    if (a == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    strcpy(a->s, "");
    
    if (pthread_create(&th, NULL, (void *(*)(void *))run, a) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
  
    int shm_fd;
    if ((shm_fd = init_shm_ofclient(r->shm_linked)) == 0){
        close_server();
        exit(EXIT_FAILURE);
    }
    if (strcmp(r->cmd_name,"useru")==0){
        treatment_useruid(r, shm_fd);
    } else if (strcmp(r->cmd_name,"useri")==0) {
        //TODO
    } //TODO......
}

int init_shm_ofclient(char * shm_name){
    int fd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        return 0;
    }
    if (shm_unlink(shm_name) == -1) {
        perror("shm_unlink");
        return 0;
    }
    if (ftruncate(fd, SIZE_FIXE_SHM) == -1) {
        perror("ftruncate");
        return 0;
    }
    char *mem = mmap(NULL, SIZE_FIXE_SHM,
    PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 0;
    }
    return fd;
}

void treatment_useruid(request* r, int shm_fd){
    pthread_t th;
    //On déplace STDOUT vers le shm
    int old_stdout = STDOUT_FILENO;
    dup2(shm_fd, STDOUT_FILENO);
    enum choose_type type;
    type = NAME;
    usrargs* args = NULL;
    args->user = (void*)r->cmd_param;
    args->type = type;
    if(pthread_create(&th, NULL, &info_user, args)==-1){
        kill(r->client_pid,SIGUSR1);
    }
    void** retval = NULL;
    if(pthread_join(&th, retval)==-1){
        kill(r->client_pid,SIGUSR1);
    }
    if((int)retval==-1){
        kill(r->client_pid,SIGUSR1);
    }
    dup2(old_stdout, shm_fd);
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

void manage_signals(void) {
    struct sigaction sigintact;
    sigintact.sa_handler = handle_sigserver;
    if (sigaction(SIGINT, &sigintact, NULL) < 0) {
        printf("%s Cannot manage SIGINT\n",SERVER_HEADER);
        close_server();
    }
}

static void handle_sigserver(int signum){
    if (signum == SIGINT){
        close_server();
        printf("%s Shutdown... Goodbye !\n", SERVER_HEADER);
        exit(EXIT_SUCCESS);
    }
}
