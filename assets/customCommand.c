#include "customCommand.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile sig_atomic_t gotCustomCommandSignal = 0;

char *executeCustomCommand(const char *command, char *output, size_t maxLen) {
  memset(output, 0, maxLen);

  FILE *pipe = popen(command, "r");

  char buff[256];
  size_t currentLen = 0;

  while (fgets(buff, sizeof(buff), pipe) != NULL &&
         currentLen < maxLen - 1) {
    size_t len = strlen(buff);
    if (len > 0 && buff[len - 1] == '\n') {
      buff[len - 1] = '\0';
      len--;
    }
    if (len > 0 && buff[len - 1] == '\r') {
      buff[len - 1] = '\0';
      len--;
    }

    size_t spaceLeft = maxLen - currentLen - 1;
    if (spaceLeft > 0) {
      if (currentLen > 0 && spaceLeft > 1) {
        strncat(output, " ", spaceLeft);
        currentLen++;
        spaceLeft--;
      }
      strncat(output, buff, spaceLeft);
      currentLen += strlen(buff);
    }
  }

  pclose(pipe);

  if (currentLen == 0) {
    snprintf(output, maxLen, "");
  }

  return output;
}
