#include "assets/customCommand.h"

static CustomCommand customCommands[] = {
  {.command = "sb-volume", .interval = 500, .signal = 34},
  {.command = "sb-forecast", .interval = 3600, .signal = 34},
  {.command = "sb-phone-battery", .interval = 300, .signal = 34},
};
