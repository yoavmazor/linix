#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "utils.h"
#include "check_exceptions.h"

#define NO_SUCH_FILE_OR_DIRECTORY 2
#define PRINTABLE_CHAR_START 32

bool convert_to_ul(const char* str, unsigned int len, unsigned long* out) {
    unsigned long num = strtoul(str, NULL, 0);
    
    if (num == 0) {
        if (len != 1 || str[0] != '0') {
            return false;
        }
    }

    *out = num;
    return true;
}

ERROR_STATUS read_string_from_file(const char* path, char** out) {
    ERROR_STATUS err = NO_ERROR_OCCURRED;
    uint64_t file_size = 0;
    char* file_str = NULL;
    uint64_t size = 0;

    int fd = open(path, O_RDWR);
    CHECK_TRUE(fd != -1);

    file_size = PATH_MAX + 1;
    
    file_str = calloc(file_size + 1, sizeof(char));
    CHECK_TRUE(file_str != NULL);

    CHECK_AND_PASS(read_file(fd, file_str, file_size, &size));

    if (file_str[0] == '\0') {
        free(file_str);        
        file_str = NULL;
    } else if (file_str[strlen(file_str) - 1] == '\n') {
        file_str[strlen(file_str) - 1] = '\0';
    }

exit:
    if (fd != -1) {
        close(fd);
    }

    if (err == ERROR_OCCURRED) {
        free(file_str);
    }

    *out = file_str;
    return err;
}

ERROR_STATUS read_link_path(const char* path, char** out) {
    ERROR_STATUS err = NO_ERROR_OCCURRED;
    char* link_str = NULL;
    ssize_t len = -1;

    link_str = calloc(PATH_MAX + 1, sizeof(char));
    CHECK_TRUE(link_str != NULL);

    len = readlink(path, link_str, PATH_MAX + 1);

    CHECK_TRUE(len != -1 || errno == NO_SUCH_FILE_OR_DIRECTORY);

    if (errno == NO_SUCH_FILE_OR_DIRECTORY) {
        free(link_str);
        link_str = NULL;
    }

exit:
    if (err == ERROR_OCCURRED) {
        free(link_str);
    }

    *out = link_str;
    return err;
}

ERROR_STATUS read_file(int fd, char* buffer, uint64_t size, uint64_t* read_size) {
    ERROR_STATUS err = NO_ERROR_OCCURRED;    
    uint64_t num_read = 0;
    uint64_t current_num_read = 0;

    while (num_read < size) {
        current_num_read = read(fd, buffer + num_read, size - num_read);        
        CHECK_TRUE(current_num_read != -1);

        if (num_read == 0) {
            break;
        }
        
        num_read += current_num_read;
    }

exit:
    *read_size = num_read;
    return err;
}

char* join_path(const char* a, const char* b) {
    ERROR_STATUS err = NO_ERROR_OCCURRED;
    char* concat = NULL;
    
    CHECK_TRUE(a != NULL && b != NULL);
   
    size_t a_len = strnlen(a, PATH_MAX);
    size_t b_len = strnlen(b, PATH_MAX); 
    size_t total_len = a_len + b_len + 1;
    
    concat = calloc(total_len + 1, sizeof(char));
    CHECK_TRUE(concat != NULL);

    memcpy(concat, a, a_len);
    strncat(concat, "/", 1);
    strncat(concat, b, b_len);
    
exit:
    if (err == ERROR_OCCURRED) {
        free(concat);
        return NULL;
    }

    return concat;
}

char* convert_null_to_empty(char* in) {
    return (in == NULL) ? EMPTY_STR : in;
}

