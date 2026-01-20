#include "cpu.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t gotCpuSignal = 0;

double cpuUsage() {
  gotCpuSignal = 0;
  FILE *f = fopen("/proc/stat", "r");
  static double a[7];
  double b[7], sum;

  memcpy(b, a, sizeof(b));
  if (fscanf(f, "cpu %lf %lf %lf %lf %lf %lf %lf", &a[0], &a[1], &a[2], &a[3],
             &a[4], &a[5], &a[6]) != 7)
    return -1;
  fclose(f);

  sum = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
        (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);

  if (b[0] == 0)
    return 0;

  if (sum == 0)
    return 0;

  return (double)((100 *
                   ((b[0] + b[1] + b[2] + b[5] + b[6]) -
                    (a[0] + a[1] + a[2] + a[5] + a[6])) /
                   sum));
}
