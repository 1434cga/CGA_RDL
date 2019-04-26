/**
* \file    AlarmHal.cpp
* \brief     Implementation of AlarmHal
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2016.04.04
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#include <cstdlib>
#include <time.h>
#include <sys/time.h>

#include <AlarmManagerService/IAlarmExpireListener.h>
#include <AlarmManagerService/IAlarmManagerService.h>
#include <AlarmManagerService/IAlarmManagerServiceType.h>
#include <SystemManagerService/ISystemManagerService.h>
#include <SystemManagerService/ISystemManagerServiceType.h>

#include "include/hal/alarm/AlarmHal.h"
#include "include/utils/ThreadCondition.h"
#include "services/AlarmManagerService/ScheduledWakeup.h"
#include "man.h"
#include "sldd_common.h"
#include "AlarmManager.h"

#define LOG_TAG "sldd_alarm"

using namespace android;

static bool al_handler_setAlarm(int32_t argc, char **argv);
static bool al_handler_getAlarm(int32_t argc, char **argv);
static bool al_handler_setAlarmHAL(int32_t argc, char **argv);
static bool al_handler_getAlarmHAL(int32_t argc, char **argv);
static bool al_handler_setScheduledWakeup(int32_t argc, char **argv);
static bool al_handler_offAlarmInterrupt(int32_t argc, char **argv);

/**
*    Declare binder interface
*
*/

SL_INTERFACE_DECLARE2(AlarmManagerService, ALARM_SRV_NAME);
SL_INTERFACE_DECLARE2(SystemManagerService, SYSTEM_SRV_NAME);

sldd_cmd_table_t alarm_cmds[] = {
    {"set", 0, al_handler_setAlarm,
     "  sldd alarm set [<time seconds>]    - set alarm time via AlarmManager \n"},
    {"get", 0, al_handler_getAlarm,
     "  sldd alarm get                - get current alarm timer value \n"},
    {"sethal", 0, al_handler_setAlarmHAL,
     "  sldd alarm sethal [<time seconds>]    - set alarm time via HAL interface \n"},
    {"gethal", 0, al_handler_getAlarmHAL,
     "  sldd alarm gethal                - get current alarm timer value \n"},
    {"schedule", 0, al_handler_setScheduledWakeup,
     "  sldd alarm schedule [<time seconds>] - set scheduled wakeup via AlarmManager \n"
     "                                    - if timesec is zero, wakeup will be cancled \n"},
    {"off", 0, al_handler_offAlarmInterrupt,
     "  sldd alarm off     - off alarm interrupt\n"},

    {NULL, NULL, NULL, NULL}
};

bool commandActionAlarm(int32_t argc, char **argv) {

    if (argc == 0) {
        usage_alarm(NULL);
        return true;
    }

    DO_HANDLER(alarm_cmds, argv[0], argc-1, argv+1);
}

char* usage_alarm(char *cmd)
{

    printMessage(" sldd alarm [<command>] [parameters] - basic commend for alarm \n");
    printMessage("    <command> \n");
    PRINT_USAGE(alarm_cmds);
    return NULL;
}

static time_t parse_time(char *s)
{
    struct tm tm;
    int day = sl::atoi(s);
    int hour = 0;

    while (*s && *s != '.') {
        s++;
    }

    if (*s) {
        s++;
        hour = sl::atoi(s);
    }

    tm.tm_year = (day / 10000) - 1900;
    tm.tm_mon = (day % 10000) / 100 - 1;
    tm.tm_mday = day % 100;
    tm.tm_hour = hour / 10000;
    tm.tm_min = (hour % 10000) / 100;
    tm.tm_sec = hour % 100;
    tm.tm_isdst = -1;

    return mktime(&tm);
}

#define tv2Sec(t) ((t).tv_sec  + ((t).tv_usec + 500) / 1000000)


void sldd_alarm_poll_cb()
{

    printMessage(
    "==============================\n"
    "   alarm expired from HAL \n",
    "==============================\n");
}

static ThreadCondition wait_update;
class sldd_alarm_expire_listener : public BnAlarmExpireListener
{
    virtual void onAlarmExpired(const int32_t param)
    {
        printMessage(
        "==============================\n"
        "   alarm expired from AlarmManager \n",
        "==============================\n");
        wait_update.signal(1);
    }
};

static bool al_handler_setAlarm(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    int64_t alarmMs = strtol(argv[0], NULL, 10);
    alarmMs = alarmMs*1000;

    sp<IAlarmExpireListener> listener = new sldd_alarm_expire_listener();
    error_t error =
            SL_INTERFACE_GET(AlarmManagerService)->
            setAlarmFromNow(0, alarmMs, false, listener );
    printMessage(
    "==============================\n"
    "   Success setAlarmFromNow <%lld> \n", alarmMs,
    "==============================\n");

    wait_update.wait(1);
    printMessage(
    "==============================\n"
    "   Success polling after <%lld> \n", alarmMs,
    "==============================\n");
    return true;
}

static bool al_handler_getAlarm(int32_t argc, char **argv)
{

    return true;
}

static int fd = -1;
static bool al_handler_setAlarmHAL(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    int64_t alarmMs = strtol(argv[0], NULL, 10);

    if(fd == -1)
        fd = alarmInit();

    printMessage(
    "==============================\n"
    "   Success setAlarm <%lld> \n", alarmMs,
    "==============================\n");

    sl_alarm_poll_cb_t cb = sldd_alarm_poll_cb;
    alarmSetAlarm(alarmMs, cb);
    alarmPollInt();

    printMessage(
    "==============================\n"
    "   Success polling after <%lld> \n", alarmMs,
    "==============================\n");
    return true;
}

static bool al_handler_getAlarmHAL(int32_t argc, char **argv)
{
    if(fd == -1)
        fd = alarmInit();

    int64_t alarmMs = alarmGetAlarm();
    printMessage(
    "==============================\n"
    "   Success getAlarm <%lld> \n", alarmMs,
    "==============================\n");
    return true;
}

static bool al_handler_setScheduledWakeup(int32_t argc, char **argv)
{
    char time_buf[1024] = {0, };
    struct tm tm;
    time_t time;
    tm = SL_INTERFACE_GET(SystemManagerService)->getCurrentTime();
    //strftime(time_buf, sizeof(time_buf), "%a %b %e %H:%M:%S %Z %Y" ,&tm);
    strftime(time_buf, sizeof(time_buf), "%F %T" ,&tm);
    printMessage("==============================\n");
    printMessage("   current time\n");
    printMessage("  %s \n", time_buf);
    printMessage("==============================\n");
    int ret = ScheduledWakeup::ERR_NONE;

    int64_t alarmSec = 0;
    if (argv[0] == NULL)
    {
        return false;
    }
    else if(sl::atoi(argv[0]) == 0)
    {
        ret = SL_INTERFACE_GET(AlarmManagerService)->setScheduledWakeup(0, 0);
    }
    else
    {

        alarmSec = strtoll(argv[0], NULL, 10);
        LOGE("alarm seconds : %lld", alarmSec);

        ret = SL_INTERFACE_GET(AlarmManagerService)->setScheduledWakeup(0, alarmSec);
    }
    time = mktime(&tm);
    time += alarmSec;
    strftime(time_buf, sizeof(time_buf), "%F %T" , gmtime(&time));

    if (ret == ScheduledWakeup::ERR_NONE) {
        printMessage(
        "==============================\n"
        "   Success schedule %s <%s> \n", (alarmSec ? "set" : "cancel"), time_buf);
        printMessage(
        "==============================\n");
    } else {
        char *msg = "UNKNOWN_ERROR";
        switch (ret) {
        case ScheduledWakeup::ERR_UNAVAILABLE_OPMODE:
            msg = "Unavailable OperationMode";
            break;
        case ScheduledWakeup::ERR_STARTTIME_IN_THE_PAST:
            msg = "Start time is in the past";
            break;
        case ScheduledWakeup::ERR_STARTTIME_TOOFAR_INTO_FUTURE:
            msg = "Start time is too far";
            break;
        case ScheduledWakeup::ERR_NOT_ALLOWED:
            msg = "Not Allowed";
            break;
        default:
            break;
        }
        printMessage(
        "==============================\n"
        "   Fail schedule %s <%s> \n", (alarmSec ? "set" : "cancel"), msg);
        printMessage(
        "==============================\n");
    }
    return true;
}

static bool al_handler_offAlarmInterrupt(int32_t argc, char **argv)
{
    int64_t time;

    time=sl::atoi(argv[0]);

    SL_INTERFACE_GET(AlarmManagerService)->OffAlarmInterrupt(time);
    printMessage(
    "==============================\n"
    "   Success Alarm Interrupt Off  \n",
    "==============================\n");
    return true;
}

void register_alarm()
{
    registerCommands(MODULE_ALARM_MANAGER, NULL, alarm_cmds);
}
