#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "cpu.h"
#include "memory.h"

void startSignals();
void signalHandler(int signal);

int main(int argc, char *argv[]) {
  startSignals();

  int flag = 1;
  time_t lastCpu = 0;
  time_t lastTime = 0;
  
  double cpuPerc = 0;
  while (1) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (gotCpuSignal || now - lastCpu >= 5) {
      cpuPerc = cpuUsage(); 
      lastCpu = now;
      flag = 1;
    }

    if (gotMemorySignal) {
      memoryUsage();
      flag = 1;
    }
    
    char dateTime[256];
    if (now - lastTime >= 1) {
      strftime(dateTime, sizeof(dateTime), "%a, %b %d | %H:%M:%S", t);
      lastTime = now;
      flag = 1;
    }
    
    if (flag) {
      printf(" CPU: %.2lf%% | %s \n", cpuPerc, dateTime);
      fflush(stdout);
      flag = 0;
    }

    usleep(2000);
  }
  return 0;
}

void startSignals() {
  struct sigaction sa = {0};
  sa.sa_handler = signalHandler;

  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);
}

void signalHandler(int signal) {
  if (signal == SIGUSR1) {
    gotCpuSignal = 1;
  }
  else if (signal == SIGUSR2) {
    gotMemorySignal = 1;
  }
}
