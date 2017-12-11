#ifndef GLOBAL_SERVER_H
#define GLOBAL_SERVER_H

#define FIFO_SERVER_NAME  "/tmp/Daemon_User_Info_58062572.fifo"
#define SEM_RQST_NAME "/sem_58054648"
#define SEM_REPONSE_NAME "/sem_56186841"
#define SIZE_FIXE_SHM 2048

#define SERVER_VERSION "0.9"
#define SERVER_HEADER  "[SERVER]:"

/* @author antoine guillory, tom chambaretaud
 * @since 0.4
 * @brief modelize a request that contains the name of the shm
 * to respond. Second the cmd name and the param of the command.
 */
typedef struct s_request {
    char *shm_linked;
    char *cmd_name;
    char *cmd_param;
} request;

#endif  // GLOBAL_SERVER_H
