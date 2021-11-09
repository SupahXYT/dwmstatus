#include "hardware.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int capacity(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/capacity", "r");
  char buf[4];
  int capacity = atoi(fgets(buf, sizeof(buf), sys));

  fclose(sys);
  return capacity;
}

bool charging(void) {
  FILE *sys = fopen("/sys/class/power_supply/BAT0/status", "r");
  char buf[10];

  char *status = fgets(buf, sizeof(buf), sys);
  fclose(sys);
  return strcmp(status, "Charging\n") == 0 || strcmp(status, "Full\n") == 0;
}

int signal() {
  FILE *proc = fopen("/proc/net/wireless", "r");
  if (proc == NULL) {
    return 0;
  }

  char buf[82];

  fgets(buf, 82, proc);
  fgets(buf, 82, proc);
  fseek(proc, 20, SEEK_CUR);
  fgets(buf, 4, proc);

  fclose(proc);
  return atoi(buf);
}

long double cpu() {
  long double a[4], b[4], loadavg;
  FILE *fp;

  fp = fopen("/proc/stat", "r");
  fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3]);
  fclose(fp);
  sleep(1);

  fp = fopen("/proc/stat", "r");
  fscanf(fp, "%*s %Lf %Lf %Lf %Lf", &b[0], &b[1], &b[2], &b[3]);
  fclose(fp);

  loadavg = (((b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2])) /
             ((b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3])) * 100);

  return (loadavg);
}
