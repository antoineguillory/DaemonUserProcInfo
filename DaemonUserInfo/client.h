#ifndef CLIENT_H
#define CLIENT_H

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "util.h"
#include "global_server.h"

#define SHM_NAME  "/shm_"
#define SIZE_CMD 6
#define PID_UID_MAX 32768

#define CMD_PROC  "proc"
#define CMD_USER_UID "useru"
#define CMD_USER_NAME "usern"
#define CMD_EXIT  "exit"

#define CLIENT_HEADER  "[CLIENT] :"
#define CLIENT_VERSION "0.6"

void print_help();

/* @author antoine guillory
 * @brief greets the user while starting the server
 * @since 0.5
 */
void greet_user();

int open_fifo(char *fifo_name);

void close_fifo(int fifo_fd);

char *initialize_shm(char *shm_name);

request *extract_request(char *shm_name);

int send_request(int fifo_fd, request *r);

int wait_reponse();

/* @author antoine guillory
 * @brief free ressources of the client.
 * @since 0.6
 */
void close_client();

#endif  //CLIENT_H
