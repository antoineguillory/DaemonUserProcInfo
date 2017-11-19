#ifndef SERVER_H
#define SERVER_H

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.2
 */
void greet_user();

enum types_commandes {PROCESS, USER};

/* @author antoine guillory
 * @brief initialize shm for communication regarding to processes and usrs
 * @since 0.2
 * @return fd of shm.
 */
int initialize_shm(enum types_commandes typecmd);


#endif
