/**
* \file    ComManager.cpp
* \brief     Implementation of ComManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.09.31
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <Log.h>
#include <utils/atoi.h>
#include <services/CommunicationManagerService/CommunicationManagerService.h>
#include <services/CommunicationManagerService/ICommunicationManagerService.h>

#include "man.h"
#include "sldd_common.h"
#include "CommunicationManager.h"

#define LOG_TAG "sldd_comm"

using namespace android;

static bool cm_handler_cmd(int32_t argc, char **argv);

static bool cm_handler_setprop(int32_t argc, char **argv);
/**
*    Declare binder interface
*
*/

SL_INTERFACE_DECLARE(CommunicationManagerService);



sldd_cmd_table_t cm_cmds[] = {
    {"cmd", 0, cm_handler_cmd,
     "  sldd cm cmd [<target>] [<data>]  - send command to target you want \n"
     "  sldd cm cmd [mcu/mdm/extap/bt] [<data>]         - send command to specific target \n"},
     {"setprop", 0, cm_handler_setprop,
     "  sldd cm setprop <prop> <value>     - set property of CommunicationManager \n"},
    {NULL, NULL, NULL, NULL}
};

bool commandActionCommunication(int32_t argc, char **argv) {

    if (argc == 0) {
        usage_communication(NULL);
        return true;
    }

    DO_HANDLER(cm_cmds, argv[0], argc-1, argv+1);
}

char* usage_communication(char *cmd)
{

    printMessage(" sldd cm [<command>] [parameters] - basic commend for CommunicationManager \n");
    printMessage("    <command> \n");
    PRINT_USAGE(cm_cmds);
    return NULL;
}

static bool cm_handler_cmd(int32_t argc, char **argv)
{
    size_t cmdLength= 0;

    if(argc < 2)
        return false;

    if(strcmp("mcu", argv[0]))
    {
        return false;
    }
    cmdLength = strlen(argv[1]);

    // command's length should be at lease 24 (12*2).
    if( cmdLength < 24 )
    {
        printMessage("[ERROR]Command's hex string minimum length is 24 \n");
        printMessage("Input Command Length is %d [%s] \n", cmdLength, argv[1]);
        return false;
    }

#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(CommunicationManagerService)->sendCommand(argv[1]);
#endif ///
    return true;
}

static bool cm_handler_setprop(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;
#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(CommunicationManagerService)->setProperty(argv[0], argv[1]);
#endif ///
    return true;
}

void register_communication()
{
    registerCommands(MODULE_COMMUNICATION_MANAGER, NULL, cm_cmds);
}
