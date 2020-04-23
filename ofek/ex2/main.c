#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include "check_exceptions.h"

#define SLEEP_BIN_DIR_PATH "/bin"
#define BUFF_SIZE (512 * sizeof(struct inotify_event))
#define BIN_NAME "sleep"
#define MASKS_COUNT (10)

int main() {
    ERROR_STATUS err = NO_ERROR_OCCURRED;
    int fd = -1;
    int wd = -1;
    uint32_t monitor_mask = IN_ACCESS | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | 
                        IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_OPEN;
    struct inotify_event* curr_event;    
    size_t read_size = 0;
    size_t i = 0;
    char buffer[BUFF_SIZE];
    uint32_t masks[] = {IN_ACCESS, IN_ATTRIB, IN_CLOSE_WRITE, IN_CLOSE_NOWRITE, 
                        IN_DELETE, IN_DELETE_SELF, IN_MODIFY, IN_MOVE_SELF, IN_MOVED_FROM, IN_OPEN};
    // events[k] contains a description of the event matching masks[k]    
    char* events[] = {"accessed", "changed", "closed", "closed", "deleted", "deleted", "modified", "moved", "moved", "opened"};    
    size_t j = 0;
 
    fd = inotify_init();
    CHECK_TRUE(fd != -1);

    wd = inotify_add_watch(fd, SLEEP_BIN_DIR_PATH, monitor_mask);
    CHECK_TRUE(wd != -1);

    printf("Monitoring events of %s/%s:\n", SLEEP_BIN_DIR_PATH, BIN_NAME);

    // main loop - keep monitoring events
    while (true) {
        // wait for new events
        read_size = read(fd, buffer, BUFF_SIZE);
        
        // iterate through all the new events
        i = 0;
        while (i < read_size) {
            curr_event = (struct inotify_event*)(&buffer[i]);
            
            if (curr_event->len > 0) {
                if(strncmp(curr_event->name, BIN_NAME, strlen(BIN_NAME)) == 0) {
                    // if an event occurred, print a descriptive message
                    for (j = 0; j < MASKS_COUNT; j++) {
                        if (curr_event->mask & masks[j]) {
                            printf("The file was %s\n", events[j]);
                        }
                    }

                }

            }

            // advance to the next event
            i += (sizeof(struct inotify_event) + curr_event->len);   
        }
    
    }

exit:
    if (wd != -1) {
        inotify_rm_watch(fd, wd);
    }

    if (fd != -1) {
        close(fd);
    }

    return err;

}

