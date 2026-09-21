#ifndef PID_H
#define PID_H

#include <sys/types.h>

typedef struct ProcessInfo {
    pid_t pid;
    char name[256];
    long rss_kb;
} ProcessInfo;

int is_pid(const char *name);
int collect_processes(ProcessInfo *procs, int max_procs);
void print_processes(ProcessInfo *procs, int count, int max_rows);

#endif
