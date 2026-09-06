#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>

int is_pid(const char *name) {
    while (*name) {
        if (!isdigit(*name)) {
            return 0;
        }
        name++;
    }
    return 1;
}

void read_processes(void) {
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
    perror("opendir");
    return;
}
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (is_pid(entry->d_name)) {
            printf("Found PID: %s\n", entry->d_name);
        }
    }

    closedir(dir);
}
