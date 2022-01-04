#include "hardware.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern char *cpu_icon;

char *mktimesn(char *buf, size_t n) {
  time_t s = time(NULL);
  struct tm *tms = localtime(&s);

  strftime(buf, n, "%H:%M %a %b %d", tms);
  return buf;
}

int main(void) {
  Display *dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  WirelessDevice wlan0;
  Battery battery;
  char status[200];
  float loadavg;
  char time[32];

  while (1) {
    mktimesn(time, sizeof(time));
    battery = get_battery();
    loadavg = cpu();

    sprintf(status, "[%s %d%%][%s %0.2f%%] %s ", battery.icon, battery.capacity,
            time, loadavg, cpu_icon);
    XStoreName(dpy, DefaultRootWindow(dpy), status);
    XSync(dpy, 0);

    printf("%f\n", cpu());
  }
}
