/**
* \file    Watchdog.cpp
* \brief     Implementation of Watchdog
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2016.03.04
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <services/WatchdogManagerService/IWatchdogManagerService.h>
#include <services/WatchdogManagerService/IWatchdogManagerServiceType.h>

#include "man.h"
#include "sldd_common.h"
#include "WatchdogManager.h"

#define LOG_TAG "sldd_watchdog"

using namespace android;

static bool Watchdog_handler_setprop(int32_t argc, char **argv);
static bool Watchdog_handler_getprop(int32_t argc, char **argv);

/**
*    Declare binder interface
*/
SL_INTERFACE_DECLARE2(WatchdogManagerService, WATCHDOG_SRV_NAME);

sldd_cmd_table_t watchdog_cmds[] = {
    {"setprop", 0, Watchdog_handler_setprop,
     "  sldd watchdog setprop <prop> <value>     - set property of WatchdogManagerService \n"},
    {"getprop", 0, Watchdog_handler_getprop,
     "  sldd watchdog getprop <prop>             - get property of WatchdogManagerService \n"},
    {NULL, NULL, NULL, NULL}
};

bool commandActionWatchdog(int32_t argc, char **argv) {

    if (argc == 0) {
        usage_Watchdog(NULL);
        return true;
    }

    DO_HANDLER(watchdog_cmds, argv[0], argc-1, argv+1);
}

char* usage_Watchdog(char *cmd)
{

    printMessage(" sldd watchdog [<command>] [parameters] - basic commend for WatchdogManagerService \n");
    printMessage("    <command> \n");
    PRINT_USAGE(watchdog_cmds);
    return NULL;
}

static bool Watchdog_handler_setprop(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;
#if defined(_DEVICE_NAD)
    printMessage("   setprop %s %s \n", argv[0], argv[1]);
    SL_INTERFACE_GET(WatchdogManagerService)->setProperty(argv[0], argv[1]);
#endif ///
    return true;
}

static bool Watchdog_handler_getprop(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;
#if defined(_DEVICE_NAD)
    char* value = (char*)SL_INTERFACE_GET(WatchdogManagerService)->getProperty(argv[0]);
    if(value == NULL)
        printMessage("   getprop %s NULL \n", argv[0]);
    else
        printMessage("   getprop %s %s \n", argv[0], value);
#endif ///
    return true;
}


void register_Watchdog()
{
    registerCommands(MODULE_WATCHDOG_MANAGER, NULL, watchdog_cmds);
}
