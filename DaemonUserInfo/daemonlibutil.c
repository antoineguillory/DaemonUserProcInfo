#include "daemonlibutil.h"

char* to_lower(char* str){
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    return str;
}

char* concat(char *s1, char *s2) {
    char *s = malloc(strlen(s1) + strlen(s2) + 1);
    if (s == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
    strcpy(s, s1);
    strcat(s, s2);
    return s;
}

void rand_str(char *dest, size_t length) {
    srand(time(NULL));
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX
				* (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

//Main de test de la bibliothèque utilitaire.
/*int main(void) {
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char coolstr[]= "cool";
    char concatstr[]="concat";
    printf("TEST TOLOWER : ABCDEFGHIJKLMNOPQRSTUVWXYZ : %s\n", to_lower(alpha));
    printf("test concat str1: cool str2:concat : %s\n",concat("cool","concat"));
    char* cool = malloc(strlen(coolstr)+strlen(concatstr)+1);
    char* randomstr=malloc(15);
    rand_str(randomstr, (size_t)15);
    printf("test random STR de taille 15 : %s\n", randomstr);
    free(cool);
    return 0;
}*/
