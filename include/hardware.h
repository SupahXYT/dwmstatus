#include <string.h>
#define ICON_SIZE 4
#define THREADS 4

enum b_status { Discharging, Charging, Full, Unknown };
enum w_status { Disabled, Enabled };

typedef struct Battery {
  enum b_status status;
  int capacity;
  char *icon;
} Battery;

typedef struct WirelessDevice {
  enum w_status status;
  int signal;
  char *icon;
} WirelessDevice;

Battery get_battery(void);
WirelessDevice get_wireless_device(char *, size_t);
float cpu_load(void);
