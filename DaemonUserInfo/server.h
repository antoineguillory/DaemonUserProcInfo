#ifndef SERVER_H
#define SERVER_H

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.2
 */
void greet_user();

/* @author antoine guillory
 * @brief initialize the fifo to communicate between clients & server
 * @since 0.3
 * @return fd of fifo
 */
int initialize_fifo();

/* @author antoine guillory
 * @brief wait for the next question from a client.
 * @since 0.4
 */
int wait_for_next_question(int fifo_fd);

/* @author antoine guillory
 * @brief convert a string formed by "shm_name,req1,paramreq;" to a well-named struct.
 * @returns -1 if convert failed, 0 else.
 * @params pointer to a struct. char of the wanted to be converted string
 * @since 0.4
 */
int str_to_request(struct Request* req, char* str);

/* @author antoine guillory
 * @brief free ressources. must be called when server needs to stop
 * @since 0.3
 */
void closeServer(int fifo_fd);


#endif
