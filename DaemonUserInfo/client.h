#ifndef SERVER_H
#define SERVER_H

/* @author antoine guillory
 * @brief initialize the fifo to communicate between clients & server
 * @since 0.5
 * @return fd of fifo
 */
int initialize_fifo();

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.5
 */
void greet_user();

/* @author antoine guillory
 * @brief wait that the user inputs a command.
 * @since 0.5
 */
char* wait_user_input();

/* @author antoine guillory
 * @brief free ressources of the client.
 * @since 0.6
 */
void close_client();

#endif
