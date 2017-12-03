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
 * @brief convert a string formed by "req1,paramreq,shm_name;" to a well-named struct.
 * @returns -1 if convert failed, 0 else.
 * @params pointer to a struct. char of the wanted to be converted string
 * @since 0.4
 */
int str_to_request(request *req, char* str);

/* @author antoine guillory
 * @brief free ressources of the client.
 * @since 0.6
 */
void close_client();

#endif
