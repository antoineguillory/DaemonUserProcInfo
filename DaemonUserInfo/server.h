#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "globals_daemons_consts.h"
#include "server_consts.h"

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
void wait_for_next_question(int fifo_fd, sem_t *sem);

/* @author antoine guillory
 * @brief free ressources. must be called when server needs to stop
 * @since 0.3
 */
void close_server(int fifo_fd);


#endif
