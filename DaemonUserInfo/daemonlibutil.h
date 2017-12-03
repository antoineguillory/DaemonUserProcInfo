#ifndef DAEMONLIBUTIL_H
#define DAEMONLIBUTIL_H

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
#endif
