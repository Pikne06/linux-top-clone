#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include "pid.h"

int is_pid(const char *name) {
    while (*name) {
        if (!isdigit((unsigned char)*name)) {
            return 0;
        }
        name++;
    }
    return 1;
}

/* Fills in one ProcessInfo from /proc/<pid>/status.
 * Returns -1 if the file couldn't be opened -- this is NORMAL, not an
 * error worth reporting: a process can exit between readdir() handing
 * us its PID and us opening its status file a moment later. */
static int read_process_status(pid_t pid, ProcessInfo *info) {
    char path[64];
    char line[256];

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return -1;
    }

    info->pid = pid;
    info->name[0] = '\0';
    info->rss_kb = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Name:", 5) == 0) {
            sscanf(line, "Name:\t%255[^\n]", info->name);
        } else if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS:\t%ld kB", &info->rss_kb);
        }
    }

    fclose(f);
    return 0;
}

/* Walks /proc once, filling procs[] with up to max_procs entries.
 * Returns how many were actually filled in. */
int collect_processes(ProcessInfo *procs, int max_procs) {
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    int count = 0;

    while (count < max_procs && (entry = readdir(dir)) != NULL) {
        if (!is_pid(entry->d_name)) {
            continue;
        }
        pid_t pid = (pid_t)atoi(entry->d_name);
        if (read_process_status(pid, &procs[count]) == 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

static int compare_by_rss_desc(const void *a, const void *b) {
    const ProcessInfo *pa = a;
    const ProcessInfo *pb = b;
    if (pb->rss_kb != pa->rss_kb) {
        return (pb->rss_kb > pa->rss_kb) ? 1 : -1;
    }
    return 0;
}

/* Sorts procs[] by memory usage (heaviest first) and prints only the
 * top max_rows entries -- this is the piece that was missing before:
 * without a cap, dumping every PID on the system is what made the
 * screen scroll forever instead of looking like a fixed "frame". */
void print_processes(ProcessInfo *procs, int count, int max_rows) {
    qsort(procs, count, sizeof(ProcessInfo), compare_by_rss_desc);

    int rows = (count < max_rows) ? count : max_rows;

    printf("  PID  RSS(MB)  NAME\n");
    for (int i = 0; i < rows; i++) {
        printf("%5d  %7.1f  %s\n", procs[i].pid, procs[i].rss_kb / 1024.0, procs[i].name);
    }
}
