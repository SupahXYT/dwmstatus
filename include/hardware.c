#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>

static const char icons[][ICON_SIZE] = {"\uf58d", "\uf579", "\uf57a", "\uf57b",
                                        "\uf57c", "\uf57d", "\uf57e", "\uf57f",
                                        "\uf580", "\uf581", "\uf578", "\uf583"};

static int _bat_capacity(char *path) {
  FILE *fp;
  char buf[4];
  int capacity;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr,
            "Could not open file at %s:"
            "\n",
            path);
    return (-1);
  } else {
    capacity = atoi(fgets(buf, 4, fp));
  }

  fclose(fp);
  return capacity;
}

static int _bat_ischarging(char *path) {
  FILE *fp;
  char fcstatus;

  fp = fopen(path, "r");
  if (fp == NULL) {
    fprintf(stderr,
            "Could not open file at %s:"
            "\n",
            path);
    return (-1);
  } else {
    fcstatus = fgetc(fp);
  }

  return (fcstatus == 'C' || fcstatus == 'F');
}

static char *bat_icon(char *path) {
  int index;
  if (_bat_ischarging(path)) {
    index = 11;
  } else {
    index = _bat_capacity(path) / 10;
  }

  return (char *)icons[index];
}

struct battery getbattery(char *path) {
  struct battery bat = {.icon = _bat_icon(path),
                        .capacity = _bat_capacity(path)};
  return bat;
}
