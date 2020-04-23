#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <linux/limits.h>
#include "check_exceptions.h"
#include "utils.h"

#define DIR_NAME "/proc"

int main() {
    ERROR_STATUS err = NO_ERROR_OCCURRED;
    DIR* dir = NULL;
    struct dirent* entry = NULL;
    unsigned long pid = 0;
    char* entry_name = NULL;
    char* proc_entry_path = NULL;
    char* comm_path = NULL;
    char* cmdline_path = NULL;
    char* cwd_path = NULL;
    char* binary_name_path = NULL;
    char* process_name = NULL;
    char* cmd_line = NULL;
    char* cwd = NULL;
    char* binary_path = NULL;

    dir = opendir(DIR_NAME);
    CHECK_TRUE(dir != NULL);
    
    printf("process name, PID, cwd, binary path, command line\n\n");

    // iterate through all the files in the directory
    while ((entry = readdir(dir)) != NULL) {
        entry_name = entry->d_name;
        
        // if the entry is not a process directory
        if (!convert_to_ul(entry_name, strnlen(entry_name, PATH_MAX), &pid)) {
            continue;
        }
        
        // get paths to needed resources
        proc_entry_path = join_path(DIR_NAME, entry_name);
        comm_path = join_path(proc_entry_path, "comm");
        cmdline_path = join_path(proc_entry_path, "cmdline");
        cwd_path = join_path(proc_entry_path, "cwd");
        binary_name_path = join_path(proc_entry_path, "exe");

        CHECK_TRUE(proc_entry_path != NULL);
        CHECK_TRUE(comm_path != NULL);
        CHECK_TRUE(cmdline_path != NULL);        
        CHECK_TRUE(cwd_path != NULL);
        CHECK_TRUE(binary_name_path != NULL);

        // read the needed resources
        CHECK_AND_PASS(read_string_from_file(comm_path, &process_name));
        CHECK_AND_PASS(read_string_from_file(cmdline_path, &cmd_line));
        CHECK_AND_PASS(read_link_path(cwd_path, &cwd));
        CHECK_AND_PASS(read_link_path(binary_name_path, &binary_path));

        // print formated output after converting NULL strings
        printf("%s %lu %s %s %s\n\n", convert_null_to_empty(process_name), pid, convert_null_to_empty(cwd), convert_null_to_empty(binary_path), convert_null_to_empty(cmd_line));        

        // free all allocated memory
        free(proc_entry_path);
        free(comm_path);
        free(cmdline_path);
        free(cwd_path);
        free(binary_name_path);
        free(process_name);
        free(cmd_line);
        free(cwd);
        free(binary_path);

        // prevent a possible double-free
        proc_entry_path = NULL;
        comm_path = NULL;
        cmdline_path = NULL;
        cwd_path = NULL;
        binary_name_path = NULL;
        process_name = NULL;
        cmd_line = NULL;
        cwd = NULL;
        binary_path = NULL;
    }    

exit:
    if (dir != NULL) {
        closedir(dir);
    }

    free(proc_entry_path);
    free(comm_path);
    free(cmdline_path);
    free(cwd_path);
    free(binary_name_path);
    free(process_name);
    free(cmd_line);
    free(cwd);
    free(binary_path);

    if (err == ERROR_OCCURRED) {
        printf("\nError occurred\n");
    }

    return err;
}

