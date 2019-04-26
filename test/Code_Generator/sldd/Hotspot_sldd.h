
#ifndef SLDD_HOTSPOT_H
#define SLDD_HOTSPOT_H

#define MODULE_MBC "mbc"

#include "services/vifManagerService/IvifManagerService.h"


extern bool commandActionMBC(int32_t argc, char **argv);
extern char* usage_MBC(char* cmd);

extern void register_MBC();

#endif

