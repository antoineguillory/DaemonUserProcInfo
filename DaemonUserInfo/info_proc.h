#ifndef INFO_PROC_H
#define INFO_PROC_H

#include <sys/types.h>

#define SEPARATOR_PROC ":"

#define PREFIX_FILE "/proc/"
#define SUFFIX_FILE_STATUS "/status"
#define SUFFIX_FILE_CMD "/cmdline"

/*
 * Function read in stdout information of proc with pid = pid.
 *  Format = "cmdline:name:umask:state:pid:ppid:uidreal:uideffect:	\
 *    uidsave:gidreal:gideffect:uidsave\n"
 *
 *    return -1 if error else 0.
 */
int info_proc(pid_t pid);

#endif  //INFO_PROC_H
