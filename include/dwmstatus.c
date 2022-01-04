#include <X11/Xlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Display *dpy;

char *mktimes(size_t size, char *buf) {
  time_t s = time(NULL);
  struct tm *tms = localtime(&s);

  strftime(buf, size, "%H:%M %a %b %d", tms);
  return buf;
}

int main(void) {
  Display *dpy = XOpenDisplay(NULL);
  if(
}
