#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define PREFIX_FILE "/proc/"
#define SUFFIX_FILE "/status"
#define SKIP_LINE(fd) while (fgetc(fd) != '\n');

size_t degre_int(int n) {
  size_t k = 1;
  while (n >= 10) {
    n /= 10;
    k++;
  }
  return k;
}

char *define_filename(pid_t pid) {
  size_t pid_size = degre_int((int) pid);
  char *str_pid = malloc(sizeof(*str_pid) * pid_size);
  if (str_pid == NULL) {
    perror("malloc");
    return NULL;
  }
  char *filename = malloc(sizeof(*filename)
      * (strlen(PREFIX_FILE) + pid_size + strlen(SUFFIX_FILE)));
  if (filename == NULL) {
    perror("malloc");
    return NULL;
  }
  strcpy(filename, PREFIX_FILE);
  sprintf(str_pid, "%d", ((int) pid));
  strcat(filename, str_pid);
  strcat(filename, SUFFIX_FILE);
  return filename;
}

void read_value(FILE *fd) {
  int fg = fgetc(fd);
  while (fg != '\n' && fg != EOF) {
    printf("%c", (char) fg);
    fg = fgetc(fd);
  }
  printf("\n");
}

int info_proc(pid_t pid) {
  char *filename = define_filename(pid);
  if (filename == NULL) {
    return -1;
  }
  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    perror("fopen");
    return -1;
  }

  read_value(fd);  //Name
  SKIP_LINE(fd);
  read_value(fd);  //State
  read_value(fd);  //Tgid
  SKIP_LINE(fd);
  read_value(fd);  //Pid
  read_value(fd);  //PPid
  SKIP_LINE(fd);
  read_value(fd);  //Uid
  read_value(fd);  //Gid

  if (fclose(fd) == -1) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  return 0;
}
