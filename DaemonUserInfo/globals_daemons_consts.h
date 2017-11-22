#ifndef GLOBALS_DAEMON_CONSTS
#define GLOBALS_DAEMON_CONSTS

#define FIFO_RQST_NAME  "/tmp/Daemon_User_Info_58062572.fifo"

#define REQUEST_SEPARATOR ","
#define REQUEST_END       ";"

/* @author antoine guillory
 * @since 0.4
 * @brief modelize a request that contains the name of the shm to respond.
 * second the cmd name and the param of the command.
 */
struct Request {
    char* shm_linked;
    char* cmd_name;
    char* cmd_param;
};

#endif
