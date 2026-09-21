#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "stats.h"
#include "memory.h"
#include "pid.h"

#define MAX_PROCESSES 512
#define VISIBLE_ROWS 15

int main(void) {

    ProcessInfo procs[MAX_PROCESSES];

   long core_count_long = sysconf(_SC_NPROCESSORS_ONLN);
    size_t core_count;

    if (core_count_long <= 0) {
        fprintf(stderr, "Failed to detect CPU cores\n");
        return 1;
    }

    core_count = (size_t)core_count_long;

    CoreTimes *prev = calloc(core_count, sizeof(*prev));
    CoreTimes *curr = calloc(core_count, sizeof(*curr));
    FILE *f;

    if (prev == NULL || curr == NULL) {
        free(prev);
        free(curr);
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    f = fopen("/proc/stat", "r");
    if (f == NULL) {
        fprintf(stderr, "Failed to open /proc/stat\n");
        free(prev);
        free(curr);
        return 1;
    }

    if (read_core_snapshots(f, prev, core_count) != 0) {
    fprintf(stderr, "Failed to read initial CPU snapshots\n");
    fclose(f);
    free(prev);
    free(curr);
    return 1;
}
    while (1) {
    sleep(1);
    
    if (read_core_snapshots(f, curr, core_count) != 0) {
        fprintf(stderr, "Failed to read CPU snapshots\n");
        break;
    }

    printf("\033[H\033[J");

    CoreTimes prev_agg, curr_agg;
    aggregate_cores(prev, core_count, &prev_agg);
    aggregate_cores(curr, core_count, &curr_agg);
    printf("CPU: %.2f%%\n", calculate_core_usage(&prev_agg, &curr_agg));

    memory_amount();

    for (size_t i = 0; i < core_count; i++)
        printf("Core %zu: %.2f%%\n", i, calculate_core_usage(&prev[i], &curr[i]));

    int proc_count = collect_processes(procs, MAX_PROCESSES);
    if (proc_count > 0) {
        print_processes(procs, proc_count, VISIBLE_ROWS);
    }

    CoreTimes *tmp = prev;
    prev = curr;
    curr = tmp;
}

fclose(f);
free(prev);
free(curr);
return 0;
    
}