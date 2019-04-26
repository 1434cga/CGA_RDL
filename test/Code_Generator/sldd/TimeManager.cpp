/**
* \file     SystemManager.cpp
* \brief    Implementation of SystemManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author   daehun.ju
* \date     2018.09.29
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#include "Error.h"
#include "system_property.h"
#include "TCUProtocol.h"

#include "man.h"
#include "sldd_common.h"
#include "TimeManager.h"
#include "services/TimeManagerService/TimeManager.h"

#define LOG_TAG "SLDD_TimeManager"

enum tm_commands {
    TM_CMD_INIT = 1,
    TM_CMD_SET_TIME,
    TM_CMD_GET_TIME,
    TM_CMD_TZOFFET,
    TM_CMD_DST,
    TM_CMD_TEST_MODE,
    TM_CMD_TEST_PRI,
    TM_CMD_TEST_PROP,
    TM_CMD_TEST_TIME_UTIL,
    TM_CMD_TEST_TIME_NOTI
};

sldd_cmd_table_t tm_cmds[] = {
    {"settime",         TM_CMD_SET_TIME,        handler_setTime_t,
     "sldd tm settime \n"},
    {"gettime",         TM_CMD_GET_TIME,        handler_getTime_t,
     "sldd tm gettime \n"},
    {"tzoffset",        TM_CMD_TZOFFET,         handler_TZoff_t,
     "sldd tm tzoffset \n"},
    {"dst",             TM_CMD_DST,             handler_DST_t,
     "sldd tm dst \n"},
    {"testtime",        TM_CMD_TEST_MODE,       handler_testTime_t,
     "sldd tm testtime \n"},
    {"testtimepri",     TM_CMD_TEST_PRI,        handler_testTimePri_t,
     "sldd tm testtimepri \n"},
    {"testtimeprop",    TM_CMD_TEST_PROP,       handler_testTimeProp_t,
     "sldd tm testtimeprop [cmd] \n"},
    {"testtimeutils",   TM_CMD_TEST_TIME_UTIL,  handler_testTimeUtils_t,
     "sldd tm testtimeutils \n"},
    {"testtimenoti",    TM_CMD_TEST_TIME_NOTI,  handler_testTimeNoti_t,
     "sldd tm testtimenoti \n"},
    {NULL, NULL, NULL, NULL}
};

void register_time()
{
    registerCommands(MODULE_TIME_MANAGER, NULL, tm_cmds);
}

bool commandActionTime(int32_t argc, char **argv) {
    if (argc == 0) {
        usage_time(nullptr);

        return true;
    }

    DO_HANDLER(tm_cmds, argv[0], argc-1, argv+1);
}

char* usage_time(char *cmd) {
    printMessage(" perform TimeManagerService's functions \n");
    printMessage(" sldd tm [<command>] \n");
    PRINT_USAGE(tm_cmds);

    return nullptr;
}

/**
 * sldd tm settime [0 - 7] [YYYYMMDD.HHMMSS] [option:offset] [option:dst]
 */
bool handler_setTime_t(int32_t argc, char **argv)
{
    LOGD("handler_setTime_t()");
    if ((argc < 1) && (4 > argc)) {
        printMessage("-> You used the wrong systemtime command!! \n");
        printMessage("-> Fail!! Please check : sldd tm settime help \n");
        return true;
    }

    if (!strcmp(argv[0],"help")) {
        printMessage("sldd tm settime [0 - 7] [YYYYMMDD.HHMMSS] [option:offset] [option:dst] \n");
        printMessage("type list is... \n");
        printMessage("[0] INIT_TIME \n");
        printMessage("[1] MANUAL_TIME \n");
        printMessage("[2] RTC_TIME \n");
        printMessage("[3] NETWORK_TIME \n");
        printMessage("[4] GPS_TIME \n");
        printMessage("[5] HUD_TIME \n");
        printMessage("[6] SECURE_TIME \n");
        printMessage("[7] NONE_TIME : super time \n");
        return true;
    }
    int32_t time_type = sl::atoi(argv[0]);
    int32_t offset = timeservice::UNKNOWN;
    int32_t dst = timeservice::UNKNOWN;

    if ((timeservice::INIT_TIME <= time_type) && (time_type <= timeservice::NONE_TIME)) {
        TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
        int32_t ret = E_ERROR;

        if ((0 < argc) && (argc < 5)) {
            offset = sl::atoi(argv[2]);
            dst = sl::atoi(argv[3]);
            ret = test_setime(time_type, argv[1], offset, dst);
        } else {
            ret = test_setime(time_type, argv[1]);
        }

        if (E_OK == ret) {
            printMessage("  [Pass]    set time (%d) [%s] \n", time_type, argv[1]);
        } else if (E_REJECTED == ret) {
            printMessage("  [Pass]    now time type (%d), block to set time (%d) [%s] \n", tm.getTimeType(), time_type, argv[1]);
        } else {
            printMessage("  [Fail]    set time (%d) [%s] \n", time_type, argv[1]);
        }
    } else {
        printMessage("  Please check : sldd tm settime help \n");
    }

    return true;
}

/**
 * sldd tm gettime
 * sldd tm gettime user
 * sldd tm gettime milli
 * sldd tm gettime boot
 */
bool handler_getTime_t(int32_t argc, char **argv)
{
    LOGD("handler_getTime_t()");
    if (argc == 0) {
        test_gettime();
    } else if (argc == 1) {
        if (!strcmp(argv[0],"help")) {
            printMessage("sldd tm gettime \n");
            printMessage("sldd tm gettime [user] \n");
            printMessage("sldd tm gettime [milli] \n");
            printMessage("sldd tm gettime [boot] \n");
            return true;
        } else if (!strcmp(argv[0],"user")) {
            test_gettime_user();
        } else if (!strcmp(argv[0],"milli")) {
            TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
            int64_t millis = tm.getCurrentMilliSec();
            printMessage("  sysem time(millis) is %"PRId64" \n", millis);
        }  else if (!strcmp(argv[0],"boot")) {
            TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
            int64_t millis = tm.getBootMilliSec();
            if (-1LL == millis) {
                printMessage("  Can not get a Time!! need to set a valid time!! \n");
            }
            printMessage("  after boot(millis) is %"PRId64" \n", millis);
        } else {
            printMessage("  Please check : sldd tm gettime help \n");
        }
    }

    return true;
}

/**
 * sldd tm tzoffset [0 - 7] [ofsset]
 * sldd tm tzoffset
 */
bool handler_TZoff_t(int32_t argc, char **argv)
{
    LOGD("handler_TZoff_t()");
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    int32_t ret = E_OK;

    if (argc == 0) {
        // get dst
        ret = tm.getOffset();
        printMessage("  [Pass]    tzoffet (%d) \n", ret);
    } else if (argc == 1) {
        if (!strcmp(argv[0],"help")) {
            printMessage("sldd tm tzoffset [0 - 7] [ofsset] \n");
            printMessage("sldd tm tzoffset \n");
            return true;
        } else {
            printMessage("  Please check : sldd tm tzoffset help \n");
        }
    } else if (argc == 2) {
        int32_t time_type = sl::atoi(argv[0]);
        int32_t tzoffset = sl::atoi(argv[1]);
        tm.setOffset(time_type, tzoffset);

        if (E_OK == ret) {
            printMessage("  [Pass]    set tzoffset (%d) (%d) \n", time_type, tzoffset);
        } else {
            printMessage("  [Fail]    set tzoffset (%d) (%d) \n", time_type, tzoffset);
        }
    } else {
        printMessage("  Please check : sldd tm tzoffset help \n");
    }

    return true;
}

/**
 * sldd tm dst [0 - 7] [dst]
 * sldd tm dst
 */
bool handler_DST_t(int32_t argc, char **argv)
{
    LOGD("handler_DST_t()");
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    int32_t ret = E_OK;

    if (argc == 0) {
        // get dst
        ret = tm.getDayLightTime();
        printMessage("  [Pass]    dst (%d) \n", ret);
    } else if (argc == 1) {
        if (!strcmp(argv[0],"help")) {
            printMessage("sldd tm dst [0 - 7] [dst] \n");
            printMessage("sldd tm dst \n");
            return true;
        } else {
            printMessage("  Please check : sldd tm dst help \n");
        }
    } else if (argc == 2) {
        int32_t time_type = sl::atoi(argv[0]);
        int32_t dst = sl::atoi(argv[1]);
        tm.setDayLightTime(time_type, dst);

        if (E_OK == ret) {
            printMessage("  [Pass]    set dst (%d) (%d) \n", time_type, dst);
        } else {
            printMessage("  [Fail]    set dst (%d) (%d) \n", time_type, dst);
        }
    } else {
        printMessage("  Please check : sldd tm dst help \n");
    }

    return true;
}

/* sldd tm testtime */
bool handler_testTime_t()
{
    LOGD("handler_testTime_t() [time test] ");
    printMessage("\============================================================ \n");
    printMessage("TimeManagerService APIs Test \n");
    printMessage("\============================================================ \n");

    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    int32_t time_type = tm.getTimeType();

    if (timeservice::INIT_TIME== time_type) {
        printMessage("[test 1]  when time_type is INIT_TIME, gettime \n");
        int32_t is_init = test_gettime_user();
        if (E_ERROR == is_init) {
            printMessage("  [Pass]  when time_type is INIT_TIME, Does not support to get time \n");
        } else {
            printMessage("  [Fail] \n");
        }
    }

    // set 3:33:33 1983-8-4
    printMessage("[test 2]  set time GMT 3:33:33 1983-8-4, without constraints \n");
    if (E_OK == test_setime(timeservice::NONE_TIME, 428816013000)) {
        printMessage("  [Pass]  set time (7) GMT Time 3:33:33 1983-8-4\n");
    } else {
        printMessage("  [Fail]  set time (7) GMT Time 3:33:33 1983-8-4\n");
    }

    // get 3:33:33 1983-8-4
    printMessage("[test 3]  get time \n");
    test_gettime();

    // set 4:44:44 1984-10-30 -540 1
    printMessage("[test 4]  set time GMT 4:44:44 1984-10-30 -540 1, without constraints \n");
    if (E_OK == test_setime(timeservice::NONE_TIME, 467959484000, -540, 1)) {
        printMessage("  [Pass]  set time (7) GMT Time 4:44:44 1984-10-30 -540 1 \n");
    } else {
        printMessage("  [Fail]  set time (7) GMT Time 4:44:44 1984-10-30 -540 1 \n");
    }

    // get 4:44:44 1984-10-30 -540 1
    printMessage("[test 5]  get time \n");
    test_gettime_user();

    // set 5:55:55 1984-10-30 540 0
    printMessage("[test 6]  set time GMT 5:55:55 1984-10-30 540 0, without constraints \n");
    if (E_OK == test_setime(timeservice::NONE_TIME, 467963755000, 540, 0)) {
        printMessage("  [Pass]  set time (7) GMT Time 5:55:55 1984-10-30 540 0 \n");
    } else {
        printMessage("  [Fail]  set time (7) GMT Time 5:55:55 1984-10-30 540 0 \n");
    }

    // get 5:55:55 1984-10-30 540 0
    printMessage("[test 7]  get time \n");
    test_gettime_user();

    // get millis about 467963755000 + alpha
    int64_t millis = tm.getCurrentMilliSec();
    printMessage("[test 8]  before setting GMT 467963755000, now GMT millis is %"PRId64" \n", millis);

    printMessage("[test 9]  time priority \n");
    default_test_pri();

    return true;
}

bool handler_testTimePri_t(int32_t argc, char **argv)
{

    if (argc == 0) {
        printMessage("default test!! \n");
        default_test_pri();
    } else {
        if (strncmp(argv[0], "help", 4) == 0) {
            printMessage("sldd tm testtimepri \n");
            printMessage("sldd tm testtimepri [clear] \n");
            printMessage("sldd tm testtimepri [init] \n");
            printMessage("sldd tm testtimepri [manual] \n");
            printMessage("sldd tm testtimepri [rtc] \n");
            printMessage("sldd tm testtimepri [network] \n");
            printMessage("sldd tm testtimepri [gps] \n");
            printMessage("sldd tm testtimepri [hud] \n");
            printMessage("sldd tm testtimepri [secure] \n");
            return true;
        }
        TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
        timeservice::test_time_service_t time_service = {};
        time_service.test_cmd = timeservice::COMMAND_TEST_TIME_PRIORITY;
        time_service.arg1 = timeservice::INIT_TIME;
        time_service.arg2 = timeservice::DISABLE;

        if (strncmp(argv[0], "clear", 5) == 0) {
            time_service.arg2 = timeservice::ENABLE;
        } else if (strncmp(argv[0], "init", 4) == 0) {
            time_service.arg1 = timeservice::INIT_TIME;
        } else if (strncmp(argv[0], "manual", 6) == 0) {
            time_service.arg1 = timeservice::MANUAL_TIME;
        } else if (strncmp(argv[0], "rtc", 3) == 0) {
            time_service.arg1 = timeservice::RTC_TIME;
        } else if (strncmp(argv[0], "network", 7) == 0) {
            time_service.arg1 = timeservice::NETWORK_TIME;
        } else if (strncmp(argv[0], "gps", 3) == 0) {
            time_service.arg1 = timeservice::GPS_TIME;
        } else if (strncmp(argv[0], "hud", 3) == 0) {
            time_service.arg1 = timeservice::HUD_TIME;
        } else if (strncmp(argv[0], "secure", 6) == 0) {
            time_service.arg1 = timeservice::SECURE_TIME;
        } else {
            printMessage("  Please check : sldd tm testtimepri help \n");
        }
        printMessage("test set [%s] \n", argv[0]);

        if (E_OK == tm.testTimeService(time_service)) {
            printMessage("  [Set]  [%s] \n", argv[0]);
        } else {
            printMessage("  [Block]  [%s] \n", argv[0]);
        }
    }

    return true;
}

bool handler_testTimeProp_t(int32_t argc, char **argv)
{
    if (argc == 0) {
        printMessage("-> You used the wrong TEST command!! \n");
        printMessage("-> Fail!! Please check : sldd tm testtimeprop help \n");
        return true;
    } else {
        if (strncmp(argv[0], "help", 4) == 0) {
            printMessage("sldd tm testtimeprop [set] \n");
            printMessage("sldd tm testtimeprop [get] \n");
            return true;
        }
    }
    LOGD("handler_testTimeProp_t() [time test] ");

    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    timeservice::test_time_service_t time_service = {};

    if (!strcmp(argv[0],"set")) {
        time_service.test_cmd = timeservice::COMMAND_TEST_SET_PROP;
        tm.testTimeService(time_service);
    } else if (!strcmp(argv[0],"get")) {
        time_service.test_cmd = timeservice::COMMAND_TEST_GET_PROP;
        tm.testTimeService(time_service);
    } else {
        printMessage("sldd tm testtimeprop [set] \n");
        printMessage("sldd tm testtimeprop [get] \n");
    }

    printMessage("Please check [time test] in main logs\n");
    return true;
}

/**
 * sldd tm testTimeUtils
 */
bool handler_testTimeUtils_t(int32_t argc, char **argv)
{
    if (argc > 0) {
        if (strncmp(argv[0], "help", 4) == 0) {
            printMessage("sldd tm testTimeUtils \n");
        } else {
            printMessage("-> You used the wrong TEST command!! \n");
            printMessage("-> Fail!! Please check : sldd tm testtimeutils help \n");
       }
        return true;
    }
    LOGD("handler_testTimeUtils_t() [time test] ");

    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    timeservice::test_time_service_t time_service = {};
    time_service.test_cmd = timeservice::COMMAND_TEST_TIME_UTILS;
    tm.testTimeService(time_service);

    int64_t sec_msb_64 = static_cast<int64_t>(time_service.arg1) & 0x00000000FFFFFFFF;
    int64_t sec_lsb_64 = static_cast<int64_t>(time_service.arg2) & 0x00000000FFFFFFFF;
    int64_t mill_sec_msb_64 = static_cast<int64_t>(time_service.arg3) & 0x00000000FFFFFFFF;
    int64_t mill_sec_lsb_64 = static_cast<int64_t>(time_service.arg4) & 0x00000000FFFFFFFF;

    int64_t sec = (sec_msb_64 << 32) | sec_lsb_64;
    int64_t mill_sec = (mill_sec_msb_64 << 32) | mill_sec_lsb_64;

    printMessage("sec_msb_64            = 0x%016"PRIx64",   sec_lsb_64 = %016"PRIx64"\n", sec_msb_64, sec_lsb_64);
    printMessage("sec_msb_64 << 32      = 0x%016"PRIx64",   sec_lsb_64 = %016"PRIx64"\n", (sec_msb_64 << 32), sec_lsb_64);
    printMessage("sec                   = %"PRId64"\n", sec);

    printMessage("mill_sec_msb_64       = 0x%016"PRIx64",   mill_sec_lsb_64 = %016"PRIx64"\n", mill_sec_msb_64, mill_sec_lsb_64);
    printMessage("mill_sec_msb_64 << 32 = 0x%016"PRIx64",   mill_sec_lsb_64 = %016"PRIx64"\n", (mill_sec_msb_64 << 32), mill_sec_lsb_64);
    printMessage("mill_sec              = %"PRId64"\n", mill_sec);

    return true;
}

bool handler_testTimeNoti_t(int32_t argc, char **argv)
{
    if (argc > 0) {
        if (strncmp(argv[0], "help", 4) == 0) {
            printMessage("sldd tm testTimeNoti \n");
        } else {
            printMessage("-> You used the wrong TEST command!! \n");
            printMessage("-> Fail!! Please check : sldd tm testtimenoti help \n");
       }

        return true;
    }
    LOGD("handler_testTimeNoti_t() [time test] ");

    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    timeservice::test_time_service_t time_service = {};
    time_service.test_cmd = timeservice::COMMAND_TEST_TIME_NOTIFY;
    tm.testTimeService(time_service);

    return true;
}

error_t test_setime(int32_t type, int64_t timeMs, int32_t offset, int32_t dst, int32_t forced)
{
    error_t ret = E_ERROR;
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    if (timeservice::NONE_TIME == type) {
        ret = tm.setCurrentTime(type, timeMs, offset, dst, timeservice::ENABLE);
    } else {
        ret = tm.setCurrentTime(type, timeMs);
    }
    return ret;
}

error_t test_setime(int32_t type, const char* timeMs, int32_t offset, int32_t dst, int32_t forced)
{
    error_t ret = E_ERROR;
    struct timeval timedate = {};
    timedate.tv_sec = parse_SystemTime(timeMs);
    timedate.tv_usec = 0LL;
    int64_t millis = static_cast<int64_t>((timedate.tv_sec * 1000LL) + (timedate.tv_usec / 1000LL));

    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    if (timeservice::NONE_TIME == type) {
        ret = tm.setCurrentTime(type, millis, offset, dst, timeservice::ENABLE);
    } else {
        ret = tm.setCurrentTime(type, millis);
    }

    return ret;
}

void test_gettime()
{
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    int32_t time_type = tm.getTimeType();

    if (timeservice::INIT_TIME == time_type) {
        printMessage("  Maybe Time is invalid.!! \n");
    }

    char time_buf[1024] = {'\0', };
    struct tm time_date;
    time_date = tm.getCurrentTime();
    strftime(time_buf, sizeof(time_buf), "%F %T" ,&time_date);
    printMessage("  ( %d ), [ %s ] \n", time_type, time_buf);
}

error_t test_gettime_user()
{
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    timeservice::timedata_t timedata = {};
    error_t time_type = E_ERROR;

    time_type = tm.getCurrentTime(&timedata);
    if (E_ERROR == time_type) {
        printMessage("  Can not get a Time!! need to set a valid time!! \n");
        printMessage("  system time is -1 \n");
    } else {
        printMessage("  ( %d ) ", time_type);
        printMessage("  GMT Time %d:%d:%d ", timedata.hour, timedata.min, timedata.sec);
        printMessage("  %d-%d-%d, ", timedata.year, timedata.mon, timedata.mday);
        printMessage("  (%d) tzoffset (%d) isdst \n", timedata.tz_offset, timedata.is_dst);
    }

    return time_type;
}

time_t parse_SystemTime(char *s)
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

    /*ex) 20151204.190400*/
    tm.tm_year = (day / 10000) - 1900;
    tm.tm_mon = (day % 10000) / 100 - 1;
    tm.tm_mday = day % 100;
    tm.tm_hour = hour / 10000;
    tm.tm_min = (hour % 10000) / 100;
    tm.tm_sec = hour % 100;
    tm.tm_isdst = -1;

    return mktime(&tm);
}

void default_test_pri()
{
    TimeManager& tm = TimeManager::getInstance(NAD_MODULE_TIME_MGR);
    timeservice::test_time_service_t time_service = {};
    time_service.test_cmd = timeservice::COMMAND_TEST_TIME_PRIORITY;
    time_service.arg1 = timeservice::NETWORK_TIME;
    time_service.arg2 = timeservice::DISABLE;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [NETWORK_TIME] \n");
    else printMessage("  [Fail]  set [NETWORK_TIME] - ok \n");

    time_service.arg1 = timeservice::GPS_TIME;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [GPS_TIME] \n");
    else printMessage("  [Fail]  set [GPS_TIME] \n");

    time_service.arg1 = timeservice::GPS_TIME;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [GPS_TIME] \n");
    else printMessage("  [Fail]  set [GPS_TIME] \n");

    time_service.arg1 = timeservice::NETWORK_TIME;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [NETWORK_TIME] \n");
    else printMessage("  [Fail]  set [NETWORK_TIME] \n");

    time_service.arg1 = timeservice::SECURE_TIME;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [SECURE_TIME] \n");
    else printMessage("  [Fail]  set [SECURE_TIME] \n");

    time_service.arg1 = timeservice::NETWORK_TIME;
    if (E_OK != tm.testTimeService(time_service)) printMessage("  [Pass]  blocked [NETWORK_TIME] \n");
    else printMessage("  [Fail]  blocked [NETWORK_TIME] \n");

    time_service.arg1 = timeservice::GPS_TIME;
    if (E_OK != tm.testTimeService(time_service)) printMessage("  [Pass]  blocked [GPS_TIME] \n");
    else printMessage("  [Fail]  blocked [GPS_TIME] \n");

    time_service.arg1 = timeservice::SECURE_TIME;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  set [SECURE_TIME] \n");
    else printMessage("  [Fail]  set [SECURE_TIME] \n");

    time_service.arg1 = timeservice::INIT_TIME;
    time_service.arg2 = timeservice::ENABLE;
    if (E_OK == tm.testTimeService(time_service)) printMessage("  [Pass]  clear TIME_PRIORITY \n");
    else printMessage("  [Fail]  clear TIME_PRIORITY \n");
}