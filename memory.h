#include <signal.h>

extern volatile sig_atomic_t gotMemorySignal;

void memoryUsage();
