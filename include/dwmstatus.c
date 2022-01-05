
/* dwmstatus is a minimal status bar for dwm.
 * Copyright (C) 2022 SupahX
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include "hardware.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern char *cpu_icon;

char *mktimesn(char *buf, size_t n) {
  time_t t = time(NULL);
  struct tm *time = localtime(&t);

  strftime(buf, n, "%H:%M %a %b %d", time);
  return buf;
}

int main(int argc, char *argv[]) {
  Display *dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "dwmstatus: cannot open display\n");
    exit(1);
  }

  WirelessDevice INTERFACE;
  Battery battery;
  char status[128];
  float loadavg;
  char time[32];

  while (1) {
    mktimesn(time, sizeof(time));
    battery = get_battery();
    loadavg = cpu_load();

    sprintf(status, "[%s %d%%][%s %0.2f%%] %s ", battery.icon, battery.capacity,
            cpu_icon, loadavg, time);
    XStoreName(dpy, DefaultRootWindow(dpy), status);
    XSync(dpy, 0);
  }
}
