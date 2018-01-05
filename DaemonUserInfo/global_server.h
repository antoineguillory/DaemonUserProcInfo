#ifndef GLOBAL_SERVER_H
#define GLOBAL_SERVER_H

#include <sys/types.h>

#define FIFO_SERVER_NAME  "/tmp/Daemon_User_Info_58062572.fifo"
#define SIZE_FIXE_SHM 4096

#define SIZE_SHM_SEM_NAME 14
#define SIZE_CMD 6
#define SIZE_PARAM 256

#define CMD_PROC  "proc"
#define CMD_USER_UID "useru"
#define CMD_USER_NAME "usern"
#define CMD_EXIT  "exit"

/* Type to modelize a request that contains the name of the shm and sem
 * to respond. Second the cmd name and the param of the command.
 * The server need pid to kill the client during an error.
 */
typedef struct s_request {
    char shm_linked[SIZE_SHM_SEM_NAME];
    char sem_linked[SIZE_SHM_SEM_NAME];
    char cmd_name[SIZE_CMD];
    char cmd_param[SIZE_PARAM];
    pid_t client_pid;
} request;

#endif  // GLOBAL_SERVER_H
