#ifndef UTIL_H
#define UTIL_H

#include <semaphore.h>

/* @author https://stackoverflow.com/questions/2661766/c-convert-a-mixed-case-string-to-all-lower-case
 * @brief return the to lower version of this string
 */
char* to_lower(char* str);

/* @author tom chambaretaud
 * @brief concat to strings with proper mallocs
 */
char* concat(char *s1, char *s2);

/* @author https://stackoverflow.com/questions/15767691/whats-the-c-library-function-to-generate-random-string , antoine guillory (improvements)
 * @brief generate a random string with the proper length
 */
void rand_str(char *dest, size_t length);

char *rdmnb_to_str(size_t n);

sem_t *init_sem(char *sem_name, unsigned int value);

void *project_new_shm(char *shm_name, size_t size);

void *project_shm(char *shm_name, size_t size);

int file_exits(const char *filename);

#endif // UTIL_H
