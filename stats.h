#include <stdio.h>
#ifndef STATS_H
#define STATS_H

typedef struct CoreTimes {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
} CoreTimes;

int read_core_snapshots(FILE *f, CoreTimes *snapshots, size_t core_count);
double calculate_core_usage(const CoreTimes *prev, const CoreTimes *curr);
void aggregate_cores(const CoreTimes *cores, size_t core_count, CoreTimes *out);

#endif