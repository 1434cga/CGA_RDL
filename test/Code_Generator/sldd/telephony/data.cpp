/**
 * @copyright
 * Copyright (c) 2015 by LG Electronics Inc
 * This program or software including the accompanying associated documentation
 * ("Software") is the proprietary software of LG Electronics Inc. and or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate written license agreement between you
 * and LG Electronics Inc. ("Authorized License"). Except as set forth in an
 * Authorized License, LG Electronics Inc. grants no license (express or implied),
 * rights to use, or waiver of any kind with respect to the Software, and LG
 * Electronics Inc. expressly reserves all rights in and to the Software and all
 * intellectual property therein. If you have no Authorized License, then you have
 * no rights to use the Software in any ways, and should immediately notify LG
 * Electronics Inc. and discontinue all use of the Software.
 *
 * @file
 * Data.cpp
 * @brief
 * Providing SLDD support for Data in TelephonyManagerService.
 */

#include "man.h"
#include "sldd_common.h"
#include "data.h"
#include "telephony/telephony_common.h"

#define LOG_TAG "sldd_data"

using namespace PhoneConstants;

enum Commands {
    CMD_DATA_CONNECTION_SATAE               /**Get Data Connection State*/=1,
    CMD_DATA_ACTIVITY_STATE                 /**Get Data Activity State*/,
    CMD_GET_IP_ADDRESS                      /**Get Board IP Address*/,
    CMD_GET_DNS                             /**Get DNS*/,
    CMD_GET_APN                             /**Get APN(radiotech, profile, apn, user, password, authtype, protocol)*/,
    CMD_SET_APN                             /**[parameters] - Set APN*/,
    CMD_GET_DATA_USAGE                      /**Get Data Usage*/,
    CMD_GET_DATA_USAGE_LIST                 /**[parameters] - Get Data Usage Print*/,
    CMD_GET_APN_NAME                        /**Get APN Name*/,
    CMD_GET_APN_USER_ID                     /**Get APN UserID*/,
    CMD_GET_APN_PASSWORD                    /**Get APN Password*/,
    CMD_GET_PACKET_LOSS                     /**[parameters] - Get Packet Loss*/,
    CMD_GET_ERROR_BLOCKS                    /**[parameters]   - Get Error Blocks*/,
    CMD_SET_USER_DATA_ENABLE                /**Data Connection User Enabled Setting*/,
    CMD_SET_INTERNAL_DATA_ENABLE            /**Internal Data Enabled Setting*/,
    CMD_GET_DATA_ROAMING_ENABLE             /**Get Data Roaming Enabled*/,
    CMD_SET_DATA_ROAMING_ENABLE             /**Data Roaming Enabled Setting*/,
    CMD_GET_DID_APN_SETTING_ENABLE          /**Get DID APN Setting Enabled*/,
    CMD_SET_DID_APN_SETTING_ENABLE          /**Set DID APN Setting Enabled(Only TSIM)*/,
    CMD_GET_AUTO_SELECT_ENABLE              /**Get Auto Select Enabled(Default is TRUE)*/,
    CMD_SET_AUTO_SELECT_ENABLE              /**Set Auto Select Enabled(PSIM Connection)*/,
    CMD_SET_MANUAL_APN_SETTING              /**[parameters] - Manaul APN Setting*/,
    CMD_MANUAL_DATA_CONNECT                 /**[parameters] - Manaul APN Setting and Data Connect*/,
    CMD_GET_ACTIVE_APN_STRING               /**Get Active APN String(apnType)*/,
    CMD_GET_PSIM_DATA_USAGE                 /**Get PSIM DataUsage*/,
    CMD_SET_PSIM_DATA_USAGE_RESET           /**Set PSIM DataUsage Reset*/,
    CMD_SET_DATA_LIMIT_SETTING              /**Set PSIM Data Limit Setting*/,
    CMD_GET_DATA_LIMIT_ALERT                /**Get PSIM Data Limit Alert*/,
    CMD_GET_DATA_LIMIT                      /**Get PSIM Data Limit*/,
    CMD_GET_DATA_LIMIT_BILL_DATE            /**Get PSIM Data Limit Bill Date*/,
    //TIGER-252-Start
    CMD_GET_DATA_GET_APN_LIST               /**Get Apn List*/,
    CMD_GET_DATA_REMOVE_APN                 /**Remove APN*/,
    CMD_GET_DATA_ADD_APN                    /**Add APN*/,
    CMD_GET_DATA_UPDATE_APN                 /**update APN By Index*/,
    CMD_GET_DATA_RESET_APN_TO_DEFAULT       /**Reset APN to Default*/,
    CMD_GET_DATA_SET_PREFERRED_APN          /**Set preferred APN*/,
    CMD_GET_DATA_GET_PREFERRED_APN          /**Get preferred apn*/,
    //TIGER-252-End

    //MPDN_Start
    CMD_SETUP_DATA                          /**Brings up the data for a particular ApnType, if there is no existing such connection.*/,
    CMD_DISCONNECT_DATA                     /**Brings down the data for a particular ApnType, if there is no more usages of this connection.*/,
    //MPDN_End

    //--------------------------------
    // hidden commands
    CMD_LISTEN,
};

sldd_cmd_table_t data_cmds[] = {
    {"getstate", CMD_DATA_CONNECTION_SATAE, commandActionData, "getstate - Get Data Connection State \n"},
    {"getactivity", CMD_DATA_ACTIVITY_STATE, commandActionData, "getactivity - Get Data Activity State \n"},
    {"getip", CMD_GET_IP_ADDRESS, commandActionData, "getip - Get Board IP Address \n"},
    {"getdns", CMD_GET_DNS, commandActionData, "getip - Get DNS \n"},
    {"getapn", CMD_GET_APN, commandActionData, "getapn - Get APN(radiotech, profile, apn, user, password, authtype, protocol) \n"},
    {"getname", CMD_GET_APN_NAME, commandActionData, "getname - Get APN Name \n"},
    {"getuser", CMD_GET_APN_USER_ID, commandActionData, "getuser - Get APN UserID \n"},
    {"getpassword", CMD_GET_APN_PASSWORD, commandActionData, "getpassword- Get APN Password \n"},
    {"getusage", CMD_GET_DATA_USAGE, commandActionData, "getusage [parameters] - Get Data Usage \n"},
    {"getusagelist", CMD_GET_DATA_USAGE_LIST, commandActionData, "getusagelist [parameters] - Get Data Usage Print \n"},
    {"getpacketloss", CMD_GET_PACKET_LOSS, commandActionData, "getpacketloss [parameters] - Get Packet Loss \n"},
    {"geterrorblocks", CMD_GET_ERROR_BLOCKS, commandActionData, "geterrorblocks [parameters]   - Get Error Blocks \n"},
    {"setapn", CMD_SET_APN, commandActionData, "setapn [parameters] - Set APN \n"},
    {"setManualApnSetting", CMD_SET_MANUAL_APN_SETTING, commandActionData, "setManualApnSetting [parameters] - Manaul APN Setting\n"},
    {"setManualConnect", CMD_MANUAL_DATA_CONNECT, commandActionData, "setManualConnect [parameters] - Manaul APN Setting and Data Connect \n"},
    {"setUserDataEnabled", CMD_SET_USER_DATA_ENABLE, commandActionData, "setUserDataEnabled - Data Connection User Enabled Setting \n"},
    {"setInternalDataEnable", CMD_SET_INTERNAL_DATA_ENABLE, commandActionData, "setInternalDataEnable - Internal Data Enabled Setting \n"},
    {"getDataRoamingEnabled", CMD_GET_DATA_ROAMING_ENABLE, commandActionData, "getDataRoamingEnabled - Get Data Roaming Enabled \n"},
    {"setDataRoamingEnabled", CMD_SET_DATA_ROAMING_ENABLE, commandActionData, "setDataRoamingEnabled - Data Roaming Enabled Setting \n"},
    {"getDidApnSettingEnabled", CMD_GET_DID_APN_SETTING_ENABLE, commandActionData, "getDidApnSettingEnabled - Get DID APN Setting Enabled \n"},
    {"setDidApnSettingEnabled", CMD_SET_DID_APN_SETTING_ENABLE, commandActionData, "setDidApnSettingEnabled - Set DID APN Setting Enabled(Only TSIM) \n"},
    {"getAutoSelectEnabled", CMD_GET_AUTO_SELECT_ENABLE, commandActionData, "getAutoSelectEnabled - Get Auto Select Enabled(Default is TRUE) \n"},
    {"setAutoSelectEnabled", CMD_SET_AUTO_SELECT_ENABLE, commandActionData, "setAutoSelectEnabled - Set Auto Select Enabled(PSIM Connection) \n"},
    {"getActiveApnString", CMD_GET_ACTIVE_APN_STRING, commandActionData, "getActiveApnString - Get Active APN String(apnType) \n"},
    {"getPsimDataUsage", CMD_GET_PSIM_DATA_USAGE, commandActionData, "getPsimDataUsage - Get PSIM DataUsage \n"},
    {"setPsimDataUsageReset", CMD_SET_PSIM_DATA_USAGE_RESET, commandActionData, "setPsimDataUsageReset - Set PSIM DataUsage Reset \n"},
    {"setDataLimitSetting", CMD_SET_DATA_LIMIT_SETTING, commandActionData, "setDataLimitSetting - Set PSIM Data Limit Setting \n"},
    {"getDataLimitAlert", CMD_GET_DATA_LIMIT_ALERT, commandActionData, "getDataLimitAlert - Get PSIM Data Limit Alert \n"},
    {"getDataLimit", CMD_GET_DATA_LIMIT, commandActionData, "getDataLimit - Get PSIM Data Limit \n"},
    {"getDataLimitBillDate", CMD_GET_DATA_LIMIT_BILL_DATE, commandActionData, "getDataLimitBillDate - Get PSIM Data Limit Bill Date \n"},
    //TIGER-252-Start
    {"getApnList", CMD_GET_DATA_GET_APN_LIST, commandActionData, "getApnList - Get Apn List \n"},
    {"removeApn", CMD_GET_DATA_REMOVE_APN, commandActionData, "removeApn - Remove APN\n"},
    {"addApn", CMD_GET_DATA_ADD_APN, commandActionData, "addAPNByMccMnc - Add APN \n"},
    {"updateApn", CMD_GET_DATA_UPDATE_APN, commandActionData, "updateAPNByIdx - Update APN \n"},
    {"resetApnToDefault", CMD_GET_DATA_RESET_APN_TO_DEFAULT, commandActionData, "resetApnToDefault - Reset APN to Default \n"},
    {"setPreferredApn", CMD_GET_DATA_SET_PREFERRED_APN, commandActionData, "setPreferredApn - Set preferred APN \n"},
    {"getPreferredApn", CMD_GET_DATA_GET_PREFERRED_APN, commandActionData, "getPreferredApn - Get preferred apn \n"},
    //TIGER-252-End

    //MPDN_Start
    {"setupData", CMD_SETUP_DATA, commandActionData, "setupData - Brings up the data for a particular ApnType, if there is no existing such connection.\n"},
    {"disconnectData", CMD_DISCONNECT_DATA, commandActionData, "disconnectData - Brings down the data for a particular ApnType, if there is no more usages of this connection.\n"},
    //MPDN_End

    {NULL, NULL, NULL, NULL}
};


//-----------------------------------------------------------------------------

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))

static char* usage_data_id(int32_t id);
static int32_t getCommandId(char* string);
static int32_t getHiddenCommandId(char* string);
static int32_t checkArguments(int32_t argc, char* argv[]);

//-----------------------------------------------------------------------------

class MyPhoneStateListener : public PhoneStateListener
{
public:
    virtual void onDataConnectionStateChanged(int32_t state, int32_t networkType) {
        printMessage("***  UNKNOWN(-1), DISCONNECTED(0), CONNECTING(1), CONNECTED(2), SUSPENDED(3) \n");
        printMessage("***  onDataConnectionStateChanged: state (%d) \n", state);
    }

    virtual void onDataConnectionFailed(const char* reason, const char* apnType) {
        printMessage("***  onDataConnectionFailed: reason(%s), apnType(%s) \n", reason, apnType);
    }

    virtual void onDataActivity(int32_t direction) {
        printMessage("***  onDataActivity: direction(%d) \n", direction);
    }
};

static sp<PhoneStateListener> sPhoneStateListener = new MyPhoneStateListener();

//-----------------------------------------------------------------------------

bool commandActionData(int32_t argc, char **argv) {
    sp<IDataService> ds = interface_cast<IDataService>
                (defaultServiceManager()->getService(
                    String16("service_layer.DataService")));

    if (ds == NULL) {
        printMessage("Data service is not ready, try later\n");
        return false;
    }

    int32_t result = checkArguments(argc, argv);
    if (result == -1) {
        printMessage("%s", usage_data_id(0));
        return true;
    } else if (result > 0) {
        printMessage("%s", usage_data_id(result));
        return true;
    } else {
        // normal case
    }

    int32_t id = getCommandId(argv[0]);
    if (id == -1) {
        printMessage("%s", usage_data_id(0));
        return true;
    }

    switch(id) {
        case CMD_DATA_CONNECTION_SATAE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Data Connection State\n");
            printMessage("-------------------------------------------------\n");
            int32_t state = ds->getDataState();
            if (state < PhoneConstants::CONNECTED || state > PhoneConstants::DISCONNECTED) {
                printMessage(" -> result : Unknown\n", state);
            } else {
                printMessage(" -> result : %s\n", PhoneConstants::DataStateString[state]);
            }
        } break;

        case CMD_DATA_ACTIVITY_STATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Data Activity State\n");
            printMessage("-------------------------------------------------\n");
            int32_t state = ds->getDataActivityState();
            if (state < PhoneConstants::NONE || state > PhoneConstants::DORMANT) {
                printMessage(" -> result : Unknown\n", state);
            } else {
                printMessage(" -> result : %s\n", PhoneConstants::DataActivityString[state]);
            }
        } break;

        case CMD_GET_IP_ADDRESS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get IP Address\n");
            printMessage("-------------------------------------------------\n");
            sp<String> ip = ds->getIPAddress();
            printMessage(" -> result : %s\n", CSTR(ip));
        } break;

        case CMD_GET_DNS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get DNS\n");
            printMessage("-------------------------------------------------\n");
            sp<String> dns = ds->getDns();
            printMessage(" -> result : %s\n", CSTR(dns));
        } break;

        case CMD_GET_APN: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get APN\n");
            printMessage("-------------------------------------------------\n");
            sp<String> apn = ds->getAPN();
            printMessage(" -> result : %s\n", CSTR(apn));
        } break;

        case CMD_GET_APN_NAME: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get APN Name\n");
            printMessage("-------------------------------------------------\n");
            sp<String> apnName = ds->getAPNName();
            printMessage(" -> result : %s\n", CSTR(apnName));
        } break;

        case CMD_GET_APN_USER_ID: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get APN UserID\n");
            printMessage("-------------------------------------------------\n");
            sp<String> apnUserid = ds->getAPNUserID();
            printMessage(" -> result : %s\n", CSTR(apnUserid));
        } break;

        case CMD_GET_APN_PASSWORD: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get APN Password\n");
            printMessage("-------------------------------------------------\n");
            sp<String> apnPassword = ds->getAPNPassword();
            printMessage(" -> result : %s\n", CSTR(apnPassword));
        } break;

        case CMD_SET_APN: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Set APN\n");
            printMessage("-------------------------------------------------\n");
            printMessage(" -> setAPN : radioTechnology : %s\n", argv[1]);
            printMessage(" -> setAPN : profile : %s\n", argv[2]);
            printMessage(" -> setAPN : apn : %s\n", argv[3]);
            printMessage(" -> setAPN : user : %s\n", argv[4]);
            printMessage(" -> setAPN : password : %s\n", argv[5]);
            printMessage(" -> setAPN : authType : %s\n", argv[6]);
            printMessage(" -> setAPN : protocol : %s\n", argv[7]);
            printMessage("-------------------------------------------------\n");
            ds->setAPN(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);
        } break;

        case CMD_SET_MANUAL_APN_SETTING: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Manual APN Setting\n");
            printMessage("-------------------------------------------------\n");
            printMessage(" -> APN Address      : %s\n", argv[1]);
            printMessage(" -> APN UserName     : %s\n", argv[2]);
            printMessage(" -> APN Password     : %s\n", argv[3]);
            printMessage(" -> Set Activie SIM  : %s\n", argv[4]);
            printMessage("-------------------------------------------------\n");
            ds->setManualApnSetting(argv[1], argv[2], argv[3], (!strcmp(argv[4], "TSIM")) ? 0 : 1);
        } break;

        case CMD_MANUAL_DATA_CONNECT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Manual APN Setting and Data Connect \n");
            printMessage("-------------------------------------------------\n");
            printMessage(" -> APN Address      : %s\n", argv[1]);
            printMessage(" -> APN UserName     : %s\n", argv[2]);
            printMessage(" -> APN Password     : %s\n", argv[3]);
            printMessage(" -> Set Activie SIM  : %s\n", argv[4]);
            printMessage("-------------------------------------------------\n");
            ds->setManualConnect(argv[1], argv[2], argv[3], (!strcmp(argv[4], "TSIM")) ? 0 : 1);
        } break;

        case CMD_GET_DATA_USAGE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get DATA USAGE\n");
            printMessage("-------------------------------------------------\n");
            int64_t datausage = ds->getDataUsage(argv[1]);
            printMessage(" -> [%s] %lld Byte \n", argv[1], datausage);
        } break;

        case CMD_GET_DATA_USAGE_LIST: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Data Usage Print\n");
            printMessage("        (TX Byte, RX Byte, TX Packets, RX, PAckets)\n");
            printMessage("-------------------------------------------------\n");
            sp<String> datausagelist = ds->getDataUsageList(argv[1]);
            printMessage(" -> [%s] %s\n", argv[1], CSTR(datausagelist));
        } break;

        case CMD_GET_PACKET_LOSS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get PACKET LOSS\n");
            printMessage("-------------------------------------------------\n");
            sp<String> packloss = ds->getPacketLoss(argv[1]);
            printMessage(" -> [%s] %s\n", argv[1], CSTR(packloss));
        } break;

        case CMD_GET_ERROR_BLOCKS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get ERROR BLOCKS\n");
            printMessage("-------------------------------------------------\n");
            sp<String> errorblocks = ds->getErrorBlocks(argv[1]);
            printMessage(" -> [%s] %s\n", argv[1], CSTR(errorblocks));
        } break;

        case CMD_SET_USER_DATA_ENABLE: {
            bool enabled = true;
            if (!strcmp(argv[1], "off")) {
                enabled = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set User Data Enabled : %s\n", enabled ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            ds->setUserDataEnable(enabled);
        } break;

        case CMD_SET_INTERNAL_DATA_ENABLE: {
            bool enabled = true;
            if (!strcmp(argv[1], "off")) {
                enabled = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set Internal Data Enable : %s\n", enabled ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            ds->setInternalDataEnable(enabled);
        } break;

        case CMD_GET_DATA_ROAMING_ENABLE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get Data Roaming Enable\n");
            printMessage("-------------------------------------------------------\n");
            bool enabled = ds->getDataRoamingEnabled();
            printMessage(" -> result : %s\n", BOOL_STR(enabled));
        } break;

        case CMD_SET_DATA_ROAMING_ENABLE: {
            bool enable = true;
            if (!strcmp(argv[1], "off")) {
                enable = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set Data Roaming Enabled : %s\n", enable ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            ds->setDataRoamingEnabled(enable);
        } break;

        case CMD_GET_DID_APN_SETTING_ENABLE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get DID APN Setting Enabled \n");
            printMessage("-------------------------------------------------------\n");
            bool enabled = ds->getDidApnSettingEnabled();
            printMessage(" -> result : %s\n", BOOL_STR(enabled));
        } break;

        case CMD_SET_DID_APN_SETTING_ENABLE: {
            bool enabled = true;
            if (!strcmp(argv[1], "off")) {
                enabled = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set DID APN Setting Enabled(Only TSIM) : %s\n", enabled ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            ds->setDidApnSettingEnabled(enabled);
        } break;

        case CMD_GET_AUTO_SELECT_ENABLE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get Auto Select Enabled(Default is TRUE)\n");
            printMessage("-------------------------------------------------------\n");
            bool enabled = ds->getAutoSelectEnabled();
            printMessage(" -> result : %s\n", BOOL_STR(enabled));
        } break;

        case CMD_SET_AUTO_SELECT_ENABLE: {
            bool enabled = true;
            if (!strcmp(argv[1], "off")) {
                enabled = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set Auto Select Enabled(PSIM Connection) : %s\n", enabled ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            ds->setAutoSelectEnabled(enabled);
        } break;

        case CMD_GET_ACTIVE_APN_STRING: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get Active APN String \n");
            printMessage("-------------------------------------------------\n");
            sp<String> apn = ds->getActiveApnString(argv[1]);
            printMessage(" -> Input apnType: %s \n", argv[1]);
            printMessage(" -> Result APN : %s \n", (apn != NULL) ? CSTR(apn) : "No Support");
        } break;

        case CMD_GET_PSIM_DATA_USAGE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get PSIM Data Usage\n");
            printMessage("-------------------------------------------------\n");
            int64_t datausage = ds->getPsimDataUsage();
            printMessage(" -> result : %lld Byte \n", datausage);
            printMessage(" -> result : %.2f KB \n", (float)datausage/(float)1024);
            printMessage(" -> result : %.2f MB \n", (float)datausage/(float)(1024*1024));
            printMessage(" -> result : %.2f GB \n", (float)datausage/(float)(1024*1024*1024));
        } break;

        case CMD_SET_PSIM_DATA_USAGE_RESET: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Set PSIM Data Usage Reset\n");
            printMessage("-------------------------------------------------\n");
            ds->setPsimDataUsageReset();
        } break;

        case CMD_SET_DATA_LIMIT_SETTING: {
            int64_t dataLimit = 0;
            sscanf(argv[2], "%lld", &dataLimit);

            printMessage("-------------------------------------------------\n");
            printMessage("    Set Data Limit Setting \n");
            printMessage("-------------------------------------------------\n");
            printMessage(" -> Data Limit Alert      : %d(%s)\n", sl::atoi(argv[1]), (sl::atoi(argv[1])==1) ? "TRUE" : "FALSE");
            printMessage(" -> Data Limit(Byte)      : %lld Byte\n", dataLimit);
            printMessage(" -> Data Limit(MB)        : %.2f MB\n", (float)dataLimit/(float)(1024*1024));
            printMessage(" -> Data Limit(GB)        : %.4f GB\n", (float)dataLimit/(float)(1024*1024*1024));
            printMessage(" -> Data Limit Bill Date  : %d Day\n", sl::atoi(argv[3]));
            printMessage("-------------------------------------------------\n");

            ds->setDataLimitSetting(sl::atoi(argv[1]), dataLimit, sl::atoi(argv[3]));
        } break;

        case CMD_GET_DATA_LIMIT_ALERT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Set PSIM Data Limit Setting \n");
            printMessage("-------------------------------------------------\n");
            bool alert = ds->getDataLimitAlert();
            printMessage(" -> result : %s\n", BOOL_STR(alert));
        } break;

        case CMD_GET_DATA_LIMIT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get PSIM Data Limit \n");
            printMessage("-------------------------------------------------\n");
            int64_t dataLimit = ds->getDataLimit();
            printMessage(" -> result : %lld Byte \n", dataLimit);
            printMessage(" -> result : %.2f KB \n", (float)dataLimit/(float)1024);
            printMessage(" -> result : %.2f MB \n", (float)dataLimit/(float)(1024*1024));
            printMessage(" -> result : %.2f GB \n", (float)dataLimit/(float)(1024*1024*1024));

        } break;

        case CMD_GET_DATA_LIMIT_BILL_DATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get PSIM Data Limit Bill Date\n");
            printMessage("-------------------------------------------------\n");
            int32_t dataLimitBillDate = ds->getDataLimitBillDate();
            printMessage(" -> result : %d \n", dataLimitBillDate);
        } break;

        //TIGER-252-Start
        case CMD_GET_DATA_GET_APN_LIST: {
            printMessage("-------------------------------------------------\n");
            printMessage("    Get APN List \n");
            printMessage("-------------------------------------------------\n");
            printMessage("[func]%s[line]%d", __func__, __LINE__);
            Vector< sp<ApnSetting> > apnList= ds->getApnList();
            printMessage("[func]%s[line]%d", __func__, __LINE__);
            uint32_t size= apnList.size();
            printMessage("[func]%s[line]%d", __func__, __LINE__);
            for(uint32_t i=0; i< size; i++)
            {
                printMessage("Index (%d)\n", i);
                printMessage(CSTR(apnList.itemAt(i).get()->toString()));
                printMessage("\n");
            }
            printMessage("\n_________The End_____________\n");
        } break;
        case CMD_GET_DATA_REMOVE_APN:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Remove APN \n");
            printMessage("-------------------------------------------------\n");
            bool status= ds->removeApn(sl::atoi(argv[1]));
            printMessage(" -> result : %s \n", (status==false?"FALSE":"TRUE"));
        } break;
        case CMD_GET_DATA_ADD_APN:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Add APN int:id, str:numeric, str:carrier, str:apn,\
                str:proxy, str:port,\
                str:user, str:password, int:authType, str:types,\
                str:protocol, str:roamingProtocol, bool:carrierEnabled, int:bearer,\
                int:bearerBitmask, int:profileId, bool:modemCognitive, int:maxConns, int:waitTime,\
                int:maxConnsTime\n");
            printMessage("-------------------------------------------------\n");
            sp<ApnSetting> tempApnsetting= new ApnSetting(sl::atoi(argv[1]), new String(argv[2]), new String(argv[3]), new String(argv[4]),
                new String(argv[5]), new String(argv[6]),
                new String(argv[7]), new String(argv[8]), new String(argv[9]), new String(argv[10]),
                new String(argv[11]), new String(argv[12]), sl::atoi(argv[13]), sl::atoi(argv[14]),
                sl::atoi(argv[15]), sl::atoi(argv[16]), sl::atoi(argv[17]), sl::atoi(argv[18]), sl::atoi(argv[19]),
                sl::atoi(argv[20]));
            bool status= ds->addApn(tempApnsetting);
            printMessage(" -> result : %s \n", (status==false?"FALSE":"TRUE"));
        } break;
        case CMD_GET_DATA_UPDATE_APN:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Update APN (int:apnIdx), (int:id, str:numeric, str:carrier, str:apn,\
                str:proxy, str:port,\
                str:user, str:password, str:authType, str:types,\
                str:protocol, str:roamingProtocol, int:carrierEnabled, int:bearer,\
                int:bearerBitmask, int:profileId, int:modemCognitive, int:maxConns, int:waitTime,\
                int:maxConnsTime)\n");
            printMessage("-------------------------------------------------\n");
            sp<ApnSetting> tempApnsetting= new ApnSetting(sl::atoi(argv[2]), new String(argv[3]), new String(argv[4]), new String(argv[5]),
                new String(argv[6]), new String(argv[7]),
                new String(argv[8]), new String(argv[9]), new String(argv[10]), new String(argv[11]),
                new String(argv[12]), new String(argv[13]), sl::atoi(argv[14]), sl::atoi(argv[15]),
                sl::atoi(argv[16]), sl::atoi(argv[17]), sl::atoi(argv[18]), sl::atoi(argv[19]), sl::atoi(argv[20]),
                sl::atoi(argv[21]));
            bool status= ds->updateApn(sl::atoi(argv[1]), tempApnsetting);
            printMessage(" -> result : %s \n", (status==false?"FALSE":"TRUE"));
        } break;
        case CMD_GET_DATA_RESET_APN_TO_DEFAULT:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Reset APN to Default \n");
            printMessage("-------------------------------------------------\n");
            bool status= ds->resetApnToDefault();
            printMessage(" -> result : %s \n", (status==false?"FALSE":"TRUE"));
        } break;
        case CMD_GET_DATA_SET_PREFERRED_APN:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Set preferred APN \n");
            printMessage("-------------------------------------------------\n");
            bool status= ds->setPreferredApn(sl::atoi(argv[1]));
            printMessage(" -> result : %s \n", (status==false?"FALSE":"TRUE"));
        } break;
        case CMD_GET_DATA_GET_PREFERRED_APN:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Get preferred APN \n");
            printMessage("-------------------------------------------------\n");
            sp<ApnSetting> tempApnSetting= ds->getPreferredApn();
            printMessage(" -> result : %s \n", CSTR(tempApnSetting.get()->toString()));
        } break;
        //TIGER-252-End

        //MPDN_Start
        case CMD_SETUP_DATA:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Setup Data \n");
            printMessage("-------------------------------------------------\n");
            int32_t status= ds->setupData(argv[1]);
            printMessage(" -> result : %d \n", status);
        } break;
        case CMD_DISCONNECT_DATA:{
            printMessage("-------------------------------------------------\n");
            printMessage("    Disconnect Data \n");
            printMessage("-------------------------------------------------\n");
            int32_t status= ds->disconnectData(argv[1]);
            printMessage(" -> result : %d \n", status);
        } break;
        //MPDN_End

        default:
            printMessage("-------------------------------------------------------\n");
            printMessage("    Not command : %d\n", sl::atoi(argv[0]));
            printMessage("-------------------------------------------------------\n");
            break;
    }

    return true;
}

char* usage_data(char* cmd) {
    return usage_data_id(getCommandId(cmd));
}

static int32_t checkArguments(int32_t argc, char* argv[]) {
    if (argc < 1) {
        return -1;
    }

    int32_t cmdId = getHiddenCommandId(argv[0]);
    if (cmdId == -1) {
        cmdId = getCommandId(argv[0]);
    }

    int32_t result = -1;
    switch(cmdId) {
        //Tiger-252-Start
        case CMD_GET_DATA_GET_APN_LIST:         //FALL-THROUGH
        case CMD_GET_DATA_RESET_APN_TO_DEFAULT: //FALL-THROUGH
        case CMD_GET_DATA_GET_PREFERRED_APN:    //FALL-THROUGH
        //Tiger-252-End
        case CMD_DATA_CONNECTION_SATAE:         //FALL-THROUGH
        case CMD_GET_IP_ADDRESS:                //FALL-THROUGH
        case CMD_GET_DNS:                       //FALL-THROUGH
        case CMD_GET_APN:                       //FALL-THROUGH
        case CMD_GET_APN_NAME :                 //FALL-THROUGH
        case CMD_GET_APN_USER_ID:               //FALL-THROUGH
        case CMD_GET_APN_PASSWORD  :            //FALL-THROUGH
        case CMD_GET_DID_APN_SETTING_ENABLE:    //FALL-THROUGH
        case CMD_GET_AUTO_SELECT_ENABLE:        //FALL-THROUGH
        case CMD_DATA_ACTIVITY_STATE:           //FALL-THROUGH
        case CMD_GET_DATA_ROAMING_ENABLE:       //FALL-THROUGH
        case CMD_GET_PSIM_DATA_USAGE:           //FALL-THROUGH
        case CMD_SET_PSIM_DATA_USAGE_RESET:     //FALL-THROUGH
        case CMD_GET_DATA_LIMIT_ALERT:          //FALL-THROUGH
        case CMD_GET_DATA_LIMIT:                //FALL-THROUGH
        case CMD_GET_DATA_LIMIT_BILL_DATE:
            if (argc == 1) {
                result = 0;
            }
            break;

        //Tiger-252-Start
        case CMD_GET_DATA_REMOVE_APN:           //FALL-THROUGH
        case CMD_GET_DATA_SET_PREFERRED_APN:    //FALL-THROUGH
        //Tiger-252-End

        //MPDN_Start
        case CMD_SETUP_DATA:                    //FALL-THROUGH
        case CMD_DISCONNECT_DATA:               //FALL-THROUGH
        //MPDN_End

        case CMD_GET_DATA_USAGE:                //FALL-THROUGH
        case CMD_GET_DATA_USAGE_LIST:           //FALL-THROUGH
        case CMD_GET_PACKET_LOSS:               //FALL-THROUGH
        case CMD_GET_ERROR_BLOCKS:              //FALL-THROUGH
        case CMD_SET_USER_DATA_ENABLE:          //FALL-THROUGH
        case CMD_SET_INTERNAL_DATA_ENABLE:      //FALL-THROUGH
        case CMD_SET_DATA_ROAMING_ENABLE:       //FALL-THROUGH
        case CMD_SET_AUTO_SELECT_ENABLE:        //FALL-THROUGH
        case CMD_SET_DID_APN_SETTING_ENABLE:    //FALL-THROUGH
        case CMD_GET_ACTIVE_APN_STRING:
            if (argc == 2) {
                result = 0;
            }
            break;
        case CMD_SET_DATA_LIMIT_SETTING:
            if (argc == 4) {
                result = 0;
            } else {
                result = cmdId;
            }
            break;
        case CMD_SET_MANUAL_APN_SETTING:        //FALL-THROUGH
        case CMD_MANUAL_DATA_CONNECT:
            if (argc == 5) {
                result = 0;
            } else {
                result = cmdId;
            }
            break;
        case CMD_SET_APN:
            if (argc == 8) {
                result = 0;
            } else {
                result = cmdId;
            }
            break;
        //Tiger-252-Start
        case CMD_GET_DATA_ADD_APN:
            if (argc == 21) {
                result = 0;
            }
            break;
        case CMD_GET_DATA_UPDATE_APN:
            if (argc == 22) {
                result = 0;
            }
            break;
        //Tiger-252-End
        default:
            break;
    }

    return result;
}

static int32_t getCommandId(char* string) {
    for (int32_t i=0; data_cmds[i].id != -1; i++) {
        if (!strcmp(data_cmds[i].name, string)) {
            return data_cmds[i].id;
        }
    }
    return -1;
}

static int32_t getHiddenCommandId(char* string) {
    const char* hiddenCommands[] = {
        "listen",
        NULL
    };

    for (int32_t i=0; hiddenCommands[i] != NULL; i++) {
        sp<String> name = new String(hiddenCommands[i]);
        if (name->equalsIgnoreCase(string)) {
            return i+CMD_LISTEN;
        }
    }

    return -1;
}

static char* usage_data_id(int32_t id) {
    return " sldd data [<command>] [parameters] \n"
            "    <command> \n"
            "       getstate                            - Get Data Connection State \n"
            "       getactivity                         - Get Data Activity State \n"
            "       getip                               - Get IP Address \n"
            "       getdns                              - Get DNS \n"
            "       getapn                              - Get APN \n"
            "       getname                             - Get APN Name \n"
            "       getuser                             - Get APN UserID \n"
            "       getpassword                         - Get APN Password \n"
            "       getDidApnSettingEnabled             - Get DID APN Setting Enabled \n"
            "       getAutoSelectEnabled                - Get Auto Select Enabled(Default is TRUE) \n"
            "       getDataRoamingEnabled               - Get Data Roaming Enabled \n"
            "       getPsimDataUsage                    - Get PSIM Data Usage \n"
            "       setPsimDataUsageReset               - Set PSIM Data Usage Reset \n"
            "       getDataLimitAlert                   - Get PSIM Data Limit Alert \n"
            "       getDataLimit                        - Get PSIM Data Limit \n"
            "       getDataLimitBillDate                - Get PSIM Data Limit Bill Date \n"
            " \n "
            "    <command> <parameters>\n"
            "       setDidApnSettingEnabled <parameters>   - Set DID Apn Setting Enabled(Only TSIM) (on/off) \n"
            "          [parameters] : on or off \n"
            "          ex) sldd data setDidApnSettingEnabled on(or off) \n"
            " \n "
            "       setDataLimitSetting <parameters>    - Set Data Limit \n"
            "          [parameters] : alert(on/off), dataLimit(Byte), dataLimitBillDate(Day) \n"
            "          ex) sldd data setDataLimitSetting 1, 200, 3 \n"
            " \n "
            "       getusage <parameters>               - Get Data Usage \n"
            "          [parameters] : dirver interface \n"
            "          ex) sldd data getusage rmnet0 \n"
            " \n "
            "       getusagelist <parameters>           - Get Data Usage Print(TX Byte, RX Byte, TX Packets, RX, PAckets) \n"
            "          [parameters] : dirver interface \n"
            "          ex) sldd data getusagelist rmnet0 \n"
            " \n "
            "       getpacketloss <parameters>          - Get Packet Loss \n"
            "          [parameters] : dirver interface \n"
            "          ex) sldd data getpacketloss rmnet0 \n"
            " \n "
            "       geterrorblocks <parameters>         - Get Error Blocks \n"
            "          [parameters] : dirver interface \n"
            "          ex) sldd data geterrorblocks rmnet0 \n"
            " \n "
            "       getActiveApnString <parameters>     - Get Active ApnString \n"
            "          [parameters] : Apn Type \n"
            "          ex) sldd data getActiveApnString supl \n"
            " \n "
            "       setManualApnSetting <parameters>    - Set Manual APN Setting \n"
            "          [parameters] : apn, user, password, activeSim(TSIM or PSIM) \n"
            "          ex) sldd data setManualApnSetting apn user password PSIM \n"
            " \n "
            "       setManualConnect <parameters>       - Set PSIM Manual Setting and Data Connect \n"
            "          [parameters] : apn, user, password, activeSim(TSIM or PSIM) \n"
            "          ex) sldd data setManualConnect lte.sktelecom.com NULL NULL PSIM \n"
            " \n "
            "       setUserDataEnabled <parameters>     - User Data Enabled Setting (on/off) \n"
            "          [parameters] : on or off \n"
            "          ex) sldd data setUserDataEnabled on(or off) \n"
            " \n "
            "       setInternalDataEnable <parameters>  - Internal Data Enabled Setting (on/off) \n"
            "          [parameters] : on or off \n"
            "          ex) sldd data setInternalDataEnable on(or off) \n"
            " \n "
            "       setDataRoamingEnabled <parameters>  - Data Roaming Enabled Setting (on/off) \n"
            "          [parameters] : on or off \n"
            "          ex) sldd data setDataRoamingEnabled on(or off) \n"
            " \n "
            "       setAutoSelectEnabled <parameters>   - Set Auto Select Enabled(PSIM Connection) (on/off) \n"
            "          [parameters] : on or off \n"
            "          ex) sldd data setAutoSelectEnabled on(or off) \n";
}

void register_data() {
    registerCommands(MODULE_DATA, NULL, data_cmds);
}
