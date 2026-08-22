#ifndef ASSETS_CUSTOMCOMMAND_H
#define ASSETS_CUSTOMCOMMAND_H

#include <time.h>
#include <stddef.h>

typedef struct {
    const char* command;
    int interval;
    char output[256];
    time_t lastUpdate;
    int signal;
    int state;
} CustomCommand;

char* executeCustomCommand(const char* command, char* output, size_t maxLen);

#endif
