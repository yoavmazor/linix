#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <stdint.h>
#include "check_exceptions.h"

#define EMPTY_STR "<NO VALUE>"

bool convert_to_ul(const char* str, unsigned int len, unsigned long* out);
ERROR_STATUS read_string_from_file(const char* path, char** out);
ERROR_STATUS read_link_path(const char* path, char** out);
ERROR_STATUS read_entire_file(int fd, char* buffer, uint64_t size, uint64_t* read_size);
char* join_path(const char* a, const char* b);
char* convert_null_to_empty(char* in);

#endif

