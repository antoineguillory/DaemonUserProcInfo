#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PATH_PASSWD "/etc/passwd"
#define SEPARATOR ":"
#define NEXT_INFO(line, save)   strtok_r(line, SEPARATOR, &save)

char *cpy(char *s) {
  char *s2 = malloc(strlen(s) + 1);
  strcpy(s2, s);
  return s2;
}

uid_t get_uid(char *line) {
  char *save = NULL;
  NEXT_INFO(line, save);
  line = NULL;
  NEXT_INFO(line, save);
  char *uid_str = NEXT_INFO(line, save);
  return (uid_t) atoi(uid_str);
}

void print_user(char *line) {
  *(line + strlen(line) - 1) = '\0';
  char *save = NULL;
  char *token = NULL;
  printf("Username : %s\n", NEXT_INFO(line, save));
  line = NULL;
  printf("Password : %s\n", NEXT_INFO(line, save));
  printf("Uuid : %s\n", NEXT_INFO(line, save));
  printf("Guid : %s\n", NEXT_INFO(line, save));
  token = NEXT_INFO(line, save);
  if (*token != '/') {
    printf("GECOS : %s\n", token);
    token = NEXT_INFO(line, save);
  }
  printf("Directory : %s\n", token);
  if ((token = NEXT_INFO(line, save)) != NULL) {
    printf("Shell : %s\n", token);
  }
}

int info_user(uid_t user) {
  FILE *f;
  if ((f = fopen(PATH_PASSWD,"r")) == NULL) {
    perror("fopen");
    return -1;
  }
  char line[256];
  uid_t uid_current;
  while (fgets(line, 100, f) != NULL) {
    char *tmp = cpy(line);
    uid_current = get_uid(tmp);
    free(tmp);
    if (uid_current == user) {
      break;
    }
  }
  if (fclose(f) == -1) {
    perror("fclose");
    return -1;
  }
  if (uid_current != user) {
    fprintf(stderr, "user doesn't found : %s\n", line);
    return -1;
  }
  print_user(line);
  return 0;
}
