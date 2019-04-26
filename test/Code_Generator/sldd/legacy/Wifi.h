
#ifndef SLDD_WIFI_H
#define SLDD_WIFI_H

#define MODULE_WIFI "wifi"
//#define MODULE_WIFI "WIFI"

extern bool commandActionWifi(int32_t argc, char **argv);
extern char* usage_wifi(char* cmd);

extern void register_wifi();

#endif

