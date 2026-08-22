#include "battery.h"
#include <stdio.h>

double batteryPerc(const char* bat) {
  char path[50];
  snprintf(path, sizeof(path), "/sys/class/power_supply/%s/capacity", bat);

  double batPerc = 0.0;

  FILE *f = fopen(path, "r");
  if (f) {
    if (fscanf(f, "%lf", &batPerc) != 1) {
      batPerc = 0.0;
    }
    fclose(f);
  }

  return batPerc;
}
