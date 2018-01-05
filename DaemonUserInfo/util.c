#include "util.h"

#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

char* to_lower(char* str){
    for(int i = 0; str[i]; i++){
        str[i] = (char) tolower(str[i]);
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
    srand((unsigned int)time(NULL));
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0) {
        size_t index = (size_t) rand() / RAND_MAX
          * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

char *rdmnb_to_str(size_t n) {
    srand((unsigned int)time(NULL));
    char *result = malloc(sizeof(*result) * n + 1);
    if (result == NULL) {
      perror("malloc");
      return NULL;
    }
    unsigned int value;
    for (unsigned int i = 0; i < n; i++) {
        value = (unsigned int)rand()%10;
        *(result + i) = (char)(value + '0');
    }
    *(result + n) = '\0';
    return result;
}

sem_t *init_sem(char *sem_name, unsigned int value) {
    sem_t *sem = sem_open(sem_name, O_RDWR | O_CREAT | O_EXCL,
        S_IRUSR | S_IWUSR, value);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    return sem;
}

void *init_shm(char *shm_name, size_t size) {
    int fd = shm_open(shm_name, O_RDWR | O_CREAT,
        S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    if (ftruncate(fd, (off_t)size) == -1) {
        perror("ftruncate");
        return NULL;
    }
    void *mem = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED,
        fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return mem;
}

void *project_shm(char *shm_name, size_t size) {
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    void *mem = mmap(NULL, size,
        PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    return mem;
}

int file_exits(const char *filename) {
  struct stat s;
  return (stat(filename, &s) != -1);
}
