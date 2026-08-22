#include "assets/battery.h"
#include "assets/cpu.h"
#include "assets/customCommand.h"
#include "assets/memory.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void startSignals();
void signalHandler(int signal);

size_t numCommands = sizeof(modules) / sizeof(modules[0]);
size_t numCustomCommands = sizeof(customCommands) / sizeof(customCommands[0]);

int main() {
  startSignals();

  for (int i = 0; i < numCommands; i++) {
    modules[i].lastUpdate = 0;
    modules[i].output[0] = '\0';
    modules[i].state = 0;
  }

  for (int j = 0; j < numCustomCommands; j++) {
    customCommands[j].lastUpdate = 0;
    customCommands[j].output[0] = '\0';
    customCommands[j].state = 0;
  }

  int flag = 1;
  time_t lastTime = 0;

  while (1) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    for (int i = 0; i < numCommands; i++) {
      if (modules[i].state ||
          now - modules[i].lastUpdate >= modules[i].interval) {
        double value = 0.0;
        
        if (strcmp(modules[i].module, "cpu") == 0) {
          value = cpuUsage();
        } else if (strcmp(modules[i].module, "ram") == 0) {
          value = memoryUsage();
        } else if (strcmp(modules[i].module, "battery") == 0) {
          value = batteryPerc(modules[i].arg);
        }
        
        snprintf(modules[i].output, sizeof(modules[i].output), "%.2lf%%", value);
        modules[i].lastUpdate = now;
        modules[i].state = 0;
        flag = 1;
      }
    }

    for (int i = 0; i < numCustomCommands; i++) {
      if (customCommands[i].state ||
          now - customCommands[i].lastUpdate >= customCommands[i].interval) {
        executeCustomCommand(customCommands[i].command,
                             customCommands[i].output,
                             sizeof(customCommands[i].output));
        customCommands[i].lastUpdate = now;
        customCommands[i].state = 0;
        flag = 1;
      }
    }

    char dateTime[256];
    if (now - lastTime >= 1) {
      strftime(dateTime, sizeof(dateTime), "%a, %b %d | %H:%M:%S", t);
      lastTime = now;
      flag = 1;
    }

    if (flag) {
      printf(formatting, formattingArgs);
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
  
  for (int i = 0; i < numCommands; i++) {
    sigaction(modules[i].signal, &sa, NULL);
  }
  for (int i = 0; i < numCustomCommands; i++) {
    sigaction(customCommands[i].signal, &sa, NULL); 
  }
}

void signalHandler(int signal) {
  for (int i = 0; i < numCommands; i++) {
    if (signal == modules[i].signal) {
      modules[i].state = 1;
    }
  }
  for (int i = 0; i < numCustomCommands; i++) {
    if (signal == customCommands[i].signal) {
      customCommands[i].state = 1;
    }
  }
}
