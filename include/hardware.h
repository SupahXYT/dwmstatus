#define ICON_SIZE 4

enum BatteryStatus { Discharging, Charging, Full, Unknown };
enum WirelessStatus { Disabled, Enabled };

typedef struct Battery {
  enum BatteryStatus status;
  int capacity;
  char *icon;
} Battery;

typedef struct WirelessDevice {
  enum WirelessStatus status;
  int signal;
  char *icon;
} WirelessDevice;

Battery get_battery(void);
WirelessDevice get_wireless_device(char *);
float cpu(void);
