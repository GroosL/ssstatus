#include "assets/customCommand.h"
#include "assets/commands.h"
#include <signal.h>

static CustomCommand customCommands[] = {
  {.command = "echo 'customCommand!!'", .interval = 500, .signal = 34},
};

static Commands modules[] = {
  {.module = "cpu", .interval = 5, .signal = SIGUSR1},
  {.module = "ram", .interval = 5, .signal = SIGUSR2},
  {.module = "battery", .arg = "BAT0", .interval = 15, .signal = SIGPWR},
};

static const char* formatting = "%s | %s | %s | BAT: %s | RAM: %s | CPU: %s | %s\n";
#define formattingArgs customCommands[0].output, customCommands[1].output, customCommands[2].output, modules[2].output, modules[1].output, modules[0].output, dateTime
