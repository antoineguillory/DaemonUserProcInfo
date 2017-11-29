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
#define MAX_LENGTH_LINE 256

#define NEXT_INFO(line, save)   strtok_r(line, SEPARATOR, &save)

/*
 *  Choose your type of information what you would communicate on input.
 */
enum choose_type {NAME, UID};

/*
 *  Simple copy in a new memomy allocated.
 */
char *cpy(char *s) {
  char *s2 = malloc(strlen(s) + 1);
  strcpy(s2, s);
  return s2;
}

/*
 *  Function comparation between uid : return 0 if user == user_current.
 */
int uidcmp(void *user, void *user_current) {
  return (*(uid_t *)user != *(uid_t *)user_current);
}

/*
 *  Function for print important informations about user with string line.
 */
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

/*
 *  Extract name of user with string line in new memory allocated.
 */
void *get_name(char *line) {
  char *save = NULL;
  char *name = NEXT_INFO(line, save);
  char *result = malloc(strlen(name));
  strcpy(result, name);
  return result;
}

/*
 *  Extract uid of user with string line in new memory allocated.
 */
void *get_uid(char *line) {
  char *save = NULL;
  NEXT_INFO(line, save);
  line = NULL;
  NEXT_INFO(line, save);
  char *uid_str = NEXT_INFO(line, save);
  uid_t *uid = malloc(sizeof(*uid));
  *uid = (uid_t) strtol(uid_str, NULL, 10);
  return uid;
}

/*
 *  Search user in f, if it find it, and it print it.
 */
int select_user(void *user, char **line, FILE *f,
      void *(*get)(char *), int (*cmp)(void *, void* )) {
  void *user_current;
  while (fgets(*line, 100, f) != NULL) {
    char *tmp = cpy(*line);
    user_current = get(tmp);
    free(tmp);
    if (cmp(user, user_current) == 0) {
      break;
    }
    free(user_current);
  }
  if (cmp(user, user_current) != 0) {
    return -1;
  }
  free(user_current);
  return 0;
}

int info_user(void *user, enum choose_type type) {
  FILE *f;
  if ((f = fopen(PATH_PASSWD,"r")) == NULL) {
    perror("fopen");
    return -1;
  }

  char *line = malloc(MAX_LENGTH_LINE);
  void *(*get)(char *) = NULL;
  int (*cmp)(void *, void *) = NULL;
  if (type == NAME) {
    get = get_name;
    cmp = (int (*)(void *, void *)) strcmp;
  } else if (type == UID) {
    get = get_uid;
    cmp = uidcmp;
  } else {
      fprintf(stderr, "type doesn't found\n");
      return -1;
  }
  if (select_user(user, &line, f, get, cmp) == -1) {
      if (fclose(f) == -1) {
        perror("fclose");
        return -1;
      }
      fprintf(stderr, "user doesn't found\n");
      return -1;
    }
  print_user(line);


  free(line);
  if (fclose(f) == -1) {
    perror("fclose");
    return -1;
  }
  return 0;
}

int main(void) {
  uid_t uid = 118;
  if (info_user(&uid, UID) == -1) {
    printf("T null");
    return EXIT_FAILURE;
  }
  char *name = "irc";
  if (info_user(name, NAME) == -1) {
    printf("T null");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
