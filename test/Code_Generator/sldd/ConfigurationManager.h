#ifndef SLDD_CONFIGURATION_H
#define SLDD_CONFIGURATION_H

#define MODULE_CONFIGURATION_MANAGER "cfg"

bool commandActionConfiguration(int32_t argc, char **argv);
char* usage_configuration(char* cmd);
extern void register_configuration();

#endif
