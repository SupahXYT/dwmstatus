#include "hardware.h"
#include "../config.h"
#include <stdio.h>

/* Nerdfont icons */
static char *battery_alert = "\uf582";
static char *battery_unknown = "\uf590";
static char *battery_charging = "\uf583";
static char *battery_discharging[] = {"\uf582", "\uf579", "\uf57a", "\uf57b",
                                      "\uf57c", "\uf57d", "\uf57e", "\uf57f",
                                      "\uf580", "\uf581", "\uf578"};

static char *wifi_on = "\ufaa8";
static char *wifi_off = "\ufaa9";

static int battery_capacity(void) {
  static char *path = BATT_PATH "/capacity";
  int capacity;
  FILE *fp;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file at %s:\n", path);
    return -1;
  } else {
    fscanf(fp, "%d", &capacity);
    fclose(fp);
  }

  return capacity;
}

static enum BatteryStatus battery_status(void) {
  static char *path = BATT_PATH "/status";
  char fcstatus;
  FILE *fp;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file at %s: \n", path);
    return Unknown;
  } else {
    fcstatus = fgetc(fp);
    fclose(fp);
  }

  // printf("status: %c\n", fcstatus);
  switch (fcstatus) {
  case 'D':
    // printf("status returned discharging\n");
    return Discharging;
  case 'C':
    // printf("status returned charging\n");
    return Charging;
  case 'F':
    // printf("status returned full\n");
    return Full;
  default:
    // printf("status returned unknown\n");
    return Unknown;
  }
}

static char *battery_icon(enum BatteryStatus status) {
  if (status == Discharging || status == Full) {
    // printf("icon set to indexed\n");
    int index = battery_capacity() / 10;
    return battery_discharging[index];
  } else if (status == Charging) {
    // printf("icon set charging\n");
    return battery_charging;
  } else {
    // printf("icon set unknown\n");
    return battery_unknown;
  }
}

Battery get_battery(void) {
  Battery battery;

  battery.capacity = battery_capacity();
  battery.status = battery_status();
  battery.icon = battery_icon(battery.status);

  return battery;
}

// #define MAX_BUF_SIZE 128
int main(void) {
  Battery battery = get_battery();
  printf("[%s %d%%]\n", battery.icon, battery.capacity);

  // WirelessDevice dev = get_wireless_device(NET_WIFI);
  // printf("%s stats:\n"
  //        "status:\t%d\n"
  //        "signal:\t%d\n"
  //        "icon:\t%s\n",
  //        NET_WIFI, dev.status, dev.signal, dev.icon);

  // int bol = 0;
  // if ((bol = 1)) {
  //   printf("True\n");
  // }

  // FILE *f = fopen("wireless", "r");

  // int a, b;
  // char buf[MAX_BUF_SIZE];
  // // fgets(buf, MAX_BUF_SIZE, f);
  // // fseek(f, 20, SEEK_CUR);
  // int no = 211;
  // // fgets(buf, 4, f);
  // // fscanf(f, "%d", &no);

  // // fgets(buf, MAX_BUF_SIZE, f);
  // // fgets(buf, MAX_BUF_SIZE, f);
  // // fscanf(f, "%s", buf);
  // fscanf(f, "%[^\n]%*[^\n]", buf);
  // printf("%s\n", buf);
  // // fscanf(f, "%[^\n] ", buf);
  // fscanf(f, "%*s %*s %s ", buf);
  // // fscanf(f, "%s %d", buf, &no);
  // printf("%s, %d\n", buf, no);
  // fclose(f);
}
