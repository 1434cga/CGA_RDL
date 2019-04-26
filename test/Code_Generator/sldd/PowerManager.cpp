/**
* \file    PowerManager.cpp
* \brief     Implementation of PowerManager
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


#include <PowerState.h>
#include <AlarmManagerService/IAlarmExpireListener.h>
#include <AlarmManagerService/IAlarmManagerService.h>
#include <AlarmManagerService/IAlarmManagerServiceType.h>
#include <ConfigurationManagerService/IConfigurationManagerService.h>
#include <ConfigurationManagerService/IConfigurationManagerServiceType.h>
#include <PowerManagerService/IPowerManagerService.h>
#include <PowerManagerService/IPowerManagerServiceType.h>
#include <PowerManagerService/KeepPower.h>

#include "include/utils/ThreadCondition.h"
//#include "services/AlarmManagerService/Alarm.h"
#include "services/AlarmManagerService/ScheduledWakeup.h"
#include "services/PowerManagerService/PowerManagerProperty.h"
#include "services/PowerManagerService/JLR_State/PowerModeTimer.h"
#include "man.h"
#include "sldd_common.h"
#include "PowerManager.h"
#include "services/ConfigurationManagerService/IConfigurationManagerServiceType.h"

#define LOG_TAG "sldd_power"

using namespace android;
using namespace powermanager;

enum pm_commands {
    PM_CMD_STATUS = 1,
    PM_CMD_ENABLE,
    PM_CMD_DISABLE,
    PM_CMD_TIMER,
    PM_CMD_TIMER_LISTEN2,
    PM_CMD_TIMER_SLEEP,
    PM_CMD_KEEPPOWER,
    PM_CMD_KEEPPOWER_RELEASE,
    PM_CMD_KEEPPOWER_CLEAR
};

#if defined(_DEVICE_NAD)
SLProperty* mProperty = NULL;
#endif


static bool handler_status(int32_t argc, char **argv);
static bool handler_enable(int32_t argc, char **argv);
static bool handler_disable(int32_t argc, char **argv);
static bool handler_settimer(int32_t argc, char **argv);
static bool handler_gettimer(int32_t argc, char **argv);
static bool handler_schedule(int32_t argc, char **argv);
static bool handler_keeppower(int32_t argc, char **argv);
static bool handler_reset(int32_t argc, char **argv);

static bool handler_network_check(int32_t argc, char **argv);
static bool handler_setprop(int32_t argc, char **argv);
static bool handler_getprop(int32_t argc, char **argv);

static bool handler_vBUB(int32_t argc, char **argv);
static bool handler_powerSource(int32_t argc, char **argv);
static bool handler_initRTC(int32_t argc, char **argv);
static bool handler_getRTCElapse(int32_t argc, char **argv);

static bool handler_listen(int32_t argc, char **argv);
static bool handler_sleep(int32_t argc, char **argv);
static bool handler_wakeup(int32_t argc, char **argv);

static bool handler_hold(int32_t argc, char **argv);
static bool handler_holdlisten(int32_t argc, char **argv);

/**
*    Declare binder interface
*/
SL_INTERFACE_DECLARE2(AlarmManagerService, ALARM_SRV_NAME);
SL_INTERFACE_DECLARE2(ConfigurationManagerService, CONFIGURATION_SRV_NAME);
SL_INTERFACE_DECLARE2(PowerManagerService, POWER_SRV_NAME);

sldd_cmd_table_t pm_cmds[] = {
    {"status", PM_CMD_STATUS, handler_status,
     "  sldd pm status     - display power manager status \n"},
    {"enable", PM_CMD_ENABLE, handler_enable,
     "  sldd pm enable     - enable power mode to perform power mode operation \n"},
    {"disable", PM_CMD_DISABLE, handler_disable,
     "  sldd pm disable    - disable power mode to keep normal mode \n"},
    {"settimer", PM_CMD_TIMER, handler_settimer,
     "  sldd pm settimer <name> <seconds> - set timer for PowerManager \n"},
    {"gettimer", PM_CMD_TIMER, handler_gettimer,
     "  sldd pm gettimer <name>           - get timer for PowerManager \n"},
    {"schedule", PM_CMD_TIMER, handler_schedule,
     "  sldd pm schedule [<command>]       - to control schedule wakeup \n"
     "  sldd pm schedule enable            - to enable schedule wakeup \n"
     "  sldd pm schedule disable           - to disable schedule wakeup \n"
     "  sldd pm schedule status            - display schedule wakeup status \n"},
    {"keeppower", PM_CMD_KEEPPOWER, handler_keeppower,
     "  sldd pm keeppower [<command>]       - to control KeepPower \n"
     "  sldd pm keeppower assign       - assign new KeepPower to use \n"
     "                       - This will display <id> to be able to use add/delete \n"
     "  sldd pm keeppower add <id>     - acquire KeepPower to PowerManager \n"
     "  sldd pm keeppower delete <id>  - release KeepPower to PowerManager \n"
     "  sldd pm keeppower clear        - release all of KeepPowers to PowerManager\n"
     "  sldd pm keeppower test         - sanity test for assinging, adding and deleting \n"},
    {"reset", PM_CMD_ENABLE, handler_reset,
     "  sldd pm reset [<target:all by default>] - reset TCU \n"
     "  sldd pm reset mcu - reset MCU \n"},
    {"network_check", PM_CMD_ENABLE, handler_network_check,
     "  sldd pm network_check enable       - enable network check \n"
     "  sldd pm network_check disable      - disable network check \n"},
    {"setprop", PM_CMD_ENABLE, handler_setprop,
     "  sldd pm setprop <prop> <value>     - set property of PowerManager \n"},
    {"getprop", PM_CMD_ENABLE, handler_getprop,
     "  sldd pm getprop <prop>             - get property of PowerManager\n"},
    {"vbub", PM_CMD_ENABLE, handler_vBUB,
     "  sldd pm vbub                       - display Voltage of Backup battery\n"},
    {"powersource", PM_CMD_ENABLE, handler_powerSource,
     "  sldd pm powersource                - display Current PowerSource\n"},
    {"initRTC", PM_CMD_ENABLE, handler_initRTC,
     "  sldd pm initRTC                - set BT RTC count init\n"},
    {"getRTCElapse", PM_CMD_ENABLE, handler_getRTCElapse,
     "  sldd pm getRTCElapse           - get BT RTC elpased time\n"},
    {"listen", PM_CMD_ENABLE, handler_listen,
     "  sldd pm listen                       - go to listen of Modem as Low power mode\n"},
    {"sleep", PM_CMD_ENABLE, handler_sleep,
     "  sldd pm sleep                       - go to sleep as Very Low power mode\n"},
    {"wakeup", PM_CMD_ENABLE, handler_wakeup,
     "  sldd pm wakeup                      - go to Normal mode\n"},
    {"hold", PM_CMD_ENABLE, handler_hold,
     "  sldd pm hold <start secs> <resume secs> - hold Power state from listen\n"},
    {"holdlisten", PM_CMD_ENABLE, handler_hold,
     "  sldd pm holdlisten <start secs> <resume secs> - go to Normal mode\n"},
    {NULL, NULL, NULL, NULL}
};

bool commandActionPower(int32_t argc, char **argv) {
#if defined(_DEVICE_NAD)
    if(mProperty == NULL)
    {
        //mProperty = new SLProperty("PowerManagerService", true);
        //mProperty->initPropertyCache();
    }
#endif

    if (argc == 0) {
        usage_power(NULL);
        return true;
    }

    DO_HANDLER(pm_cmds, argv[0], argc-1, argv+1);
}

char* usage_power(char *cmd)
{

    printMessage(" sldd pm [<command>] [parameters] - basic commend for PowerManager \n");
    printMessage("    <command> \n");
    PRINT_USAGE(pm_cmds);
    return NULL;
}

static bool handler_status(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    int32_t enable = SL_INTERFACE_GET(PowerManagerService)->getProperty(PROPERTY_SERVICE_DISABLE);
#else
    int32_t enable = true;
#endif
//    int32_t power_state = SL_INTERFACE_GET(PowerManagerService)->getCurrentPowerState();
    int32_t power_mode = SL_INTERFACE_GET(PowerManagerService)->getCurrentPowerMode();


    printMessage("====================================================\n");
    printMessage(" Power Manager Status \n");
    printMessage("----------------------------------------------------\n");
    printMessage("|%17s | %17s| %50s |\n", "name", "value", "service state");
    printMessage("----------------------------------------------------\n");

    printMessage("|%17s | %17s|%50s |\n", "mode", (const char*)((enable == 0)?"enabled":"disabled"),
                  " power manager setting");

    printMessage("|%17s | %17s|%50s |\n", "power mode", power_mode_str[power_mode],
                  " power mode from CAN signal ");
#if defined(_DEVICE_NAD)

    for(int8_t i=0 ; i<PowerModeTimer::TIMER_IDX_MAX ; i++)
    {
        if(i == PowerModeTimer::TIMER_IDX_LISTEN1)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" Listen%d Timers \n", 1);
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_LISTEN2)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" Listen%d Timers \n", 2);
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING1)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING1 Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING1_SLEEP)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING1 Sleep Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING2)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING2 Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING2_SLEEP)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING2 Sleep Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING1_RECEIVE)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING1 receive Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        if(i == PowerModeTimer::TIMER_IDX_POLLING2_RECEIVE)
        {
            printMessage("----------------------------------------------------\n");
            printMessage(" POLLING2 receive Timers \n");
            printMessage("----------------------------------------------------\n");
        }
        int32_t schedule_not_set = i;
        int32_t schedule_set = i+1;
        int32_t unprovision = i+2;
        int32_t value = SL_INTERFACE_GET(ConfigurationManagerService)->
                  get_int(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL,
                                  TIMER_CONFIG_NAME[schedule_not_set]);
        printMessage("|%35s | %10d|%50s |\n", TIMER_CONFIG_NAME[schedule_not_set], value,
                      " active timer when Scheduled Wakeup has not been set ");
        value = SL_INTERFACE_GET(ConfigurationManagerService)->
                  get_int(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL,
                                  TIMER_CONFIG_NAME[schedule_set]);
        printMessage("|%35s | %10d|%50s |\n", TIMER_CONFIG_NAME[schedule_set], value,
                      " active timer when Scheduled Wakeup has been set ");
        value = SL_INTERFACE_GET(ConfigurationManagerService)->
                  get_int(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL,
                                  TIMER_CONFIG_NAME[unprovision]);
        printMessage("|%35s | %10d|%50s |\n", TIMER_CONFIG_NAME[unprovision], value,
                      " active timer in unprovisioned mode ");
        i+=2;
    }
#endif
    printMessage("============================================\n");

    return true;
}

static bool handler_enable(int32_t argc, char **argv)
{
    SL_INTERFACE_GET(PowerManagerService)->enablePowerMode();

    return true;
}

static bool handler_disable(int32_t argc, char **argv)
{
    SL_INTERFACE_GET(PowerManagerService)->disablePowerMode();
    return true;
}

static bool handler_settimer(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;

    if(argv[0] == NULL)
        return false;

    const char* timer_name =  argv[0];
    int32_t sec = sl::atoi(argv[1]);
    if (sec < 0)
        return false;
#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(ConfigurationManagerService)->
                  change_set_int(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL,
                                  timer_name, sec);
    //SL_INTERFACE_GET(PowerManagerService)->setPowerModeTimer(mode, strtol(argv[1], NULL, 10));
    printMessage("========  set timer  ==========\n");
    printMessage("  %s  value:%d \n", argv[0], sec);
    printMessage("==============================\n");
#endif
    return true;
}

static bool handler_gettimer(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(argv[0] == NULL)
        return false;

#if defined(_DEVICE_NAD)

    const char* timer_name =  argv[0];
    int32_t sec = SL_INTERFACE_GET(ConfigurationManagerService)->
                  get_int(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL,
                                  timer_name);
    printMessage("========  set timer  ==========\n");
    printMessage("  %s  value:%d \n", argv[0], sec);
    printMessage("==============================\n");
#endif
    return true;
}

static bool handler_schedule(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

#if defined(_DEVICE_NAD)
    int32_t enable = SL_INTERFACE_GET(PowerManagerService)->getProperty(PROPERTY_SERVICE_DISABLE);

    if(!strcmp(argv[0],"enable"))
    {
        SL_INTERFACE_GET(PowerManagerService)->setProperty(PROPERTY_TIMER_SCHEDULE_WAKEUP, 1);
        printMessage("========  enable scheduled wakeup  ==========\n");
    } else if(!strcmp(argv[0],"disable"))
    {
        SL_INTERFACE_GET(PowerManagerService)->setProperty(PROPERTY_TIMER_SCHEDULE_WAKEUP, 0);
        printMessage("========  disable scheduled wakeup  ==========\n");
    } else if(!strcmp(argv[0],"status"))
    {
        int32_t enable = SL_INTERFACE_GET(PowerManagerService)->getProperty(PROPERTY_SERVICE_DISABLE);
        printMessage("========  scheduled wakeup  ==========\n");
        printMessage("     %s  \n", (const char*)((enable == 0)?"enabled":"disabled"));
        printMessage("==============================\n");
    } else
    {
        ///abnormal parameter
        return false;
    }
#endif
    return true;
}

static bool handler_keeppower(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"assign"))
    {
        int32_t id = SL_INTERFACE_GET(PowerManagerService)->assignKeepPower(0, new BBinder());
        printMessage("========  success ==========\n");
        printMessage("   use as below with %d \n", id);
        printMessage(" sldd pm keeppower add %d  \n", id);
        printMessage(" sldd pm keeppower delete %d  \n", id);
        printMessage("==============================\n");
    } else if(!strcmp(argv[0],"add"))
    {
        int32_t id = sl::atoi(argv[1]);
        SL_INTERFACE_GET(PowerManagerService)->acquireKeepPower(id);
    } else if(!strcmp(argv[0],"delete"))
    {
        int32_t id = sl::atoi(argv[1]);
        SL_INTERFACE_GET(PowerManagerService)->releaseKeepPower(id);
    } else if(!strcmp(argv[0],"clean"))
    {
        //SL_INTERFACE_GET(PowerManagerService)->releaseKeepPower(0);
    } else if(!strcmp(argv[0],"test"))
    {
        KeepPower keeppower(10);

        keeppower.acquire();
        keeppower.acquire();

        keeppower.release();
        keeppower.release();

        keeppower.release();
        keeppower.release();
    }else
    {
        ///abnormal parameter
        return false;
    }

    return true;
}

bool handler_reset(int32_t argc, char **argv)
{
    char* target = argv[0];
    if(target == NULL)
        target = "all";

    printMessage(" waiting reset %s \n", target);
    if(!strcmp(target,"all"))
    {
        SL_INTERFACE_GET(PowerManagerService)->reset(POWERMANAGER_RESET_ALL);
    } else if(!strcmp(target,"mcu"))
    {
        SL_INTERFACE_GET(PowerManagerService)->reset(POWERMANAGER_RESET_MCU);
    } else if(!strcmp(target,"mdm"))
    {
        SL_INTERFACE_GET(PowerManagerService)->reset(POWERMANAGER_RESET_MDM);
    } else
    {
        ///abnormal parameter
        return false;
    }

    return true;
}

bool handler_network_check(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;
#if defined(_DEVICE_NAD)

    if(!strcmp(argv[0],"enable"))
    {
        SL_INTERFACE_GET(PowerManagerService)->setProperty(PROPERTY_CHECK_NETWORK_STATE, 1);
    } else if(!strcmp(argv[0],"disable"))
    {
        SL_INTERFACE_GET(PowerManagerService)->setProperty(PROPERTY_CHECK_NETWORK_STATE, 0);
    } else if(!strcmp(argv[0],"status"))
    {
        int32_t enable = SL_INTERFACE_GET(PowerManagerService)->getProperty(PROPERTY_CHECK_NETWORK_STATE);
        printMessage("========  network_check  ==========\n");
        printMessage("     %s  \n", (const char*)((enable == 1)?"enabled":"disabled"));
        printMessage("==============================\n");
    } else
    {
        ///abnormal parameter
        return false;
    }
#endif ///
    return true;
}

static bool handler_setprop(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;
#if defined(_DEVICE_NAD)
    SL_INTERFACE_GET(PowerManagerService)->setProperty(argv[0], argv[1]);
#endif ///
    return true;
}

bool handler_getprop(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;
#if defined(_DEVICE_NAD)
    int32_t value = SL_INTERFACE_GET(PowerManagerService)->getProperty(argv[0]);
    printMessage("========  get prop  ==========\n");
    printMessage(" prop:%s value:%d  \n", argv[0], value);
    printMessage("==============================\n");
#endif ///
    return true;
}

bool handler_vBUB(int32_t argc, char **argv)
{
    float value = SL_INTERFACE_GET(PowerManagerService)->getBUBVoltage();
    printMessage("========  Backup battery Voltage  ==========\n");
    printMessage(" value:%.4f  \n", value);
    printMessage("============================================\n");
    return true;
}

bool handler_powerSource(int32_t argc, char **argv)
{
    int32_t value = SL_INTERFACE_GET(PowerManagerService)->getPowerSource();
    printMessage("========  PowerSource  ==========\n");
    printMessage(" value:%d  (0 - BUB, 1 - VBATT)\n", value);
    printMessage("============================================\n");
    return true;
}

bool handler_initRTC(int32_t argc, char **argv)
{
    error_t value = SL_INTERFACE_GET(PowerManagerService)->startRTCCountInit();
    printMessage("================  initRTC  =================\n");
    printMessage(" %s\n", value == E_OK ? "Success" : "Fail");
    printMessage("============================================\n");
    return true;
}

bool handler_getRTCElapse(int32_t argc, char **argv)
{
    int32_t value = SL_INTERFACE_GET(PowerManagerService)->getRTCElapsedTime();
    printMessage("===============  getRTCElapse  =============\n");
    printMessage(" value:%d (seconds)\n", value);
    printMessage("============================================\n");
    return true;
}

bool handler_listen(int32_t argc, char **argv)
{
    printMessage("========  goToListen  ==========\n");
    uint64_t sleepMs = sl::atoi(argv[0]);
    if(sleepMs == 0)
        sleepMs = 10*1000;

    SL_INTERFACE_GET(PowerManagerService)->goToSleep(1, sleepMs);

    printMessage("=================================\n");
    return true;
}

bool handler_sleep(int32_t argc, char **argv)
{
    printMessage("========  goToSleep  ==========\n");
    uint64_t sleepMs = sl::atoi(argv[0]);
    if(sleepMs == 0)
        sleepMs = 10*1000;

    SL_INTERFACE_GET(PowerManagerService)->goToSleep(0, sleepMs);

    printMessage("=================================\n");
    return true;
}

bool handler_wakeup(int32_t argc, char **argv)
{
    printMessage("========  wakeup  ==========\n");
    int32_t reason = sl::atoi(argv[0]);

    SL_INTERFACE_GET(PowerManagerService)->wakeup(reason);

    printMessage("=================================\n");
    return true;
}

static ThreadCondition wait_update;
class sldd_start_expire_listener : public BnAlarmExpireListener
{
    virtual void onAlarmExpired(const int32_t param)
    {
        LOGE(
        "========================================\n"
        "   start time is expired from AlarmManager \n",
        "========================================\n");
        SL_INTERFACE_GET(PowerManagerService)->holdPowerState(false);
        wait_update.signal(1);
    }
};

class sldd_end_expire_listener : public BnAlarmExpireListener
{
    virtual void onAlarmExpired(const int32_t param)
    {
        LOGE(
        "========================================\n"
        "   end time is expired from AlarmManager \n",
        "========================================\n");

        SL_INTERFACE_GET(PowerManagerService)->resumePowerState();
        wait_update.signal(1);
    }
};


bool handler_hold(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;

    int64_t startMs = strtol(argv[0], NULL, 10);
    startMs = startMs*1000;

    int64_t endMs = strtol(argv[1], NULL, 10);
    endMs = endMs*1000;

    sp<IAlarmExpireListener> listener = new sldd_start_expire_listener();
    error_t error =
            SL_INTERFACE_GET(AlarmManagerService)->
            setAlarmFromNow(0, startMs, false, listener );

    listener = new sldd_end_expire_listener();
    SL_INTERFACE_GET(AlarmManagerService)->
            setAlarmFromNow(0, endMs, false, listener );
    wait_update.wait(1);
    LOGE(
    "==============================\n"
    "   Success start to hold <%lld> \n",startMs);

    wait_update.wait(1);
    LOGE(
    "==============================\n"
    "   Success resume from hold <%lld> \n", endMs);

    while(true)
    {
    }
    return true;
}


bool handler_holdlisten(int32_t argc, char **argv)
{
    return true;
}

void register_power()
{
    registerCommands(MODULE_POWER_MANAGER, NULL, pm_cmds);
}
