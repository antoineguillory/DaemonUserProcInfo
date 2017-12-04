#ifndef INFO_USER
#define INFO_USER

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#define SEPARATOR_USER ":"

/*
 *  Send informations about user in STDOUT_FILENO.
 *    User can be indicate with uid or string.
 * 		return -1 if error else 0.
 */
int info_user(void *user, enum choose_type type);

#endif	//INFO_USER
