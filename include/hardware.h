#define ICON_SIZE 4

struct battery {
  char *icon;
  int capacity;
  int status;
};

struct battery getbattery(char *path);
int signal(char *interface);
float cpu(void);
