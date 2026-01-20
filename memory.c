#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "cpu.h"

volatile sig_atomic_t gotMemorySignal = 0;

void memoryUsage() {
  printf("MemorySIGNAL\n");
  gotMemorySignal = 0;
}
