#include "info_user.h"

#define PATH_PASSWD "/etc/passwd"
#define MAX_LENGTH_LINE 256

#define NEXT_INFO(line, save)   strtok_r(line, SEPARATOR_USER, &save)

/*
 *  Simple copy in a new memomy allocated.
 */
char *cpy(char *s) {
    char *s2 = malloc(strlen(s) + 1);
    if (s2 == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
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
 *  Function for print important informations about user with one string.
 *    format : "Username:Password:Uuid:Guid:GECOS:Directory:Shell\n"
 */
void print_user(char *line) {
    *(line + strlen(line) - 1) = '\0';
    char *save = NULL;
    char *token = NULL;
    printf("%s%s", NEXT_INFO(line, save), SEPARATOR_USER);   //Username
    line = NULL;
    printf("%s%s", NEXT_INFO(line, save), SEPARATOR_USER);   //Password
    printf("%s%s", NEXT_INFO(line, save), SEPARATOR_USER);   //Uuid
    printf("%s%s", NEXT_INFO(line, save), SEPARATOR_USER);   //Guid
    token = NEXT_INFO(line, save);
    if (*token != '/') {
        printf("%s%s", token, SEPARATOR_USER);               //GECOS
        token = NEXT_INFO(line, save);
    }
    printf("%s%s", token, SEPARATOR_USER);                   //Directory
    if ((token = NEXT_INFO(line, save)) != NULL) {
        printf("%s", token);                			//Shell
    }
    printf("\n");
}

/*
 *  Extract name of user with string line in new memory allocated.
 */
void *get_name(char *line) {
    char *save = NULL;
    char *name = NEXT_INFO(line, save);
    char *result = malloc(strlen(name));
    if (result == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    strcpy(result, name);
    return result;
}

/*
 *  Extract uid of user with string line in new memory allocated.
 *    return NULL if error else uid of user.
 */
void *get_uid(char *line) {
    char *save = NULL;
    NEXT_INFO(line, save);
    line = NULL;
    NEXT_INFO(line, save);
    char *uid_str = NEXT_INFO(line, save);
    uid_t *uid = malloc(sizeof(*uid));
    if (uid == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    long int r = strtol(uid_str, NULL, 10);
    if (r == LONG_MIN || r == LONG_MAX) {
      perror("strtol");
      return NULL;
    }
    *uid = (uid_t) r;
    return uid;
}

/*
 *  Search user in f, if it find it, and it print it.
 *    return -1 if error else 0.
 */
int select_user(void *user, char **line, FILE *f,
      void *(*get)(char *), int (*cmp)(void *, void* )) {
    void *user_current;
    while (fgets(*line, 100, f) != NULL) {
        char *tmp = cpy(*line);
        user_current = get(tmp);
        if (user_current == NULL) {
          fprintf(stderr, "Problem current user.\n");
          return -1;
        }
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
    if (line == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
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

//  Jeux de test

// int main(void) {
    // uid_t uid = 118;
    // if (info_user(&uid, UID) == -1) {
        // printf("T null");
        // return EXIT_FAILURE;
    // }
    // char *name = "irc";
    // if (info_user(name, NAME) == -1) {
        // printf("T null");
        // return EXIT_FAILURE;
    // }
    // return EXIT_SUCCESS;
// }
