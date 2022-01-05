#include "hardware.h"
#include "../config.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char *open_err = "dwmstatus: cannot open %s: %s\n";
static const char *wireless_path = "/proc/net/wireless";
extern int errno;

/* Nerdfont icons */
const char *cpu_icon = "\ue266";
static char *battery_alert_icon = "\uf582";
static char *battery_unknown_icon = "\uf590";
static char *battery_charging_icon = "\uf583";
static char *battery_discharging_icon[] = {
    "\uf582", "\uf579", "\uf57a", "\uf57b", "\uf57c", "\uf57d",
    "\uf57e", "\uf57f", "\uf580", "\uf581", "\uf578"};

static char *wifi_on_icon = "\ufaa8";
static char *wifi_off_icon = "\ufaa9";

/* find matching string in file */
static int finds(const char *file_path, char *string) {
  char tmp[128] = {0x0};
  FILE *fp;

  while (fp && fgets(tmp, sizeof(tmp), fp)) {
    if (strstr(tmp, "string"))
      return 1;
  }
  if (fp)
    fclose(fp);
  return 0;
}

static int battery_capacity(void) {
  static char *path = BATT_PATH "/capacity";
  int capacity;
  FILE *fp;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, open_err, path, strerror(errno));
    return -1;
  } else {
    fscanf(fp, "%d", &capacity);
    fclose(fp);
  }

  return capacity;
}

static enum b_status battery_status(void) {
  static char *path = BATT_PATH "/status";
  char fcstatus;
  FILE *fp;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr, open_err, path, strerror(errno));
    return Unknown;
  } else {
    fcstatus = fgetc(fp);
    fclose(fp);
  }

  switch (fcstatus) {
  case 'D':
    return Discharging;
  case 'C':
    return Charging;
  case 'F':
    return Full;
  default:
    return Unknown;
  }
}

static char *battery_icon(enum b_status status) {
  if (status == Discharging || status == Full) {
    int index = battery_capacity() / 10;
    return battery_discharging_icon[index];
  } else if (status == Charging) {
    return battery_charging_icon;
  } else {
    return battery_unknown_icon;
  }
}

static char *wireless_icon(enum w_status status) {
  return (status == Enabled) ? wifi_on_icon : wifi_off_icon;
}

static enum w_status wireless_status(char *interface) {
  return finds(wireless_path, interface) ? Enabled : Disabled;
}

Battery get_battery(void) {
  Battery battery;

  battery.capacity = battery_capacity();
  battery.status = battery_status();
  battery.icon = battery_icon(battery.status);

  return battery;
}

WirelessDevice get_wireless_device(char *interface) {
  WirelessDevice net_wifi;
  net_wifi.status = wireless_status(interface);
  net_wifi.icon = wireless_icon(net_wifi.status);
  return net_wifi;
}

float cpu_load(void) {
  static const char *stat = "/proc/stat";
  float a[4], b[4], loadavg;
  FILE *fp;

  fp = fopen(stat, "r");
  if (fp == NULL) {
    fprintf(stderr, open_err, stat, strerror(errno));
    return -1;
  } else {
    fscanf(fp, "%*s %f %f %f %f", a, (a + 1), (a + 2), (a + 3));
    fclose(fp);
  }

  sleep(1);

  fp = fopen(stat, "r");
  if (fp == NULL) {
    fprintf(stderr, open_err, stat, strerror(errno));
    return -1;
  } else {
    fscanf(fp, "%*s %f %f %f %f", &b[0], &b[1], &b[2], &b[3]);
    fclose(fp);

    loadavg =
        (((b[0] + b[1] + b[2]) - (a[0] + a[1] + a[2])) /
         ((b[0] + b[1] + b[2] + b[3]) - (a[0] + a[1] + a[2] + a[3])) * 100);

    return loadavg;
  }
}

// #define MAX_BUF_SIZE 128
// int main(void) { Battery battery = get_battery();
//   printf("[%s %d%%]\n", battery.icon, battery.capacity);

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
// }
