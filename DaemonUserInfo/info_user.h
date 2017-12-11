#ifndef INFO_USER_H
#define INFO_USER_H

#define _XOPEN_SOURCE 500

#include <string.h>

#define PATH_PASSWD "/etc/passwd"
#define MAX_LENGTH_LINE 256

#define NEXT_INFO(line, save)   strtok_r(line, SEPARATOR_USER, &save)

#define SEPARATOR_USER ":"

/*
 *  Choose your type of information what you would communicate on input.
 *    Must be move to global.h
 */
enum choose_type {NAME, UID};

/*
 *  Send informations about user in STDOUT_FILENO.
 *    User can be indicate with uid or string.
 *    return -1 if error else 0.
 */
int info_user(void *user, enum choose_type type);

#endif  //INFO_USER_H
