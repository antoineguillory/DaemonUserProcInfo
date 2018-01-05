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

#define PID_UID_MAX 32768

#define CLIENT_HEADER  "[CLIENT] :"
#define CLIENT_VERSION "0.6"

request *extract_request(char *shm_name, char *sem_name);

void greet_user(char *id);

void close_client();

void manage_client_signals(void);

#endif  //CLIENT_H
