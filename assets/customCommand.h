#include <signal.h>
#include <time.h>

extern volatile sig_atomic_t gotCustomCommandSignal;

typedef struct {
    const char* command;
    int interval;
    char output[256];
    time_t lastUpdate;
} CustomCommand;

char* executeCustomCommand(const char* command, char* output, size_t maxLen);
