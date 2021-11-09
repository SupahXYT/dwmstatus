#include "hardware_err.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

char *capacity(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/capacity", "r");

  if (sys == NULL) {
    return "N/A";
  }

  return fgets(malloc(4), 4, sys);
}

enum status _charging(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/capacity", "r");

  switch (getc(sys)) {
  case 'C':
    return CHARGING;
  default:
    return UNKNOWN;
  }
}
