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


#include <hardware/gps.h>
#include <ConfigurationManagerService/IConfigurationManagerService.h>
#include <ConfigurationManagerService/IConfigurationManagerServiceType.h>
#include <LocationManagerService/ILocationManagerService.h>
#include <LocationManagerService/ILocationManagerServiceType.h>
#include <LocationManagerService/Location.h>
#include "services/ConfigurationManagerService/IConfigurationManagerServiceType.h"

#include "man.h"
#include "sldd_common.h"
#include "LocationManager.h"

#define LOG_TAG "sldd_loc"

using namespace android;

enum lm_commands {
    LM_CMD_GPS_START =1,
    LM_CMD_GPS_STOP,
    LM_CMD_DELETE_AIDING_DATA,
    LM_CMD_POSITIONING_METHOD,
    LM_CMD_SET_AGPS_SERVER,
    LM_CMD_SET_APN_INFO,
    LM_CMD_SET_AGPS_BEARER_TYPE,
    LM_CMD_GET_GPS_FIX_STATE,
    LM_CMD_GET_LAST_LOCATION_DATA,
    LM_CMD_SET_GPS_TIME_SET_FLAG,
    LM_CMD_SET_GPS_START,
    LM_CMD_ENABLE,
    LM_CMD_DISABLE,
    LM_CMD_GET_DELTA,
    LM_CMD_SET_CCF_NAVIGATION_SYSTEM,
    LM_CMD_SET_CCF_NAVIGATION_CONNECT_AND_VIEW,
    LM_CMD_GET_GNSS_JAMMING,
    LM_CMD_SET_NMEA_LOG,
    LM_CMD_NMEA_DISPLAY,
    LM_CMD_GET_PREDICT_LOCATION_DATA,
    LM_CMD_RESET_LOCATION,
    LM_CMD_GET_LOCATION_DATA,
    LM_CMD_SET_NMEA_SERIAL_PORT_ON_OFF,
    LM_CMD_GET_GPS_STATUS,
    LM_CMD_GNSS_CMD,
    LM_CMD_SET_LABTEST_MODE,
    LM_CMD_SET_LABTEST_AUTO_START,
    LM_CMD_SET_LOG_ENABLE,
    LM_CMD_SET_XTRA_ENABLE,
    LM_CMD_NTP_DOWNLOAD_ENABLE,
    LM_CMD_XTRA_DOWNLOAD_ENABLE,
    LM_CMD_SVT_REQUIRE_ENABLE,
    LM_CMD_SVT_REQUIRE_ENABLE2,
    LM_CMD_GET_XTRA_STATUS,
    LM_CMD_GET_DISTANCE,
};

static bool handler_gps_start(int32_t argc, char **argv);
static bool handler_gps_stop(int32_t argc, char **argv);
static bool handler_delete_aiding_data(int32_t argc, char **argv);
static bool handler_positioning_method(int32_t argc, char **argv);
static bool handler_set_AGPS_Server(int32_t argc, char **argv);
static bool handler_set_APN_Info(int32_t argc, char **argv);
static bool handler_set_AGPS_BearerType(int32_t argc, char **argv);
static bool handler_get_GPS_Fix_State(int32_t argc, char **argv);
static bool handler_get_Last_LocationData(int32_t argc, char **argv);
static bool handler_set_GPS_Time_Set_Flag(int32_t argc, char **argv);
static bool handler_set_GPS_Start(int32_t argc, char **argv);
static bool handler_GPS_Start_enable(int32_t argc, char **argv);
static bool handler_GPS_Start_disable(int32_t argc, char **argv);
static bool handler_get_Delta(int32_t argc, char **argv);
static bool handler_Set_CCFNavigationSystem(int32_t argc, char **argv);
static bool handler_Set_CCFConnectAndView(int32_t argc, char **argv);
static bool handler_get_GNSSJammingStatus(int32_t argc, char **argv);
static bool handler_set_NMEALog(int32_t argc, char **argv);
static bool handler_NMEALogDisplay(int32_t argc, char **argv);
static bool handler_get_PredictLocData(int32_t argc, char **argv);
static bool handler_ResetLocation(int32_t argc, char **argv);
static bool handler_get_Location_Data(int32_t argc, char **argv);
static bool handler_set_NMEASerialPortOnOff(int32_t argc, char **argv);
static bool handler_get_GPSStatus(int32_t argc, char **argv);
static bool handler_LgeGNSSCmd(int32_t argc, char **argv);
static bool handler_set_LabTestMode(int32_t argc, char **argv);
static bool handler_set_LabTestAutoStart(int32_t argc, char **argv);
static bool handler_set_LogEnable(int32_t argc, char **argv);
static bool handler_set_XtraEnable(int32_t argc, char **argv);
static bool handler_set_NTP_Download(int32_t argc, char **argv);
static bool handler_set_Download(int32_t argc, char **argv);
static bool handler_set_requestSVT(int32_t argc, char **argv);
static bool handler_set_requestSVT2(int32_t argc, char **argv);
static bool handler_get_XtraStatus(int32_t argc, char **argv);
static bool handler_get_distance(int32_t argc, char **argv);

sldd_cmd_table_t lm_cmds[] = {
    {"start", LM_CMD_GPS_START, handler_gps_start,
     "sldd loc start - gps start\n"},
    {"stop", LM_CMD_GPS_STOP, handler_gps_stop,
     "sldd loc stop - gps stop\n"},
    {"delete", LM_CMD_DELETE_AIDING_DATA, handler_delete_aiding_data,
     "sldd loc delete  [<command>]               - delete_aiding_data\n"
     "      all       - GPS_DELETE_ALL\n"
     "      ephemeris - GPS_DELETE_EPHEMERIS | GLO_DELETE_EPHEMERIS\n"},
    {"pos", LM_CMD_POSITIONING_METHOD, handler_positioning_method,
     "sldd loc pos  [<command>]                  - SetPositionMode\n"
     "      msa       - GPS_POSITION_MODE_MS_ASSISTED\n"
     "      msb       - GPS_POSITION_MODE_MS_BASED\n"
     "      sa        - GPS_POSITION_MODE_STANDALONE\n"},
    {"setAgpsServer", LM_CMD_SET_AGPS_SERVER, handler_set_AGPS_Server,
     "sldd loc setAgpsServer  [ host ][ port ]   - AGPS set server\n"},
    {"setApnInfo", LM_CMD_SET_APN_INFO, handler_set_APN_Info,
     "sldd loc setApnInfo  [ apn ][ agpsBearerType ][ suplVer ][ suplHost ][ suplPort ]   - set APN info\n"},
    {"setAgpsBearerType", LM_CMD_SET_AGPS_BEARER_TYPE, handler_set_AGPS_BearerType,
     "sldd loc setAgpsBearerType  [ agpsBearerType ]   - set AGPS bearer type\n"},
    {"getGPSFixState", LM_CMD_GET_GPS_FIX_STATE, handler_get_GPS_Fix_State,
     "sldd loc getGPSFixState - get GPS Fix state\n"},
    {"getLastLocData", LM_CMD_GET_LAST_LOCATION_DATA, handler_get_Last_LocationData,
     "sldd loc getLastLocData - get Last Location Data\n"},
    {"setGPSTimeSetFlag", LM_CMD_SET_GPS_TIME_SET_FLAG, handler_set_GPS_Time_Set_Flag,
     "sldd loc setGPSTimeSetFlag - set GPS Time set flag\n"},
    {"setGPSStart", LM_CMD_SET_GPS_START, handler_set_GPS_Start,
     "sldd loc setGPSStart - \n"},
    {"enable", LM_CMD_ENABLE, handler_GPS_Start_enable,
     "sldd loc enable - gps start enable\n"},
    {"disable", LM_CMD_DISABLE, handler_GPS_Start_disable,
     "sldd loc disable - gps start disable\n"},
    {"getDelta", LM_CMD_GET_DELTA, handler_get_Delta,
     "sldd loc getDelta - Recent Vehicle Location delta \n"},
    {"setCCFNavigationSystem", LM_CMD_SET_CCF_NAVIGATION_SYSTEM, handler_Set_CCFNavigationSystem,
     "sldd loc setCCFNavigationSystem\n"},
    {"setCCFConnectAndView", LM_CMD_SET_CCF_NAVIGATION_CONNECT_AND_VIEW, handler_Set_CCFConnectAndView,
     "sldd loc setCCFConnectAndView \n"},
    {"getJamming", LM_CMD_GET_GNSS_JAMMING, handler_get_GNSSJammingStatus,
     "sldd loc getJamming - get GNSS jamming status \n"},
    {"setNMEALog", LM_CMD_SET_NMEA_LOG, handler_set_NMEALog,
     "sldd loc setNMEALog - set NMEA Log enable/disable \n"},
    {"NMEADisplay", LM_CMD_NMEA_DISPLAY, handler_NMEALogDisplay,
     "sldd loc NMEADisplay - NMEA display \n"},
    {"getPredictData", LM_CMD_GET_PREDICT_LOCATION_DATA, handler_get_PredictLocData,
     "sldd loc getPredictData - Get Predict Location Data \n"},
    {"ResetLocation", LM_CMD_RESET_LOCATION, handler_ResetLocation,
     "sldd loc ResetLocation - Reset Location Info \n"},
    {"getLocationData", LM_CMD_GET_LOCATION_DATA, handler_get_Location_Data,
     "sldd loc getLocationData - Get Location Data \n"},
    {"nmea_serial_port", LM_CMD_SET_NMEA_SERIAL_PORT_ON_OFF, handler_set_NMEASerialPortOnOff,
     "sldd loc nmea_serial_port - \n"},
    {"getgpsstatus", LM_CMD_GET_GPS_STATUS, handler_get_GPSStatus,
     "sldd loc getgpsstatus - \n"},
    {"lgegnsscmd", LM_CMD_GNSS_CMD, handler_LgeGNSSCmd,
     "sldd loc lgegnsscmd [command type] [value] \n"},
    {"labTestMode", LM_CMD_SET_LABTEST_MODE, handler_set_LabTestMode,
     "sldd loc labTestMode [<command>] \n"
     "      on \n"
     "      off \n"},
    {"labTestAutoStart", LM_CMD_SET_LABTEST_AUTO_START, handler_set_LabTestAutoStart,
     "sldd loc labTestAutoStart [<command>] \n"
     "      on \n"
     "      off \n"},
     {"SetLogEnable", LM_CMD_SET_LOG_ENABLE, handler_set_LogEnable,
     "sldd loc SetLogEnable [<command>] \n"
     "      on \n"
     "      off \n"},
     {"SetXtraEnable", LM_CMD_SET_XTRA_ENABLE, handler_set_XtraEnable,
     "sldd loc SetXtraEnable [<command>] \n"
     "      on \n"
     "      off \n"},
     {"setNTPDownload", LM_CMD_NTP_DOWNLOAD_ENABLE, handler_set_NTP_Download,
     "sldd loc setNTPDownload - NTP Download\n"},
     {"setXdownload", LM_CMD_XTRA_DOWNLOAD_ENABLE, handler_set_Download,
     "sldd loc setXdownload - Xtra Download\n"},
     {"requestSVT", LM_CMD_SVT_REQUIRE_ENABLE, handler_set_requestSVT,
     "sldd loc requestSVT - requireGNSSDataForSVT \n"},
     {"requestSVT2", LM_CMD_SVT_REQUIRE_ENABLE, handler_set_requestSVT2,
     "sldd loc requestSVT2 [<command>] \n"
     "      0 \n"
     "      1 \n"
     "      2 \n"},
     {"GetXtraStatus", LM_CMD_GET_XTRA_STATUS, handler_get_XtraStatus,
     "sldd loc GetXtraStatus - \n"},
     {"getDistance", LM_CMD_GET_DISTANCE, handler_get_distance,
     "sldd loc getDistance - \n"},
    {NULL, NULL, NULL},
};

static sp<ILocationManagerService> lm = NULL;
sp<ILocationManagerService> getLOCInterface()
{
    if( lm == NULL)
    {
        lm = interface_cast<ILocationManagerService>
                (defaultServiceManager()->getService(String16(LOCATION_SRV_NAME)));
    }
    return lm;
}

bool commandActionLocation(int32_t argc, char **argv) {

    if (argc == 0) {
        usage_location(NULL);
        return true;
    }

    DO_HANDLER(lm_cmds, argv[0], argc-1, argv+1);
}

char* usage_location(char *cmd) {
    printMessage(" sldd lm [<command>]         - perform LocationManager's functions \n");
    printMessage("    <command> \n");
    PRINT_USAGE(lm_cmds);
    return NULL;
}

static bool handler_gps_start(int32_t argc, char **argv)
{
    getLOCInterface()->setLocationStatus(1);

    return true;
}

static bool handler_gps_stop(int32_t argc, char **argv)
{
    getLOCInterface()->setLocationStatus(0);

    return true;
}

static bool handler_delete_aiding_data(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    uint64_t mode;
    if(!strcmp(argv[0],"all"))
    {
        mode = GPS_DELETE_ALL;
    }
    else if(!strcmp(argv[0],"ephemeris"))
    {
        mode = GPS_DELETE_EPHEMERIS | GLO_DELETE_EPHEMERIS;
    }
    else
    {
        ///abnormal parameter
        return false;
    }

    getLOCInterface()->deleteAidingData(mode);

    return true;
}

static bool handler_positioning_method(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    uint32_t mode;

    if(!strcmp(argv[0],"msa"))
    {
        mode = GPS_POSITION_MODE_MS_ASSISTED;
    }
    else if(!strcmp(argv[0],"msb"))
    {
        mode = GPS_POSITION_MODE_MS_BASED;
    }
    else if(!strcmp(argv[0],"sa"))
    {
        mode = GPS_POSITION_MODE_STANDALONE;
    }
    else
    {
        ///abnormal parameter
        return false;
    }
    getLOCInterface()->SetPositionMode(mode);

    return true;
}

static bool handler_set_AGPS_Server(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;

    int32_t port = strtol(argv[1], NULL, 10);

    getLOCInterface()->setAgpsServer(argv[0], port);

    return true;
}

static bool handler_set_APN_Info(int32_t argc, char **argv)
{
    if(argc < 5)
        return false;

    int16_t agpsBearerType = strtol(argv[1], NULL, 10);
    unsigned long suplVer  = strtol(argv[2], NULL, 16);
    int32_t suplPort       = strtol(argv[4], NULL, 10);

    getLOCInterface()->setAPNInfo(argv[0], agpsBearerType, suplVer, argv[3], suplPort);

    return true;
}

static bool handler_set_AGPS_BearerType(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    uint8_t agpsBearerType = strtol(argv[0], NULL, 10);

    getLOCInterface()->setAgpsBearerType(agpsBearerType);

    return true;
}

static bool handler_get_GPS_Fix_State(int32_t argc, char **argv)
{
    uint8_t state = getLOCInterface()->getGpsPosReportStatus();
    if(state == GPS_POS_REPORT_STATUS_NONE)
    {
        printMessage("==============================\n");
        printMessage("GPS not fixed.\n");
        printMessage("==============================\n");
    }
    else if(state == GPS_POS_REPORT_STATUS_CONNECTED)
    {
        printMessage("==============================\n");
        printMessage("GPS fixed\n");
        printMessage("==============================\n");
    }
    else if(state == GPS_POS_REPORT_STATUS_DISCONNECTED)
    {
        printMessage("==============================\n");
        printMessage("GPS fix disconnected.\n");
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("ERROR!!!\n");
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_get_Last_LocationData(int32_t argc, char **argv)
{
    LocationData_t last_loc = {0,};
    getLOCInterface()->getLastLocationData(&last_loc, sizeof(LocationData_t));

    printMessage("   latitude       : %f \n", last_loc.latitude);
    printMessage("   longitude      : %f \n", last_loc.longitude);
    printMessage("   altitude       : %f \n", last_loc.altitude);
    printMessage("   speed          : %f \n", last_loc.speed);
    printMessage("   bearing        : %f \n", last_loc.bearing);
    printMessage("   accuracy       : %f \n", last_loc.accuracy);
    printMessage("   fixType        : %d \n", last_loc.fixType);
    printMessage("   noOfSatellites : %d \n", last_loc.noOfSatellites);
    printMessage("   timestamp : %llu \n", last_loc.timestamp);

    return true;
}

static bool handler_set_GPS_Time_Set_Flag(int32_t argc, char **argv)
{
    getLOCInterface()->setGPSTimeSetFlag(true);
    return true;
}

static bool handler_set_GPS_Start(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    uint8_t mode;
    if(!strcmp(argv[0],"cold"))
    {
        mode = 0;
    }
    else if(!strcmp(argv[0],"warm"))
    {
        mode = 1;
    }
    else if(!strcmp(argv[0],"hot"))
    {
        mode = 2;
    }
    else
    {
        ///abnormal parameter
        return false;
    }

    getLOCInterface()->setGpsStartMode(mode);

    return true;
}

static bool handler_GPS_Start_enable(int32_t argc, char **argv)
{
    getLOCInterface()->enableGPSStart();

    return true;
}

static bool handler_GPS_Start_disable(int32_t argc, char **argv)
{
    getLOCInterface()->disableGPSStart();

    return true;
}

static bool handler_get_Delta(int32_t argc, char **argv)
{
    sp<LocationDeltaData> location_delta = new LocationDeltaData();

    getLOCInterface()->getRecentLocationDelta(location_delta);

    printMessage("  N1 latitude delta   : %d \n", location_delta->N1_latitude_delta);
    printMessage("  N1 longitude delta : %d \n\n", location_delta->N1_longitude_delta);
    printMessage("  N2 latitude delta   : %d \n", location_delta->N2_latitude_delta);
    printMessage("  N2 longitude delta : %d \n", location_delta->N2_longitude_delta);

    return true;
}

bool handler_Set_CCFNavigationSystem(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    error_t error = E_ERROR;
    sp<ConfigurationBuffer> buf = new ConfigurationBuffer();
    uint8_t NavigationSystem = strtol(argv[0], NULL, 10);

    sp<IConfigurationManagerService> mConfiguration = interface_cast<IConfigurationManagerService>(defaultServiceManager()->getService(String16(CONFIGURATION_SRV_NAME)));

    buf->setTo(NavigationSystem);
    error =  mConfiguration->change_set(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL, "navigationSystem", buf);

    if(error == E_OK)
    {
        printMessage("==============================\n");
        printMessage("   Success CCFNavigationSystem Set %d \n", NavigationSystem);
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("==============================\n");
        printMessage("   Fail CCFNavigationSystem Set \n");
        printMessage("==============================\n");
        return false;
    }
}

bool handler_Set_CCFConnectAndView(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    error_t error = E_ERROR;
    sp<ConfigurationBuffer> buf = new ConfigurationBuffer();
    uint8_t ConnectView = strtol(argv[0], NULL, 10);

    sp<IConfigurationManagerService> mConfiguration = interface_cast<IConfigurationManagerService>(defaultServiceManager()->getService(String16(CONFIGURATION_SRV_NAME)));

    buf->setTo(ConnectView);
    error =  mConfiguration->change_set(configmgr::OEM_USR_CONFIG_FILE, NAME_VAL, "connectView", buf);

    if(error == E_OK)
    {
        printMessage("==============================\n");
        printMessage("   Success CCFConnectAndView Set %d \n", ConnectView);
        printMessage("==============================\n");
        return true;
    }
    else
    {
        printMessage("==============================\n");
        printMessage("   Fail CCFConnectAndView Set \n");
        printMessage("==============================\n");
        return false;
    }
}

static bool handler_get_GNSSJammingStatus(int32_t argc, char **argv)
{
    bool jamming = getLOCInterface()->getGNSSJammingStatus();

    if(jamming == true)
    {
        printMessage("==============================\n");
        printMessage("Jamming status : TRUE\n");
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("Jamming status : FALSE\n");
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_set_NMEALog(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    uint8_t log_state = strtol(argv[0], NULL, 10);

    getLOCInterface()->setNMEALog(log_state);

    if(log_state == 0)
    {
        printMessage("==============================\n");
        printMessage("NMEA LOG Disable \n");
        printMessage("==============================\n");
    }
    else if(log_state == 1)
    {
        printMessage("==============================\n");
        printMessage("NMEA LOG Enable \n");
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("ERROR !!! \n");
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_NMEALogDisplay(int32_t argc, char **argv)
{
    system("logcat -v raw LocationCb:I *:S");

    return true;
}

static bool handler_get_PredictLocData(int32_t argc, char **argv)
{
    sp<PredictLocationData> predictLocData = new PredictLocationData();

    getLOCInterface()->getPredictedLocationData(predictLocData);

    printMessage(" predict latitude : %.14f \n", predictLocData->predictLocData.latitude);
    printMessage(" predict longitude : %.14f \n", predictLocData->predictLocData.longitude);

    return true;
}

static bool handler_ResetLocation(int32_t argc, char **argv)
{
    getLOCInterface()->resetLocationInfo();

    return true;
}

static bool handler_get_Location_Data(int32_t argc, char **argv)
{
    double Invalid_Value_latitude = 90.000000;
    double Invalid_Value_longitude = 180.000000;
    sp<LocationData> location_data = new LocationData();
    getLOCInterface()->getLocationData(location_data);

    if((location_data->locationData.latitude == 0) || (location_data->locationData.latitude > Invalid_Value_latitude))
        printMessage("   latitude       : 0x7FFFFFFF \n", location_data->locationData.latitude);
    else
        printMessage("   latitude       : %.14f \n", location_data->locationData.latitude);

    if((location_data->locationData.longitude == 0) || (location_data->locationData.longitude > Invalid_Value_longitude))
        printMessage("   longitude      : 0x7FFFFFFF \n", location_data->locationData.longitude);
    else
        printMessage("   longitude      : %.14f \n", location_data->locationData.longitude);

    return true;
}

static bool handler_set_NMEASerialPortOnOff(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"on"))
    {
        getLOCInterface()->setNMEASerialPortOnOff(0);

        system("usb_composition empty y y y");
        system("usb_composition 9025 n y n");
        system("reboot");
    }
    else if(!strcmp(argv[0],"off"))
    {
        getLOCInterface()->setNMEASerialPortOnOff(1);

        system("usb_composition 633A_lge n y n");
        system("usb_composition 9099 y y n");
        system("reboot");
    }
    else
    {
        ///abnormal parameter
        return false;
    }

    return true;
}

static bool handler_get_GPSStatus(int32_t argc, char **argv)
{
    if (getLOCInterface()->IsLocationStarted() == true)
    {
        printMessage("==============================\n");
        printMessage("GPS On \n");
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("GPS Off \n");
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_LgeGNSSCmd(int32_t argc, char **argv)
{
    if(argc < 2)
        return false;

    uint32_t cmdType = strtol(argv[0], NULL, 10);
    uint32_t value = strtol(argv[1], NULL, 10);
    uint32_t resp_value;
    error_t return_value;

    return_value = getLOCInterface()->LocationLgeGnssCmd(cmdType, value, &resp_value);

    {
        printMessage("==============================\n");
        printMessage("resp_value : %d \n", resp_value);
        printMessage("return_value : %d \n", return_value);
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_set_LabTestMode(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"on"))
    {
        getLOCInterface()->setLabTestMode(1);
    }
    else if(!strcmp(argv[0],"off"))
    {
        getLOCInterface()->setLabTestMode(0);
    }
    else
    {
        return false;
    }

    return true;
}

static bool handler_set_LabTestAutoStart(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"on"))
    {
        getLOCInterface()->setLabTestAutoStart(1);
    }
    else if(!strcmp(argv[0],"off"))
    {
        getLOCInterface()->setLabTestAutoStart(0);
    }
    else
    {
        return false;
    }

    return true;
}

static bool handler_set_LogEnable(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"on"))
    {
        printMessage("==============================\n");
        printMessage("LogEnable On \n");
        printMessage("==============================\n");
        getLOCInterface()->setLogEnable(1);
    }
    else if(!strcmp(argv[0],"off"))
    {
        printMessage("==============================\n");
        printMessage("LogEnable OFF \n");
        printMessage("==============================\n");
        getLOCInterface()->setLogEnable(0);
    }
    else
    {
        return false;
    }
    return true;
}

static bool handler_set_XtraEnable(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"on"))
    {
        printMessage("==============================\n");
        printMessage("XtraEnable On \n");
        printMessage("==============================\n");
        getLOCInterface()->setXtraEnable(1);
    }
    else if(!strcmp(argv[0],"off"))
    {
        printMessage("==============================\n");
        printMessage("XtraEnable OFF \n");
        printMessage("==============================\n");
        getLOCInterface()->setXtraEnable(0);
    }
    else
    {
        return false;
    }
    return true;
}
static bool handler_set_NTP_Download(int32_t argc, char **argv)
{
    getLOCInterface()->setNTPDownload();
    printMessage("NTP Download\n");
    return true;
}
static bool handler_set_Download(int32_t argc, char **argv)
{
    getLOCInterface()->setXdownload();
    printMessage("Xtra Download\n");
    return true;
}
static bool handler_set_requestSVT(int32_t argc, char **argv)
{
    getLOCInterface()->requestSVT();
    printMessage("requireGNSSDataForSVT\n");
    return true;
}
static bool handler_set_requestSVT2(int32_t argc, char **argv)
{
    if(argc < 1)
        return false;

    if(!strcmp(argv[0],"0"))
    {
        printMessage("==============================\n");
        printMessage("requestSVT High Quality Fix obtained \n");
        printMessage("==============================\n");
        getLOCInterface()->requestSVT2(0);
    }
    else if(!strcmp(argv[0],"1"))
    {
        printMessage("==============================\n");
        printMessage("requestSVT Low Quality Fix obtained \n");
        printMessage("==============================\n");
        getLOCInterface()->requestSVT2(1);
    }
    else
    {
        printMessage("==============================\n");
        printMessage("GNSSFixTimerLongS YES \n");
        printMessage("==============================\n");
        getLOCInterface()->requestSVT2(2);
    }
    return true;
}
static bool handler_get_XtraStatus(int32_t argc, char **argv)
{
    if (getLOCInterface()->IsXtraStatus() == true)
    {
        printMessage("==============================\n");
        printMessage("XtraStatus On \n");
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("XtraStatus Off \n");
        printMessage("==============================\n");
    }

    return true;
}

static bool handler_get_distance(int32_t argc, char **argv)
{
    if (argc < 4) {
       printMessage("Need more arg.\n");
       return false;
    }
   double lat1  = sl::atof(argv[0]);
   double long1 = sl::atof(argv[1]);
   double lat2  = sl::atof(argv[2]);
   double long2 = sl::atof(argv[3]);
   printMessage("-------------------------------------------------------\n");
   printMessage("    Calculate Distance : %lf %lf %lf %lf \n", lat1, long1, lat2, long2);
   printMessage("-------------------------------------------------------\n");

   float array[3];
   Location::distanceBetween(lat1, long1, lat2, long2, array);


   printMessage(" -> Result[0] : %lf\n", array[0]);
   printMessage(" -> Result[1] : %lf\n", array[1]);
   printMessage(" -> Result[2] : %lf\n", array[2]);

   return true;
}

void register_location()
{
    registerCommands(MODULE_LOCATION_MANAGER, NULL, lm_cmds);
}
