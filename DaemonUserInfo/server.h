#ifndef SERVER_H
#define SERVER_H

#include "global_server.h"

#include <semaphore.h>

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.2
 */
void greet_user(void);

/* @author antoine guillory
 * @brief initialize the fifo to communicate between clients & server
 * @since 0.3
 * @return fd of fifo
 */
int initialize_fifo(void);

int init_shm_ofclient(char * shm_name);


/* @author antoine guillory
 * @brief wait for the next question from a client.
 * @since 0.4
 */
void wait_for_next_question(int fifo_fd, sem_t *sem);

void treatment_request(int fifo_fd);

void treatment_useruid(request* r, int shm_fd);

/* @author antoine guillory
 * @brief free ressources. must be called when server needs to stop
 * @since 0.3
 */
void close_server(void);

static void handle_sigserver(int signum);

/* @author antoine guillory
 * @brief manage signals to close ressources properly
 * @since 0.9
 */
void manage_signals(void);


#endif   //SERVER_H
