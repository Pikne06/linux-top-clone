# htopclone
## Build

**Requirements**
- GCC
- Linux (reads from /proc, won't work on Mac or Windows)

**Compile**
```bash
gcc -Wall -Wextra -o htopclone main.c stats.c memory.c pid.c
```

**Run**
```bash
./htopclone
```

## Future plans
- Add PID lists, and possibility to sort them by usage % (RAM %, CPU %, swap %, gpu %)
- Add GPU usage for either AMD / Nvidia or both.
- Add swap usage
- Add disk storage availibility / usage (% too)
- Possibly to add network monitoring
- Better usability and the ability to only show things you're interested in
