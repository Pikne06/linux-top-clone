#include <stdio.h>
#include <stdlib.h>


int memory_amount() {
    
    char line[256];
    long mem_total = 0;
    long mem_available = 0;

    FILE *f = fopen("/proc/meminfo", "r");
    if (f == NULL) {
        perror("Failed to open meminfo");
        return 1;
    }

    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, "MemTotal: %ld", &mem_total) == 1) {
            printf("Total Memory: %.1f MB\n", mem_total / 1024.0);
        }
        if (sscanf(line, "MemAvailable: %ld", &mem_available) == 1) {
            printf("Available Memory: %.1f MB\n", mem_available / 1024.0);
        } 
    }   
        if(mem_total >0) {
        long used_memory = mem_total - mem_available;
        double used_memory_procentage = (used_memory * 100.0) / mem_total;
        printf("Used Memory: %.1f MB\n", used_memory / 1024.0);
        printf("Used Memory Percentage: %.2f%%\n", used_memory_procentage);

        } else {
            printf("Error: Total memory is zero or negative.\n");
            fclose(f);
            return 1;
        }

        
        fclose(f);
        return 0;
    }


