#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <stdint.h>
#include "check_exceptions.h"

#define EMPTY_STR "<NO VALUE>"

/* converts the given string to unsigned long
 * returns true if successful
 * writes the result to out
 */
bool convert_to_ul(const char* str, unsigned int len, unsigned long* out);

/*
 * reads the string stored in the file with the given path
 * writes the string to the buffer given by out
 * returns the ERROR_OCCURRED if failed
 */
ERROR_STATUS read_string_from_file(const char* path, char** out);

/*
 * reads the link pointed path of the given link path
 * writes the string to the buffer given by out
 * returns the ERROR_OCCURRED if failed
 */
ERROR_STATUS read_link_path(const char* path, char** out);

/*
 * reads up to size bytes from the given file
 * writes the bytes to the given buffer
 * returns the ERROR_OCCURRED if failed
 */
ERROR_STATUS read_file(int fd, char* buffer, uint64_t size, uint64_t* read_size);

/*
 * joins the strings to form a valid path string
 * returns the path
 */
char* join_path(const char* a, const char* b);

/*
 * if the given string is NULL, returns an empty string.
 * else, returns the string itself
 */
char* convert_null_to_empty(char* in);

#endif

