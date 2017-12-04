#ifndef INFO_PROC
#define INFO_PROC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SEPARATOR_PROC ":"

/*
 * Function read in stdout information of proc with pid = pid.
 *  Format = "cmdline:name:umask:state:pid:ppid:uidreal:uideffect:	\
 *    uidsave:gidreal:gideffect:uidsave\n"
 *
 *    return -1 if error else 0.
 */
int info_proc(pid_t pid);

#endif	//INFO_PROC
