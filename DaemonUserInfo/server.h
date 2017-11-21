#ifndef SERVER_H
#define SERVER_H

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.2
 */
void greet_user();

enum types_commandes {PROCESS, USER};

/* @author antoine guillory
 * @brief initialize the fifo to communicate between clients & server
 * @since 0.3
 * @return fd of fifo
 */
int initialize_fifo();

/* @author antoine guillory
 * @brief free ressources. must be called when server needs to stop
 * @since 0..3
 */
void closeServer(int fifo_fd);


#endif
