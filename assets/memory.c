#include "memory.h"
#include <stdio.h>
#include <stdint.h>

typedef struct memory {
  uintmax_t total;
  uintmax_t available;
} Mem;

Mem m = {0};

static void getMemStats() {
  FILE *f = fopen("/proc/meminfo", "r");
  if (!f) {
    perror("Could not open /proc/meminfo");
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    if (sscanf(line, "MemTotal: %ju kB", &m.total) == 1) 
      continue;
    if (sscanf(line, "MemAvailable: %ju kB", &m.available) == 1)
      break;
  }
  fclose(f);
}

double memoryUsage() {
  getMemStats();
  if (m.total == 0)
    return 0;
  return (double)(((double)m.total - m.available) / m.total) * 100.0;
}
