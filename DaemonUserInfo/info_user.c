#define _DEFAULT_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_PASSWD "/etc/passwd"
#define SEPARATOR ":"

char *next_info(char *line) {
  return strsep(&line, SEPARATOR);
}

char *cpy(char *s) {
  char *s2 = malloc(strlen(s) + 1);
  strcpy(s2, s);
  return s2;
}

uid_t get_uid(char *line) {
  strsep(&line, SEPARATOR);
  strsep(&line, SEPARATOR);
  char *uid_str = strsep(&line, SEPARATOR);
  return (uid_t) atoi(uid_str);
}

int info_user(uid_t user) {
  FILE *f;
  if ((f = fopen(PATH_PASSWD,"r")) == NULL) {
    perror("fopen");
    return -1;
  }
  char line[256];
  int user_found = -1;
  while (fgets(line, 100, f) != NULL) {
    char *tmp = cpy(line);
    if (get_uid(tmp) == user) {
      user_found = 0;
      printf("%u : %s", user, line);
      free(tmp);
      break;
    }
    free(tmp);
  }
  if (fclose(f) == -1) {
    perror("fclose");
    return -1;
  }
  return user_found;
}

int main(void) {
  if (info_user(1) == -1) {
    return EXIT_FAILURE;
  }
  return (EXIT_SUCCESS);
}
