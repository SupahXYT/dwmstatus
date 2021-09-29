#include "hardware.h"
#include <X11/Xlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static Display *dpy;

char *mktimes() {
  time_t tim = time(NULL);
  struct tm *tmp = localtime(&tim);
  char *buf = calloc(20, sizeof(char));

  strftime(buf, 20, "%H:%M %a %b %d", tmp);
  return buf;
}

char *mksigs() {
  char *string = calloc(10, sizeof(char));

  if (signal() < 0) {
    char buf[4];
    sprintf(buf, "%d", signal());
    strcat(string, "\ufaa8");
    strcat(string, buf);
    strcat(string, " dBm");
  } else {
    strcat(string, "\ufaa9");
  }

  return string;
}

char *baticon(void) {

  static const char icons[] = "\uf58d\uf579\uf57a\uf57b\uf57c\uf57d"
                              "\uf57e\uf57f\uf580\uf581\uf578\uf583";

  char *baticon = calloc(4, sizeof(char));

  if (charging()) {
    int index = capacity() / 10;
    for (int i = 0; i < 3; i++) {
      baticon[i] = icons[i + index * 3];
    }
    return baticon;
  }

  if (capacity() == 100) {
    int index = 10;
    for (int i = 0; i < 3; i++) {
      baticon[i] = icons[i + index * 3];
    }
    return baticon;
  }

  for (int i = 0; i < 3; i++) {
    baticon[i] = icons[i + (11 * 3)];
  }
  return baticon;
}

int main(void) {

  Display *dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  char status[200];
  char *time;
  char *sig;
  char *battery;

  for (;; true) {
    time = mktimes();
    sig = mksigs();
    battery = baticon();

    sprintf(status, "[%s][\uf2db %0.2Lf%%][%s %d%%] %s ", sig, cpu(), battery,
            capacity(), time);

    XStoreName(dpy, DefaultRootWindow(dpy), status);
    XSync(dpy, False);

    free(time);
    free(sig);
    free(battery);
  }
}