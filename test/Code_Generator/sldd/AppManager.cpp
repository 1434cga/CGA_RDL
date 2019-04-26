/**
* \file    AppManager.cpp
* \brief     Implementation of AppManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.09.14
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <cstdlib>

#include <Log.h>
#include <corebase/Post.h>
#include <utils/Buffer.h>
#include <ApplicationManagerService/IApplicationManagerService.h>
#include <ApplicationManagerService/IApplicationManagerServiceType.h>

#include "include/ApplicationTable.h"
#include "man.h"
#include "sldd_common.h"
#include "AppManager.h"

#define LOG_TAG "sldd_app"

using namespace android;

enum am_commands {
    AM_CMD_STATUS =1,
    AM_CMD_POST,
    AM_CMD_SYSTEMPOST,
    AM_CMD_ENABLE_APP,
    AM_CMD_DISABLE_APP,
    //---------------------------------
    AM_CMD_DUMP,
};

static bool handler_am_status(int32_t argc, char **argv);
static bool handler_am_post(int32_t argc, char **argv);
static bool handler_am_systempost(int32_t argc, char **argv);
static bool handler_am_enable_app(int32_t argc, char **argv);
static bool handler_am_disable_app(int32_t argc, char **argv);
static bool handler_am_dump(int32_t argc, char **argv);
static bool handler_am_getprop(int32_t argc, char **argv);
static bool handler_am_setprop(int32_t argc, char **argv);
static bool handler_am_setpropi(int32_t argc, char **argv);
static bool handler_am_service(int32_t argc, char ** argv);

/**
*    Declare binder interface
*
*/

SL_INTERFACE_DECLARE2(ApplicationManagerService, APPLICATION_SRV_NAME);

sldd_cmd_table_t am_cmdTable[] = {
    {"status", AM_CMD_STATUS, handler_am_status,
     "  sldd am status                   - display Status of application manager  \n"
     "                                 app name, app id, service status and etc\n"},
    {"post", AM_CMD_POST, handler_am_post,
     "  sldd am post <app_id> <what>     - broadcast post to application \n"},
    {"systempost", AM_CMD_SYSTEMPOST, handler_am_systempost,
     "  sldd am systempost <what>        - broadcast system post \n"},
    {"enable_app", AM_CMD_ENABLE_APP, handler_am_enable_app,
     "  sldd am enable_app <app_id>      - enable application by app id \n"},
    {"disable_app", AM_CMD_DISABLE_APP, handler_am_disable_app,
     "  sldd am disable_app <app_id>     - disable application by app id \n"},
    {"setprop", AM_CMD_DISABLE_APP, handler_am_setprop,
     "  sldd am setprop <app_id> <key> <value> - set property of application by app id \n"},
    {"setpropi", AM_CMD_DISABLE_APP, handler_am_setpropi,
     "  sldd am setprop <app_id> <key> <integer value> - set property of application by app id \n"},
    {"getprop", AM_CMD_DISABLE_APP, handler_am_getprop,
     "  sldd am getprop <app_id> <key>    - get property of application by app id \n"},
    {"dump", AM_CMD_DUMP, handler_am_dump,
     "  sldd am dump <what>              - not work now \n"},
    {"service", AM_CMD_DUMP, handler_am_service,
     "  sldd am service <signal>         - inquire service \n"},
    {NULL, NULL, NULL}
};

bool commandActionApplication(int32_t argc, char **argv)
{

    if (argc == 0) {
        usage_application(NULL);
        return true;
    }

    DO_HANDLER(am_cmdTable, argv[0], argc-1, argv+1);
}


char* usage_application(char *cmd)
{
    printMessage(" sldd am [<command>]           - perform ApplicationManager's functions \n");
    printMessage("    <command> \n");
    PRINT_USAGE(am_cmdTable);
    return NULL;
}

static bool handler_am_status(int32_t argc, char **argv)
{
    printMessage("==============================================\n");
    printMessage(" Appliacation Status \n");
    printMessage("----------------------------------------------\n");
    printMessage("|%17s | %7s| %15s |\n", "app name", "app id", "service state");
#if 0/*!defined(APP_MANIFEST)*/
    for(int i=1 ; sApp_list[i].app_id != APP_ID_MAX ; i++)
    {
        printMessage("|%17s | %7d|", sApp_list[i].app_name, sApp_list[i].app_id);

        bool enabled = SL_INTERFACE_GET(ApplicationManagerService)->isEnabledApplication(sApp_list[i].app_id);
        const char* status = (const char*)((enabled == true)?"enabled":"disabled");

        printMessage(" %15s |\n", status);
    }
#endif
    printMessage("======================================\n");
    return true;
}

bool handler_am_post(int32_t argc, char **argv)
{
    if (argc != 2) {
        usage_application(NULL);
        return false;
    }

    int32_t app_id = strtol(argv[0], NULL, 10);
    const char* name = find_app_name(app_id);
    if(name == NULL)
    {
        printMessage("==============================\n");
        printMessage("   Fail post Application:%d \n", app_id);
        printMessage("     Invalid app id:%d \n", app_id);
        printMessage("==============================\n");
        return false;
    }

    int32_t what = strtol(argv[1], NULL, 10);

    printMessage("========================================\n");
    printMessage("    post (To:%s what: %d)\n", name,what);
    printMessage("========================================\n");
    sp<Post> post = Post::obtain(what);
    error_t error =
            SL_INTERFACE_GET(ApplicationManagerService)->sendPost(app_id, post);
    if(error == E_OK)
    {
        printMessage(
            "==============================\n"
            "   Success post <%s>:%d \n", name, app_id,
            "==============================\n");
        return true;
    }
    else
   {
        printMessage(
            "==============================\n"
            "   Fail post :%d \n", app_id,
            "==============================\n");
    }

    return false;
}

bool handler_am_systempost(int32_t argc, char **argv)
{
    if (argc != 1) {
        return false;
    }

    int32_t what = strtol(argv[0], NULL, 10);
    if(what == 0)
    {
        printMessage("==============================\n");
        printMessage("   Bad system post :0x%x \n", what);
        printMessage("==============================\n");
        return false;
    }

    printMessage("========================================\n");
    printMessage("    sytem post (what: %d)\n", what);
    printMessage("========================================\n");
    sp<Post> post = Post::obtain(what);
    error_t error =
            SL_INTERFACE_GET(ApplicationManagerService)->broadcastSystemPost(post);
    if(error == E_OK)
    {
        printMessage(
            "==============================\n"
            "   Success sytem post \n"
            "==============================\n");
        return true;
    }
    else
    {
        printMessage(
            "==============================\n"
            "   Fail sytem post \n"
            "==============================\n");
    }

    return false;
}


bool handler_am_enable_app(int32_t argc, char **argv)
{
    int32_t app_id = strtol(argv[0], NULL, 10);

    const char* name = find_app_name(app_id);
    if(name == NULL)
    {
        printMessage(
            "==============================\n"
            "   Fail Enable Application:%d \n", app_id,
            "     Invalid app id:%d \n", app_id,
            "==============================\n");
        return false;
    }

    error_t error =
        SL_INTERFACE_GET(ApplicationManagerService)->enableApplication(app_id, 0/*reason*/);

    if(error == E_OK)
    {
        printMessage(
            "==============================\n"
            "   Success Enable Application:%d \n", app_id,
            "     <%s> : enabled \n", name,
            "==============================\n");
        return true;
    }
    else
   {
        printMessage(
            "==============================\n"
            "   Fail Enable Application:%d \n", app_id,
            "     can not be enabled %s  due to %d\n", name, error,
            "==============================\n");
    }

    return false;
}


bool handler_am_disable_app(int32_t argc, char **argv)
{
    int32_t app_id = strtol(argv[0], NULL, 10);

    const char* name = find_app_name(app_id);
    if(name == NULL)
    {
        printMessage(
            "==============================\n"
            "   Fail Disable Application:%d \n", app_id,
            "     Invalid app id:%d \n", app_id,
            "==============================\n");
        return false;
    }

    error_t error =
        SL_INTERFACE_GET(ApplicationManagerService)->disableApplication(app_id, 0/*reason*/);

    if(error == E_OK)
    {
        printMessage(
            "==============================\n"
            "   Success Disable Application:%d \n", app_id,
            "     <%s> : enabled \n", name,
            "==============================\n");
        return true;
    }
    else
    {
        printMessage(
            "==============================\n"
            "   Fail Disable Application:%d \n", app_id,
            "     can not be enabled %s  due to %d\n", name, error,
            "==============================\n");
    }

    return false;
}

bool handler_am_getprop(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;

    int32_t app_id = strtol(argv[0], NULL, 10);
    const char* appname = find_app_name(app_id);
    if(appname == NULL)
    {
        printMessage("==============================\n");
        printMessage("     Invalid app id:%d \n", app_id);
        printMessage("==============================\n");
        return false;
    }

    bool enable = SL_INTERFACE_GET(ApplicationManagerService)->isEnabledApplication(app_id);
    if(enable == false)
    {
        printMessage("==============================\n");
        printMessage("   %s should be enabled first \n", appname);
        printMessage("==============================\n");
        return false;
    }

    char* name = argv[1];
#if defined(_DEVICE_NAD)
    sp<Buffer> buf = SL_INTERFACE_GET(ApplicationManagerService)->getProperty(app_id, name);
    printMessage("========  get prop[%s][%d]  ==========\n",appname, app_id);
    printMessage(" prop:%s value:%s  \n", name, buf->data());
    printMessage("==============================\n");
#endif ///
    return true;
}

bool handler_am_setprop(int32_t argc, char **argv)
{
    if(argc < 3)
        return false;

    int32_t app_id = strtol(argv[0], NULL, 10);
    const char* appname = find_app_name(app_id);
    if(appname == NULL)
    {
        printMessage("==============================\n");
        printMessage("     Invalid app id:%d \n", app_id);
        printMessage("==============================\n");
        return false;
    }

    bool enable = SL_INTERFACE_GET(ApplicationManagerService)->isEnabledApplication(app_id);
    if(enable == false)
    {
        printMessage("==============================\n");
        printMessage("   %s should be enabled first \n", appname);
        printMessage("==============================\n");
        return false;
    }

    char* name = argv[1];
    char* value = argv[2];
#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(ApplicationManagerService)->setProperty(app_id, name, value);
    printMessage("========  set prop[%d]  ==========\n", app_id);
    printMessage(" prop:%s value:%s  \n", ((name==NULL)?"NULL":name), value);
    printMessage("==============================\n");
#endif ///
    return true;
}

bool handler_am_setpropi(int32_t argc, char **argv)
{
    if(argc < 3)
        return false;

    int32_t app_id = strtol(argv[0], NULL, 10);
    const char* appname = find_app_name(app_id);
    if(appname == NULL)
    {
        printMessage("==============================\n");
        printMessage("     Invalid app id:%d \n", app_id);
        printMessage("==============================\n");
        return false;
    }

    bool enable = SL_INTERFACE_GET(ApplicationManagerService)->isEnabledApplication(app_id);
    if(enable == false)
    {
        printMessage("==============================\n");
        printMessage("   %s should be enabled first \n", appname);
        printMessage("==============================\n");
        return false;
    }

    char* name = argv[1];
    int32_t value =  strtol(argv[2], NULL, 10);
#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(ApplicationManagerService)->setProperty(app_id, name, value);
    printMessage("========  set prop[%d]  ==========\n", app_id);
    printMessage(" prop:%s value:%d  \n", ((name==NULL)?"NULL":name), value);
    printMessage("==============================\n");
#endif ///
    return true;
}

bool handler_am_service(int32_t argc, char ** argv)
{
    if(argc < 1)
        return false;

    int32_t signal_id = strtol(argv[0], NULL, 10);

    error_t error = SL_INTERFACE_GET(ApplicationManagerService)->inquireApplicationStatus(signal_id);
    switch(error)
    {
        case E_OK:
            printMessage("==============================\n");
            printMessage("   service[%d] can start now \n", signal_id);
            printMessage("==============================\n");
            break;

        case E_ERROR:
            printMessage("==============================\n");
            printMessage("   service[%d] can not start due to error \n", signal_id);
            printMessage("==============================\n");
            break;

        case E_REJECTED:
            printMessage("==============================\n");
            printMessage("   service[%d] is rejected \n", signal_id);
            printMessage("==============================\n");
            break;

        case E_PENDING:
            printMessage("==============================\n");
            printMessage("   service[%d] can not start now. wait please... \n", signal_id);
            printMessage("==============================\n");
            break;

        default:
            printMessage("==============================\n");
            printMessage("   service[%d]. unexpected error \n", signal_id);
            printMessage("==============================\n");
            break;
    }
    return true;
}

bool handler_am_dump(int32_t argc, char **argv)
{
    return false;
}

void register_application()
{
    registerCommands(MODULE_APP_MANAGER, NULL, am_cmdTable);
}

bool check_application()
{
    if( SL_INTERFACE_GET(ApplicationManagerService) == NULL)
        return false;

    return true;
}

