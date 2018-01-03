#ifndef GLOBAL_SERVER_H
#define GLOBAL_SERVER_H

#define FIFO_SERVER_NAME  "/tmp/Daemon_User_Info_58062572.fifo"
#define SIZE_FIXE_SHM 2048

#define SHM_NAME "/shm_"
#define SEM_NAME "/sem_"
#define NB_NUMBER_NAME_SHM_SEM 8
#define SIZE_INFO_PROC 10
#define SIZE_NAME_SHM 14
#define SIZE_CMD 6
#define SIZE_PARAM 256

#define SERVER_VERSION "0.9"
#define SERVER_HEADER  "[SERVER]:"

/* @author antoine guillory, tom chambaretaud
 * @since 0.4
 * @brief modelize a request that contains the name of the shm
 * to respond. Second the cmd name and the param of the command.
 */
typedef struct s_request {
    char shm_linked[SIZE_NAME_SHM];
    char sem_linked[SIZE_NAME_SHM];
    char cmd_name[SIZE_CMD];
    char cmd_param[SIZE_PARAM];
    pid_t client_pid;
} request;

#endif  // GLOBAL_SERVER_H
