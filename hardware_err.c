#include "hardware_err.h"
#include <stdio.h>
#include <stdlib.h>

char *capacity(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/capacity", "r");

  if (sys == NULL) {
    return "n/a";
  }
  char *cs = fgets(malloc(4), 4, sys);
  fclose(sys);
  return cs;
}

enum status _charging(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/status", "r");
  char c = getc(sys);
  fclose(sys);

  switch (c) {
  case 'C':
    return CHARGING;
  case 'D':
    return DISCHARGING;
  default:
    return UNKNOWN;
  }
}

int main(void) {
  printf("charging state: %d\n", _charging());
  //
}
