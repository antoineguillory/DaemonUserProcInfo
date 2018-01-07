#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define SEPARATOR_PROC ":"

#define PREFIX_FILE "/proc/"
#define SUFFIX_FILE_STATUS "/status"
#define SUFFIX_FILE_CMD "/cmdline"

/*
 * Function must be move to util.c
 *    return degre ten of argument n.
 */
size_t degre_int(int n) {
    size_t k = 1;
    while (n >= 10) {
        n /= 10;
        k++;
    }
    return k;
}

/*
 *  Function which define the filename by suffix.
 */
char *define_filename(pid_t pid, char *suffix) {
    size_t pid_size = degre_int((int) pid);
    char *str_pid = malloc(sizeof(*str_pid) * pid_size);
    if (str_pid == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    char *filename = malloc(sizeof(*filename)
      * (strlen(PREFIX_FILE) + pid_size + strlen(suffix)));
    if (filename == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(filename, PREFIX_FILE);
    sprintf(str_pid, "%d", ((int) pid));
    strcat(filename, str_pid);
    free(str_pid);
    strcat(filename, suffix);
    return filename;
}

/*
 * Function skip information with skip == '\t'
 *    or line with skip == '\n'
 */
void skip(FILE *fd, char skip) {
    int c = fgetc(fd);
    while (c != skip) {
        if (c == EOF) {
            fprintf(stderr, "end of file or error SKIP\n");
            exit(EXIT_FAILURE);
        }
        c = fgetc(fd);
    }
}

/*
 *  Function read in stdout content of fd.
 */
void read_content(FILE *fd) {
    printf("Cmdline: ");
    int fg = fgetc(fd);
    while (fg != '\0' && fg != EOF) {
        printf("%c", (char) fg);
        fg = fgetc(fd);
    }
}

/*
 * Function read information without introduction of information.
 *    This function read one line.
 */
void read_line(FILE *fd) {
    int fg = fgetc(fd);
    while (fg != '\n' && fg != EOF) {
        if (fg == '\t') {
            printf(" ");
        } else {
            printf("%c", (char) fg);
        }
        fg = fgetc(fd);
    }
    if (fg == EOF) {
        fprintf(stderr, "end of file or error\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");
}

/*
 * Function read different informations of file status.
 */
void read_status(FILE *fd) {
    read_line(fd);    // Name
    read_line(fd);    // Umask
    read_line(fd);   // State
    skip(fd, '\n');
    skip(fd, '\n');
    read_line(fd);    // Pid
    read_line(fd);    // Pid of parent process
    skip(fd, '\n');
    read_line(fd);    // Uid Real, effective, saved set
    read_line(fd);    // Gid Real, effective, saved set
    skip(fd, '\n');
    read_line(fd);    // Groups
    for (int k = 0; k < 21; k++) {
        skip(fd, '\n');
    }
    read_line(fd);    // Thread
}

/*
 * Function read informations of file
 *    with suffix = suffix and function read.
 *    return -1 if error else 0.
 */
int info_proc_read(pid_t pid, char *suffix, void (*fct_read)(FILE *)) {
    char *filename = define_filename(pid, suffix);
    if (filename == NULL) {
        return -1;
    }

    FILE *fd = fopen(filename, "r");
    free(filename);
    if (fd == NULL) {
        printf("proc doesn't exist.\n");
        return -1;
    }

    fct_read(fd);

    if (fclose(fd) == -1) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/*
 * Function read in stdout information of proc with pid = pid.
 *  Format = "cmdline:$(value)\n[...]"
 *
 *    return -1 if error else 0.
 */
int info_proc(pid_t pid) {
    if (info_proc_read(pid, SUFFIX_FILE_CMD, read_content) == -1) {
        return -1;
    }
    printf("\n");
    if (info_proc_read(pid, SUFFIX_FILE_STATUS, read_status) == -1) {
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Too many arguments.\n");
    return EXIT_FAILURE;
  }
  pid_t n = strtol(argv[1], NULL, 10);
  if (n == 0 && strcmp(argv[1], "0") != 0) {
    printf("pid isn't correct.\n");
    return EXIT_FAILURE;
  }
  if (info_proc(n) == -1) {
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
