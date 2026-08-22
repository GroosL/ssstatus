#ifndef ASSETS_COMMANDS_H
#define ASSETS_COMMANDS_H

#include <time.h>

typedef struct {
    const char* module;
    const char* arg;

    char output[256];
    time_t lastUpdate;
    int interval;
    int signal;
    int state;
} Commands;

#endif
