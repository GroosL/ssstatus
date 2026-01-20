#include "assets/cpu.h"
#include "assets/customCommand.h"
#include "assets/memory.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void startSignals();
void signalHandler(int signal);

int main() {
  startSignals();

  CustomCommand customCommands[] = {
      {.command = "sb-volume", .interval = 500},
      {.command = "sb-forecast", .interval = 3600},
      {.command = "sb-phone-battery", .interval = 300},
  };

  int numCustomCommands = sizeof(customCommands) / sizeof(customCommands[0]);

  for (int i = 0; i < numCustomCommands; i++) {
    customCommands[i].lastUpdate = 0;
    customCommands[i].output[0] = '\0';
  }

  int flag = 1;
  time_t lastCpu = 0;
  time_t lastMem = 0;
  time_t lastTime = 0;

  double cpuPerc = 0;
  double memPerc = 0;

  while (1) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (gotCpuSignal || now - lastCpu >= 5) {
      cpuPerc = cpuUsage();
      lastCpu = now;
      flag = 1;
    }

    if (gotMemorySignal || now - lastMem >= 5) {
      memPerc = memoryUsage();
      lastMem = now;
      flag = 1;
    }

    for (int i = 0; i < numCustomCommands; i++) {
      if (gotCustomCommandSignal ||
          now - customCommands[i].lastUpdate >= customCommands[i].interval) {
        executeCustomCommand(customCommands[i].command,
                             customCommands[i].output,
                             sizeof(customCommands[i].output));
        customCommands[i].lastUpdate = now;
        flag = 1;
      }
    }
    if (gotCustomCommandSignal) {
      gotCustomCommandSignal = 0;
    }

    char dateTime[256];
    if (now - lastTime >= 1) {
      strftime(dateTime, sizeof(dateTime), "%a, %b %d | %H:%M:%S", t);
      lastTime = now;
      flag = 1;
    }

    if (flag) {
      // char customOutput[1024] = "";
      // for (int i = 0; i < numCustomCommands; i++) {
      //   if (strlen(customCommands[i].output) > 0) {
      //     if (strlen(customOutput) > 0) {
      //       strcat(customOutput, " | ");
      //     }
      //     strcat(customOutput, customCommands[i].output);
      //   }
      // }
      if (numCustomCommands > 0) {
        printf("| %s | %s | %s | RAM: %.2lf%% | CPU: %.2lf%% | %s\n", customCommands[0].output, customCommands[1].output, customCommands[2].output, memPerc, cpuPerc, dateTime);
      }
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
  sigaction(SIGRTMIN, &sa, NULL);
}

void signalHandler(int signal) {
  if (signal == SIGUSR1) {
    gotCpuSignal = 1;
  } else if (signal == SIGUSR2) {
    gotMemorySignal = 1;
  } else if (signal == SIGRTMIN) {
    gotCustomCommandSignal = 1;
  }
}
