/**
* \file    SystemManager.cpp
* \brief     Implementation of SystemManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       cheolhwan.bae
* \date       2015.12.29
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <cstdio>
#include <time.h>
#include <sys/time.h>

#include <SystemManagerService/ISystemManagerService.h>
#include <SystemManagerService/ISystemManagerServiceType.h>

#include "services/SystemManagerService/SystemTimeUtils.h"
#include "man.h"
#include "sldd_common.h"
#include "SystemManager.h"

#define LOG_TAG "SystemManager"

using namespace android;

enum sm_commands {
    SM_CMD_INIT =1,
    SM_CMD_GET_ANT_STATUS,
    SM_CMD_SET_CURRNET_ANT,
    SM_CMD_SET_ANT_PATH,
//    SM_CMD_NOTIFY_ANT_STATUS,
    SM_CMD_GPS_ANT_IN_BUB,
    SM_CMD_ANT_TEST,
    SM_CMD_SET_SYSTEM_TIME,
//    SM_CMD_GET_GPIO_VALUE,
//    SM_CMD_REQ_EXT_AP_GPIO_VALUE,
//    SM_CMD_RQE_EXT_SIM_STATUS,
    SM_CMD_SET_SYS_PROP,
    SM_CMD_GET_SYS_PROP,
    SM_CMD_ANT_TEST_FOR_QMI
};

static bool handler_getAntStatus(int32_t argc, char **argv);
static bool handler_switchAnt(int32_t argc, char **argv);
static bool handler_switchAntPath(int32_t argc, char **argv);
/*
// internal API Test
static bool handler_notifyAntStatus(int32_t argc, char **argv);
*/
static bool handler_setGPSAntDetectMode(int32_t argc, char **argv);
static bool handler_setSystemTime(int32_t argc, char **argv);
static bool handler_getSystemTime(int32_t argc, char **argv);
/*
static bool handler_readGPIOvalue(int32_t argc, char **argv);
static bool handler_reqExtApGPIOvalue(int32_t argc, char **argv);
static bool handler_requestExtSIMStatus(int32_t argc, char **argv);
*/
static bool handler_getSystemProperty(int32_t argc, char **argv);
static bool handler_QMI_TEST(int32_t argc, char **argv);
static bool handler_antTest(int32_t argc, char **argv);

sldd_cmd_table_t sm_cmds[] = {
    {"getAntStatus",    SM_CMD_GET_ANT_STATUS,        handler_getAntStatus,
     "sldd sys getAntStatus [ant_mode]  - get antenna status of ant_mode\n"},
    {"switchAntMode",   SM_CMD_SET_CURRNET_ANT,       handler_switchAnt,
     "sldd sys switchAntMode [ant_mode]  - set antenna mode to ant_mode\n"},
    {"switchAntPath",   SM_CMD_SET_ANT_PATH,          handler_switchAntPath,
     "sldd sys switchAntPath [ant_mode][ant_path]  - set antenna path to ant_path in ant-mode\n"},
/*
    {"notifyAntStatus", SM_CMD_NOTIFY_ANT_STATUS,     handler_notifyAntStatus,
     "sldd sys notifyAntStatus - notify antenna status\n"},
*/
    {"setGPSAntDetect", SM_CMD_GPS_ANT_IN_BUB,        handler_setGPSAntDetectMode,
     "sldd sys setGPSAntDetectMode [flag] \n"},
    {"antTest",        SM_CMD_ANT_TEST,              handler_antTest,
     "sldd sys antTest  - total antenna test\n"},
    {"setSystemTime",   SM_CMD_SET_SYSTEM_TIME,       handler_setSystemTime,
     "sldd sys setSystemTime [time_type] [YYYYMMDD.HHMMSS]  - set System Time\n"},
/*
    {"readGPIOe",       SM_CMD_GET_GPIO_VALUE,        handler_readGPIOvalue,
     "sldd sys readGPIOvalue [gpio_num]  - read value of gpio\n"},
    {"reqExtApGPIO",    SM_CMD_REQ_EXT_AP_GPIO_VALUE, handler_reqExtApGPIOvalue,
     "sldd sys reqExtApGPIOvalue [gpio_num]  - request gpio value of Ext.AP\n"},
    {"reqExtSIMStatus", SM_CMD_RQE_EXT_SIM_STATUS,    handler_requestExtSIMStatus,
     "sldd sys handler_requestExtSIMStatus      - request status of Ext.SIM \n"},
*/
    {"getProp",         SM_CMD_GET_SYS_PROP,          handler_getSystemProperty,
     "sldd sys getProp [property]  - get the property of system manager \n"},
    {"qmiTest",         SM_CMD_ANT_TEST_FOR_QMI,      handler_QMI_TEST,
     "sldd sys qmiTest [flag]  - set qmi antenna flag for testing \n"},

    {NULL, NULL, NULL, NULL}
};


SL_INTERFACE_DECLARE2(SystemManagerService, SYSTEM_SRV_NAME);

bool commandActionSystem(int32_t argc, char **argv)
{

    if (argc == 0) {
        usage_system(NULL);
        return true;
    }

    DO_HANDLER(sm_cmds, argv[0], argc-1, argv+1);
}

char* usage_system(char *cmd) {
    printMessage(" sldd sys [<command>]         - perform SystemManager's functions \n");
    printMessage("    <command> \n");
    PRINT_USAGE(sm_cmds);
    return NULL;
}

void usage_setSystemTime()
{
    printMessage("\nNeed to Command Set Time Option \n");
    printMessage("[time_type] : 1~4 \n");
    printMessage("[timeMs]    : YYYYMMDD.HHMMSS \n");
}

void usage_AntennaUse() {
    printMessage("\nNeed to Command Antenna Option \n");
    printMessage("[ant_mode] : MAIN | SUB | GNSS \n");
    printMessage("[ant_path] : INTERNAL | EXTERNAL \n");
}

/*
 * sldd sys getAntStatus [ main / sub / gnss ]
 */
bool handler_getAntStatus(int32_t argc, char **argv)
{
    if (argc == 0) {
        usage_AntennaUse();
        return true;
    }

    int32_t ret = E_ERROR;
    ant_status_t ant_status = {}; //fixed: Uninitialized scalar variable
    int8_t ant_type = 0;

    if (!strcmp(argv[0],"MAIN") || !strcmp(argv[0],"main"))
    {
        ant_type = 0;
        ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_MAIN, &ant_status);
    }
    else if (!strcmp(argv[0],"SUB") || !strcmp(argv[0],"sub"))
    {
        ant_type = 1;
        ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_SUB, &ant_status);
    }
    else if (!strcmp(argv[0],"GNSS") || !strcmp(argv[0],"gnss"))
    {
        ant_type = 2;
        ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_GNSS, &ant_status);
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("The test code is not processed due to E_INVALID_PARAM \n");
        printMessage("==============================\n");
        return true;
    }

    if(ret == E_OK)
    {
        printMessage("\n==============================\n");
        printMessage("Success getting antenna status info \n\n");
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("Fail getting antenna status info : \n");
        printMessage("The test code is not processed due to [%d] \n", ret);
        printMessage("==============================\n");
        return true;
    }

    printMessage("Antenna(%d), adc value(%d) detectfaultcnt(%d) status(%d) \n",
        ant_type, ant_status.adc_value, ant_status.detect_fault_cnt, ant_status.status);
    printMessage("==============================\n");

    return true;
}

/*
 * sldd sys switchAntMode [ main / sub ]
 */
bool handler_switchAnt(int32_t argc, char **argv)
{
    int32_t set_ret = E_ERROR;
    int32_t get_ret = E_ERROR;
    int32_t ant_mode = ANTENNA_MAIN;

    printMessage("==============================\n");
    if (argc == 0) {
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getCurrentAntenna();

        printMessage("Current Antenna Mode(%d) \n", get_ret);
        printMessage("==============================\n");

        usage_AntennaUse();

        return true;
    }

    if (!strcmp(argv[0],"MAIN") || !strcmp(argv[0],"main")) {
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchCurrentAntenna(ANTENNA_MAIN);
        printMessage("Switching Antenna Mode(%d)... \n", ANTENNA_MAIN);
    } else if (!strcmp(argv[0],"SUB") || !strcmp(argv[0],"sub")) {
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchCurrentAntenna(ANTENNA_SUB);
        ant_mode = ANTENNA_SUB;
        printMessage("Switching Antenna Mode(%d)... \n", ANTENNA_SUB);
    } else {
        set_ret = E_INVALID_PARAM;
    }

    get_ret = SL_INTERFACE_GET(SystemManagerService)->getCurrentAntenna();
    if (set_ret == E_OK &&  ant_mode == get_ret) {
        printMessage("Switch Success!! Get Current Antenna Mode(%d) \n", get_ret);
    } else {
        printMessage("Fail Switching Antenna Mode by cause(%d) \n", set_ret);
    }
    printMessage("==============================\n");

    return true;
}

/*
 * sldd sys switchAntPath [ main internal / main external ]
 * sldd sys switchAntPath [ sub internal   / sub external   ]
*/
bool handler_switchAntPath(int32_t argc, char **argv)
{
    int32_t set_ret = E_ERROR;
    int32_t get_ret = E_ERROR;
    int32_t ant_path = INTERNAL_ANT;

    printMessage("\n==============================\n");
    if (argc < 2) {
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_MAIN);
        printMessage("Getting Main Antenna Path(%d) \n", get_ret);
        get_ret = E_ERROR;
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_SUB);
        printMessage("Getting Sub Antenna Path(%d) \n", get_ret);

        usage_AntennaUse();
        return true;
    }

    if (!strcmp(argv[0],"MAIN") || !strcmp(argv[0],"main")) {
        if(!strcmp(argv[1],"INTERNAL") || !strcmp(argv[1],"internal")) {
            set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_MAIN, INTERNAL_ANT);
            printMessage("Switching Main Antenna Path(%d)... \n", INTERNAL_ANT);
        } else if(!strcmp(argv[1],"EXTERNAL") || !strcmp(argv[1],"external")) {
            set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_MAIN, EXTERNAL_ANT);
            ant_path = EXTERNAL_ANT;
            printMessage("Switching Main Antenna Path(%d)... \n", EXTERNAL_ANT);
        } else {
            set_ret = E_INVALID_PARAM;
        }

        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_MAIN);
    } else if (!strcmp(argv[0],"SUB") || !strcmp(argv[0],"sub")) {
        if(!strcmp(argv[1],"INTERNAL") || !strcmp(argv[1],"internal")) {
            set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_SUB, INTERNAL_ANT);
            printMessage("Switching Sub Antenna Path(%d)... \n", INTERNAL_ANT);
        } else if(!strcmp(argv[1],"EXTERNAL") || !strcmp(argv[1],"external")) {
            set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_SUB, EXTERNAL_ANT);
            ant_path = EXTERNAL_ANT;
            printMessage("Switching Sub Antenna Path(%d)... \n", EXTERNAL_ANT);
        } else {
            set_ret = E_INVALID_PARAM;
        }

        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_SUB);
    }

    if (set_ret == E_OK && ant_path == get_ret) {
        printMessage("Switch Success!! Get Antenna Path(%d) \n", get_ret);
    } else {
        printMessage("Fail Switching Antenna Path by cause(%d) \n", set_ret);
    }
    printMessage("==============================\n");

    return true;
}

/*
// Internal API Test
bool handler_notifyAntStatus(int32_t argc, char **argv)
{
    int32_t ret = E_ERROR;

    ret = SL_INTERFACE_GET(SystemManagerService)->requestNotifyAntStatus(0);

    if (ret == E_ERROR) {
        printMessage("\n==============================\n");
        printMessage("Fail notifying antenna status \n");
        printMessage("The test code is not processed due to [%d] \n", ret);
        printMessage("==============================\n");
    } else {
        printMessage("\n==============================\n");
        printMessage("Success notifying antenna\n");
        printMessage("==============================\n");
    }

    return true;
}
*/

/* sldd sys setGPSAntDetectMode on/off */
bool handler_setGPSAntDetectMode(int32_t argc, char **argv)
{
    int32_t error;

    LOGV("%s() : ", __func__);
    if(!strcmp(argv[0],"on"))
    {
        error = SL_INTERFACE_GET(SystemManagerService)->setGPSAntDetectMode(1UL);
    }
    else if(!strcmp(argv[0],"off"))
    {
        error = SL_INTERFACE_GET(SystemManagerService)->setGPSAntDetectMode(0UL);
    }
    else
    {
        return false;
    }

    if(!strcmp(argv[0],"on"))
    {
        if(error == E_OK)
        {
            printMessage("\n==============================\n");
            printMessage("   Success setting 'On' for detecting gps antenna in BUB Mode. \n");
            printMessage("==============================\n");
            return true;
        }
        else
        {
            printMessage("\n==============================\n");
            printMessage("   Fail setting 'On' for detecting gps antenna in BUB Mode : \n");
            printMessage("     The test code is not processed due to %d\n", error);
            printMessage("==============================\n");
        }
    }
    else if(!strcmp(argv[0],"off"))
    {
        if(error == E_OK)
        {
            printMessage("\n==============================\n");
            printMessage("   Success setting 'Off' for no detecting gps antenna in BUB Mode. \n");
            printMessage("==============================\n");
            return true;
        }
        else
        {
            printMessage("\n==============================\n");
            printMessage("   Fail setting 'Off' for no detecting gps antenna in BUB Mode : \n");
            printMessage("     The test code is not processed due to %d\n", error);
            printMessage("==============================\n");
        }
    }

    return false;
}

bool handler_antTest(int32_t argc, char **argv)
{
    ant_status_t ant_status = {};
    int32_t set_ret = E_ERROR;
    int32_t get_ret = E_ERROR;
    int32_t prop = E_ERROR;

    printMessage("\n============================================================\n");
    printMessage("Start Test Read Antenna Status \n");

    set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_MAIN, &ant_status);
    printMessage("Main Antenna, adc value(%d) detectfaultcnt(%d) status(%d) \n",
            ant_status.adc_value, ant_status.detect_fault_cnt, ant_status.status);

    ant_status.adc_value = 0;
    ant_status.detect_fault_cnt = 0;
    ant_status.status = DTC_STATUS_INIT;
    set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_SUB, &ant_status);
    printMessage("Sub Antenna, adc value(%d) detectfaultcnt(%d) status(%d) \n",
            ant_status.adc_value, ant_status.detect_fault_cnt, ant_status.status);

    ant_status.adc_value = 0;
    ant_status.detect_fault_cnt = 0;
    ant_status.status = DTC_STATUS_INIT;
    set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_GNSS, &ant_status);
    printMessage("GNSS Antenna, adc value(%d) detectfaultcnt(%d) status(%d) \n",
            ant_status.adc_value, ant_status.detect_fault_cnt, ant_status.status);
    printMessage("End Test Read Antenna Status \n");

    printMessage("\n============================================================\n");
//    if (CONFIG_ANTENNA_USE == 1) {
        set_ret = E_ERROR;
        printMessage("Start Single Antenna TEST Switch to Main Antenna \n");
        int32_t ant_mode = SL_INTERFACE_GET(SystemManagerService)->getSystemMgrProperty(PROP_CURRENT_ANT_MODE);
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchCurrentAntenna(ANTENNA_MAIN);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getCurrentAntenna();
        if (ANTENNA_MAIN == get_ret) {
            printMessage("PASS.. Current Antenna Mode is Main \n");
        } else {
            printMessage("FAIL.. Current Antenna Mode is %d \n", get_ret);
        }

        printMessage("Start Single Antenna TEST Switch to Sub Antenna \n");
        set_ret = E_ERROR;
        get_ret = E_ERROR;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchCurrentAntenna(ANTENNA_SUB);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getCurrentAntenna();
        if (ANTENNA_SUB == get_ret) {
            printMessage("PASS.. Current Antenna Mode is Sub \n");
        } else {
            printMessage("FAIL.. Current Antenna Mode is %d \n", get_ret);
        }

        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchCurrentAntenna(ant_mode);
        printMessage("End Single Antenna TEST Switch Antenna Mode \n");
//    }
/*
    else if (CONFIG_ANTENNA_USE == 2) {
        printMessage("Start Dual Antenna TEST Switch to Main Antenna Internal Path \n");
        set_ret = E_ERROR;
        get_ret = E_ERROR;
        int32_t ant_path = SL_INTERFACE_GET(SystemManagerService)->getSystemMgrProperty(PROP_MAIN_ANT_PATH);
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_MAIN, INTERNAL_ANT);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_MAIN);
        if (INTERNAL_ANT == get_ret) {
            printMessage("PASS.. Current Main Antenna Path is INTERNAL_ANT \n");
        } else {
            printMessage("FAIL.. Current Main Antenna Path is %d \n", get_ret);
        }

        printMessage("Start Dual Antenna TEST Switch to Main Antenna External Path \n");
        set_ret = E_ERROR;
        get_ret = E_ERROR;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_MAIN, EXTERNAL_ANT);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_MAIN);
        if (EXTERNAL_ANT == get_ret) {
            printMessage("PASS.. Current Main Antenna Path is EXTERNAL_ANT \n");
        } else {
            printMessage("FAIL.. Current Main Antenna Path is %d \n", get_ret);
        }
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_MAIN, ant_path);

        printMessage("Start Dual Antenna TEST Switch to Sub Antenna Internal Path \n");
        set_ret = E_ERROR;
        get_ret = E_ERROR;
        ant_path = SL_INTERFACE_GET(SystemManagerService)->getSystemMgrProperty(PROP_MAIN_ANT_PATH);
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_SUB, INTERNAL_ANT);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_SUB);
        if (INTERNAL_ANT == get_ret) {
            printMessage("PASS.. Current Sub Antenna Path is INTERNAL_ANT \n");
        } else {
            printMessage("FAIL.. Current Sub Antenna Path is %d \n", get_ret);
        }

        printMessage("Start Dual Antenna TEST Switch to Sub Antenna External Path \n");
        set_ret = E_ERROR;
        get_ret = E_ERROR;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_SUB, EXTERNAL_ANT);
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaPath(ANTENNA_SUB);
        if (EXTERNAL_ANT == get_ret) {
            printMessage("PASS.. Current Sub Antenna Path is EXTERNAL_ANT \n");
        } else {
            printMessage("FAIL.. Current Sub Antenna Path is %d \n", get_ret);
        }
        set_ret = SL_INTERFACE_GET(SystemManagerService)->switchAntennaPath(ANTENNA_SUB, ant_path);
        printMessage("End Dual Antenna TEST Switch Antenna Path \n");

    }
    else {
            printMessage("Test Error!! Ant Use value (%d) Error!! \n", CONFIG_ANTENNA_USE);
    }
*/
    printMessage("\n============================================================\n");
    return true;
}

static time_t parse_SystemTime(char *s)
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
    printMessage("%s() day=%d, hour=%d \n", __func__, day, hour);
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

/* sldd sys setSystemTime 4 20160420.101038 */
bool handler_setSystemTime(int32_t argc, char **argv)
{
    int32_t set_ret = E_ERROR;
    int32_t get_ret = INIT_TIME;
    char time_buf[1024] = {};
    struct tm tm;
    time_t time;

    printMessage("==============================\n");
    if(argc == 0)
    {
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getLastTimeType();
        tm = SL_INTERFACE_GET(SystemManagerService)->getCurrentTime();
        strftime(time_buf, sizeof(time_buf), "%F %T" ,&tm);
        printMessage("System Type(%d), Time : ", get_ret);
        printMessage("%s \n", time_buf);
        printMessage("==============================\n");

        usage_setSystemTime();
        return true;
    }

    int32_t time_type = sl::atoi(argv[0]);
    struct timeval tv;

    if(time_type < MANUAL_TIME && GPS_TIME < time_type)
    {
        usage_setSystemTime();
        return true;
    }

    tv.tv_sec = parse_SystemTime(argv[1]);
    tv.tv_usec = 0;
    //#define tv2mS(t) ((t).tv_sec * 1000LL + ((t).tv_usec + 500) / 1000)
    //int64_t timeMs = ( (static_cast<int64_t>(tv.tv_sec) * 1000LL) + ((static_cast<int64_t>(tv.tv_usec) + 500LL) / 1000LL) );

    set_ret = SL_INTERFACE_GET(SystemManagerService)->setCurrentTimeMs(time_type, tv2mS(tv));

    if (set_ret == E_OK) {
        get_ret = SL_INTERFACE_GET(SystemManagerService)->getLastTimeType();
        tm = SL_INTERFACE_GET(SystemManagerService)->getCurrentTime();
        strftime(time_buf, sizeof(time_buf), "%F %T" ,&tm);
        printMessage("Setting Success!! System Type(%d), Time : ", get_ret);
        printMessage("%s \n", time_buf);
    } else {
        printMessage("Fail Setting System Time by cause(%d) \n", set_ret);
    }
    printMessage("==============================\n");

    return true;
}

/*
// sldd sys readGPIOvalue 8
bool handler_readGPIOvalue(int32_t argc, char **argv)
{
    if(argc < 1) {
        return false;
    }

    int32_t error;
    uint32_t gpio;

    gpio = sl::atoi(argv[0]);
    LOGV("%s() : gpio[%d]", __func__, gpio);

    error = SL_INTERFACE_GET(SystemManagerService)->readGPIOvalue(gpio);
    if(error != ERR)
    {
        printMessage("\n==============================\n");
        printMessage("   Success getting the value of gpio : \n");
        printMessage("      gpio[%d]=%d \n", gpio, error);
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("   Fail getting the value of gpio[%d] : \n", gpio);
        printMessage("     The test code is not processed due to %d\n", error);
        printMessage("==============================\n");
    }
    return false;
}

// sldd sys reqExtApGPIOvalue 8
bool handler_reqExtApGPIOvalue(int32_t argc, char **argv)
{
    int32_t error;
    uint32_t gpio;

    if(argc < 1) {
        return false;
    }

    gpio = sl::atoi(argv[0]);
    LOGV("%s() : gpio[%d]", __func__, gpio);

    error = SL_INTERFACE_GET(SystemManagerService)->reqExtApGPIOvalue(gpio);
    if(error == E_OK)
    {
        printMessage("\n==============================\n");
        printMessage("   Success requestting the gpio[%d] value of Ext.AP : \n", gpio);
        //printMessage("      gpio[%d]=%d \n", gpio, error);
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("   Fail requestting the gpio[%d] value of Ext.AP : \n", gpio);
        printMessage("     The test code is not processed due to %d\n", error);
        printMessage("==============================\n");
    }
    return false;
}

// sldd sys requestExtSIMStatus
bool handler_requestExtSIMStatus(int32_t argc, char **argv)
{
    int32_t error;

    LOGV("%s() : ", __func__);
    error = SL_INTERFACE_GET(SystemManagerService)->requestExtSIMStatus();
    if(error == E_OK)
    {
        printMessage("\n==============================\n");
        printMessage("   Success getting usb(Ext.SIM) status info : \n");
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("   Fail getting usb(Ext.SIM) status info : \n");
        printMessage("     The test code is not processed due to %d\n", error);
        printMessage("==============================\n");
    }
    return false;
}
*/

bool handler_getSystemProperty(int32_t argc, char **argv)
{
    LOGV("%s ", __func__);
    int32_t ret;
    int32_t property;

    if(argc < 1) {
        return false;
    }

    property = sl::atoi(argv[0]);
    ret = SL_INTERFACE_GET(SystemManagerService)->getSystemMgrProperty(property);

    if(ret != E_ERROR)
    {
        printMessage("\n==============================\n");
        printMessage("   Success getting the value of property : \n");
        printMessage("      property[%d]=%d \n", property, ret);
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("\n==============================\n");
        printMessage("   Fail getting the value of property[%d] : \n", property);
        printMessage("     The test code is not processed due to %d\n", ret);
        printMessage("==============================\n");
    }
    return false;
}

/*
// sldd sys dump PID
bool handler_dumpCallStack(int32_t argc, char **argv)
{
    uint32_t pid;
    char cmd_buffer[64];

    if(argc < 1) {
        printMessage(" Please input PID \n");
        return false;
    }

    pid = sl::atoi(argv[0]);
    LOGV("%s() : pid[%d]", __func__, pid);

    memset(cmd_buffer, 0x0, sizeof(cmd_buffer));
    snprintf(cmd_buffer,sizeof(cmd_buffer), "kill -12 %d", pid);
    system(cmd_buffer);

    printMessage("\n==============================\n");
    printMessage("...Wait for about 10 sec...\n");
    sleep(10); // 10sec
    printMessage("   Success dumpping the call stack of all threads for PID[%d] \n", pid);
    printMessage("==============================\n");
    return true;
}
*/

/* sldd sys qmi_test on/off */
bool handler_QMI_TEST(int32_t argc, char **argv)
{
    printMessage("not yet implemented!!");
    return false;
}

void register_system()
{
    registerCommands(MODULE_SYSTEM_MANAGER, NULL, sm_cmds);
}
