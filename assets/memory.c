#include <stdio.h>
#include <stdint.h>
#include <signal.h>

volatile sig_atomic_t gotMemorySignal = 0;

typedef struct memory {
  uintmax_t total;
  uintmax_t available;
} Mem;

Mem m = {0};

static void getMemStats() {
  FILE *f = fopen("/proc/meminfo", "r");
  if (!f) {
    perror("Could not open /proc/meminfo");
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
  gotMemorySignal = 0;
  return (double)(((double)m.total - m.available) / m.total) * 100.0;
}
