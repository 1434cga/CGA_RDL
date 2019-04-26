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


#include <stdlib.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

//#include "ApplicationTable.h"
//#include "AppManager.h"
//#include "Post.h"
#include "Log.h"
#include "OperationalMode.h"
#include "services/OperationModeManagerService/IOperationModeManagerServiceType.h"
#include "services/NGTPManagerService/INGTPManagerServiceType.h"
#include "services/ApplicationManagerService/IApplicationManagerService.h"
#if defined(_DEVICE_NAD)
#include "services/ConfigurationManagerService/IConfigurationManagerService.h"
#include "services/ConfigurationManagerService/ConfigurationManagerService.h"
#include "services/NGTPManagerService/INGTPManagerService.h"
//#include "services/NGTPManagerService/NGTPManagerService.h"
#endif
#include "services/OperationModeManagerService/IOperationModeManagerService.h"
#include "utils/atoi.h"

#include "daemon/sldd/sldd_common.h"
#include "daemon/sldd/NGTP.h"
#include "daemon/sldd/man.h"
#include "services/ConfigurationManagerService/IConfigurationManagerServiceType.h"

#define LOG_TAG "NGTP"

using namespace android;

//#define CONFIGURATION_PATH "jlr_tcu_data_dictionary_swr_9_3.xml"
#define CONFIGURATION_PATH configmgr::OEM_CONFIG_FILE

enum ngtp_commands {
    CMD_NGTP_GET_EQUIPMENT_ID =1,
    CMD_NGTP_CLEAR_EQUIPMENT_ID,
    CMD_NGTP_SET_VIN,
    CMD_NGTP_SET_SERIAL,
    CMD_NGTP_GET_VIN,
    CMD_NGTP_GET_SERIAL,
    CMD_NGTP_SET_EQUIPMENT_ID,
    CMD_NGTP_FTUPDATE,
    CMD_NGTP_SET_ENC_KEY,
    CMD_NGTP_GET_ENC_KEY,
    CMD_NGTP_GET_TSP_IP,
    CMD_NGTP_SET_TSP_IP,
    CMD_NGTP_GET_COMET_ADDRESS,
    CMD_NGTP_SET_COMET_ADDRESS,
    CMD_NGTP_SET_MAX_UDP_RETRY,
    CMD_NGTP_GET_MAX_UDP_RETRY,
    CMD_NGTP_SET_UDP_DELAY,
    CMD_NGTP_GET_UDP_DELAY,
    CMD_NGTP_SET_MAX_SMS_RETRY,
    CMD_NGTP_GET_MAX_SMS_RETRY,
    CMD_NGTP_SET_SMS_DELAY,
    CMD_NGTP_GET_SMS_DELAY,
    CMD_NGTP_SET_MAX_HTTP_RETRY,
    CMD_NGTP_GET_MAX_HTTP_RETRY,
    CMD_NGTP_SET_HTTP_DELAY,
    CMD_NGTP_GET_HTTP_DELAY,
    CMD_NGTP_SET_DECODER_LOG,
    CMD_NGTP_GET_DECODER_LOG,
    CMD_NGTP_SEND_SVC_DATA_TO_APP,
    CMD_NGTP_SET_TOPS_FLAG,
    CMD_NGTP_GET_TOPS_FLAG,
    //---------------------------------
    NGTP_CMD_DUMP,
};

static bool handler_ngtp_get_equipment_id(int32_t argc, char **argv);
static bool handler_ngtp_clear_equipment_id(int32_t argc, char **argv);
static bool handler_ngtp_set_vin(int32_t argc, char **argv);
static bool handler_ngtp_set_serial(int32_t argc, char **argv);

static bool handler_ngtp_get_vin(int32_t argc, char **argv);
static bool handler_ngtp_get_serial(int32_t argc, char **argv);

static bool handler_ngtp_set_equipment_id(int32_t argc, char **argv);
static bool handler_ngtp_ftupdate(int32_t argc, char **argv);
static bool handler_ngtp_dump(int32_t argc, char **argv);
static bool handler_ngtp_get_enc_key(int32_t argc, char **argv);
static bool handler_ngtp_set_enc_key(int32_t argc, char **argv);
static bool handler_ngtp_get_tsp_ip(int32_t argc, char **argv);
static bool handler_ngtp_set_tsp_ip(int32_t argc, char **argv);
static bool handler_ngtp_get_comet_address(int32_t argc, char **argv);
static bool handler_ngtp_set_comet_address(int32_t argc, char **argv);

//---------------------------------------------------------------------
static bool handler_ngtp_set_max_udp_retry(int32_t argc, char **argv);
static bool handler_ngtp_get_max_udp_retry(int32_t argc, char **argv);

static bool handler_ngtp_set_udp_delay(int32_t argc, char **argv);
static bool handler_ngtp_get_udp_delay(int32_t argc, char **argv);

static bool handler_ngtp_set_max_sms_retry(int32_t argc, char **argv);
static bool handler_ngtp_get_max_sms_retry(int32_t argc, char **argv);

static bool handler_ngtp_set_sms_delay(int32_t argc, char **argv);
static bool handler_ngtp_get_sms_delay(int32_t argc, char **argv);

static bool handler_ngtp_set_max_http_retry(int32_t argc, char **argv);
static bool handler_ngtp_get_max_http_retry(int32_t argc, char **argv);

static bool handler_ngtp_set_http_delay(int32_t argc, char **argv);
static bool handler_ngtp_get_http_delay(int32_t argc, char **argv);

static bool handler_ngtp_set_decoder_log(int32_t argc, char **argv);
static bool handler_ngtp_get_decoder_log(int32_t argc, char **argv);

static bool handler_send_svc_data_to_app(int32_t argc, char **argv);

static bool handler_set_tops_flag(int32_t argc, char **argv);
static bool handler_get_tops_flag(int32_t argc, char **argv);
/**
*    Declare binder interface
*
*/
#if defined(_DEVICE_NAD)
SL_INTERFACE_DECLARE(ConfigurationManagerService);
#endif

SL_INTERFACE_DECLARE(ApplicationManagerService);


sldd_cmd_table_t ngtp_cmdTable[] = {
    {"get_equipment_id", CMD_NGTP_GET_EQUIPMENT_ID, handler_ngtp_get_equipment_id,
     "  get_equipment_id           - display current equipment id in factory test \n"},
    {"clear_equipment_id", CMD_NGTP_CLEAR_EQUIPMENT_ID, handler_ngtp_clear_equipment_id,
     "  clear_equipment_id         - clean current equipment id \n"},
    {"set_vin", CMD_NGTP_SET_VIN, handler_ngtp_set_vin,
     "  set_vin <number string>    - assign vin number to new value \n"},
    {"set_serial", CMD_NGTP_SET_SERIAL, handler_ngtp_set_serial,
     "  set_serial <number string> - assign serial number to new value \n"},
    {"get_vin", CMD_NGTP_GET_VIN, handler_ngtp_get_vin,
     "  get_vin                    - get vin number to new value \n"},
    {"get_serial", CMD_NGTP_GET_SERIAL, handler_ngtp_get_serial,
     "  get_serial                 - get serial number to new value \n"},

    {"set_equipment_id", CMD_NGTP_SET_EQUIPMENT_ID, handler_ngtp_set_equipment_id,
     "  set_equipment_id <number string> - assign equipment id to new value \n"},
    {"ftupdate", CMD_NGTP_FTUPDATE, handler_ngtp_ftupdate,
     "  ftupdate                   - perform ftp update operation \n"},
    {"set_enc_key", CMD_NGTP_SET_ENC_KEY,  handler_ngtp_set_enc_key,
     "  set_enc_key                - set encryption key \n"},
    {"get_enc_key", CMD_NGTP_GET_ENC_KEY,  handler_ngtp_get_enc_key,
     "  get_enc_key                - display encryption key \n"},

    {"get_tsp_ip_port", CMD_NGTP_GET_TSP_IP,  handler_ngtp_get_tsp_ip,
     "  get_tsp_ip_port                - display tsp ip:port \n"},
    {"set_tsp_ip_port", CMD_NGTP_SET_TSP_IP,  handler_ngtp_set_tsp_ip,
     "  set_tsp_ip_port                - set tsp ip:port  ex) 71.2.3.100:51000 \n"},

    {"get_comet_server", CMD_NGTP_GET_COMET_ADDRESS,  handler_ngtp_get_comet_address,
     "  get_comet_server          - display comet server address ex) jlrowqa4-comet.wirelesscar.net\n"},
    {"get_set_comet_server", CMD_NGTP_SET_COMET_ADDRESS,  handler_ngtp_set_comet_address,
     "  get_set_comet_server      - set comet server address  ex) jlrowqa4-comet.wirelesscar.net \n"},

    {"dump", NGTP_CMD_DUMP, handler_ngtp_dump, NOT_IMPLEMENTED},


    //-----------------------------------------------------------------------------------

     {"set_max_udp_retry", CMD_NGTP_SET_MAX_UDP_RETRY, handler_ngtp_set_max_udp_retry,
     "  set_max_udp_retry <number string> - set max retry number in UDP \n"},
     {"get_max_udp_retry", CMD_NGTP_GET_MAX_UDP_RETRY, handler_ngtp_get_max_udp_retry,
     "  get_max_udp_retry    - get max retry number in UDP \n"},

     {"set_udp_delay", CMD_NGTP_SET_UDP_DELAY, handler_ngtp_set_udp_delay,
     "  set_udp_delay <number string> - set delay seconds in UDP \n"},
     {"get_udp_delay", CMD_NGTP_GET_UDP_DELAY, handler_ngtp_get_udp_delay,
     "  get_udp_delay    - get  delay seconds in UDP \n"},

     {"set_max_sms_retry", CMD_NGTP_SET_MAX_SMS_RETRY, handler_ngtp_set_max_sms_retry,
     "  set_max_sms_retry <number string> - set max retry number in SMS \n"},
     {"get_max_sms_retry", CMD_NGTP_GET_MAX_SMS_RETRY, handler_ngtp_get_max_sms_retry,
     "  get_max_sms_retry    - get max retry number in SMS \n"},

     {"set_sms_delay", CMD_NGTP_SET_UDP_DELAY, handler_ngtp_set_sms_delay,
     "  set_sms_delay <number string> - set delay seconds in SMS \n"},
     {"get_sms_delay", CMD_NGTP_GET_UDP_DELAY, handler_ngtp_get_sms_delay,
     "  get_sms_delay    - get delay seconds in SMS \n"},

     {"set_max_http_retry", CMD_NGTP_SET_MAX_HTTP_RETRY, handler_ngtp_set_max_http_retry,
     "  set_max_http_retry <number string> - set max retry number in HTTP \n"},
     {"get_max_http_retry", CMD_NGTP_GET_MAX_HTTP_RETRY, handler_ngtp_get_max_http_retry,
     "  get_max_http_retry    - get max retry number in HTTP \n"},

     {"set_http_delay", CMD_NGTP_SET_HTTP_DELAY, handler_ngtp_set_http_delay,
     "  set_http_delay <number string> - set delay seconds in HTTP\n"},
     {"get_http_delay", CMD_NGTP_GET_HTTP_DELAY, handler_ngtp_get_http_delay,
     "  get_http_delay    - get delay seconds in HTTP \n"},
     {"set_decoder_log", CMD_NGTP_SET_DECODER_LOG, handler_ngtp_set_decoder_log,
     "  set_decoder_log <number string> - set decoder log option 1-on 0-off"},
     {"get_decoder_log", CMD_NGTP_GET_DECODER_LOG, handler_ngtp_get_decoder_log,
     "  get_decoder_log    - get decoder log option \n"},
     {"send_svc_data", CMD_NGTP_SEND_SVC_DATA_TO_APP , handler_send_svc_data_to_app,
      " send_svc_data  <servicetype> <length> <hexbyte> - send svc data to app \n"
     },

     {"set_tops_flag", CMD_NGTP_SET_TOPS_FLAG , handler_set_tops_flag,
      " set_tops_flag  <flag> - enable :1 , disable : 0 \n"
     },

     {"get_tops_flag", CMD_NGTP_GET_TOPS_FLAG , handler_get_tops_flag,
      " get_tops_flag   - enable :1 , disable :0\n"
     },

    {NULL, NULL, NULL}
};

bool commandActionNGTP(int32_t argc, char **argv)
{

    if (argc == 0) {
        usage_ngtp(NULL);
        return true;
    }

    DO_HANDLER(ngtp_cmdTable, argv[0], argc-1, argv+1);
}


char* usage_ngtp(char *cmd)
{

    printMessage(" sldd ngtp [<command>]         - perform ngtp's functions \n");
    printMessage("    <command> \n");
    PRINT_USAGE(ngtp_cmdTable);
    return NULL;
}



bool handler_ngtp_get_equipment_id(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    sp<Buffer> rBuffer;

    printMessage(
        "==================================================\n"
        "   get equipment ID will be activated with Configuration manager \n"
        "==================================================\n");

    rBuffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string (CONFIGURATION_PATH ,2, "EquipmentId");


     printMessage(
        "===================================\n"
        "   Equipment ID : %s \n"
        "===================================\n", rBuffer->data());

#endif
    return true;
}


bool handler_ngtp_clear_equipment_id(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   clear equipment ID will be activated with Configuration manager \n"
        "==================================================\n");

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "EquipmentId", 0);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "   clear Equipment ID is failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   Equipment ID is cleared to 0 \n"
            "===================================\n");
        return true;
    }
#endif
    return false;
}

bool handler_ngtp_set_vin(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set VIN will be activated with Configuration manager , vin = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "VIN", argv[0]);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "   set VIN is failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   VIN is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return false;
}

bool handler_ngtp_set_serial(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set Serial will be activated with Configuration manager , serial = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "SerialNumber", argv[0]);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "   set serial is failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   Serial Number is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return false;
}

bool handler_ngtp_set_equipment_id(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set equipment ID will be activated with Configuration manager , equipement id = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "EquipmentId", argv[0]);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "   set equipment ID is failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   Equipment ID is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return false;
}

bool handler_ngtp_ftupdate(int32_t argc, char **argv)
{
    int32_t what = 0xFEBA;

    sp<Post> post = Post::obtain(what);
    error_t error =
            SL_INTERFACE_GET(ApplicationManagerService)->sendPost(1007, post);
    if(error == E_OK)
    {
        printMessage(
            "==============================\n"
            "   sending FTUpdate to ProvisioningApp Success\n"
            "==============================\n");
        return true;
    }
    else
    {
        printMessage(
            "==============================\n"
            "   sending FTUpdate to ProvisioningApp has failed\n"
            "==============================\n");
    }
    return false;
}

bool handler_ngtp_set_enc_key(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set encryption key = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "EncryptionKey", argv[0]);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "   set encryption key is failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "  Encryption key is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif

    return false;
}


bool handler_ngtp_get_enc_key(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    sp<Buffer> rBuffer;

    printMessage(
        "==================================================\n"
        "   get encryption key \n"
        "==================================================\n");

    rBuffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string(CONFIGURATION_PATH, 2, "EncryptionKey");


     printMessage(
        "===================================\n"
        "  Encryption key  %s \n"
        "===================================\n", rBuffer->data());

#endif

    return true;
}


bool handler_ngtp_dump(int32_t argc, char **argv)
{
    return false;
}

void register_ngtp()
{
    registerCommands(MODULE_NGTP_MANAGER, NULL, ngtp_cmdTable);
}



bool handler_ngtp_get_vin(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    sp<Buffer> rBuffer;

    printMessage(
        "==================================================\n"
        "   get VIN \n"
        "==================================================\n");

    rBuffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string(CONFIGURATION_PATH, 2, "VIN");


     printMessage(
        "===================================\n"
        "  VIN is %s \n"
        "===================================\n", rBuffer->data());

#endif

    return true;
}



bool handler_ngtp_get_serial(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    sp<Buffer> rBuffer;

    printMessage(
        "==================================================\n"
        "   get serial \n"
        "==================================================\n");

    rBuffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string(CONFIGURATION_PATH, 2, "SerialNumber");


     printMessage(
        "===================================\n"
        "  SerialNumber   %s \n"
        "===================================\n", rBuffer->data());

#endif

    return true;
}



static bool handler_ngtp_get_tsp_ip(int32_t argc, char **argv)
{

#if defined(_DEVICE_NAD)

    sp<Buffer> IPBuffer;
    int port;

    printMessage(
        "==================================================\n"
        "   get tsp ip \n"
        "==================================================\n");

    IPBuffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string(CONFIGURATION_PATH, 2, "MessageServerAddress");
    port  = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "MessageServerPort");

    printMessage(
        "===================================\n"
        "  TSP IP:Port   %s:%d \n"
        "===================================\n", IPBuffer->data(), port) ;// PortBuffer->data());

#endif

    return true;




}

uint8_t getCurrentOperationalMode()
{
   sp<IOperationModeManagerService> iOperationalMode;

    if (iOperationalMode == NULL) {
        iOperationalMode = interface_cast<IOperationModeManagerService>(defaultServiceManager()->getService(String16(OPERATION_SRV_NAME)));
    }
    return iOperationalMode->getCurrentOperationMode();
}


static bool handler_ngtp_set_tsp_ip(int32_t argc, char **argv)
{

#if defined(_DEVICE_NAD)

    sp<Buffer> IPBuffer;
    int8_t ret;
    char IP[15];
    char portstr[10];
    char  *ptr = NULL;
    char  *spaceptr = NULL;
    memset ( IP , 0, 15);
    memset ( portstr , 0, 10);


    ptr = strstr (argv[0] , ":");
    spaceptr = strstr(argv[0], " ");

    if ( ptr == NULL || spaceptr != NULL )
    {
        printMessage(
        "==================================================\n"
        "  wrong format error !!! use ip:port format like 172.10.3.22:52000 without space \n"
        "==================================================\n" );
    }
    else
    {


        uint8_t total_len = strlen (argv[0]);
        uint8_t port_len = strlen (ptr);
        strncpy (IP, argv[0], total_len - port_len  );
        strncpy (portstr , ptr+1, port_len -1 );

        int port = sl::atoi ( portstr);
        ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "MessageServerAddress", IP);
        ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "MessageServerPort", port );


        if(getCurrentOperationalMode() == OPERATIONAL_MODE_DEACTIVATED)
        {
            printMessage(
            "==================================================\n"
            "   set tsp ip : port  = %s \n"
            "   It's deactivated mode. This value will be applied from now. \n"
            "==================================================\n" ,argv[0]);
            sp<INGTPManagerService> iNGTP = interface_cast<INGTPManagerService>
                     (defaultServiceManager()->getService(
                         String16(NGTP_SRV_NAME)));
            iNGTP->readConfiguration();
        }
        else
        {
            printMessage(
            "==================================================\n"
            "   set tsp ip : port  = %s \n"
            "   This value will be applied after restart of the system\n"
            "==================================================\n" ,argv[0]);

        }

    }


#endif

    return true;


}




static bool handler_ngtp_get_comet_address(int32_t argc, char **argv)
{

#if defined(_DEVICE_NAD)

    sp<Buffer> Buffer;


    printMessage(
        "==================================================\n"
        "   get Comet server address \n"
        "==================================================\n");

    Buffer = SL_INTERFACE_GET(ConfigurationManagerService)->get_string(CONFIGURATION_PATH, 2, "CometServerAddress");


    printMessage(
        "===================================\n"
        "  Comet server address is  %s \n"
        "===================================\n", Buffer->data()) ;// PortBuffer->data());

#endif

    return true;




}



static bool handler_ngtp_set_comet_address(int32_t argc, char **argv)
{

#if defined(_DEVICE_NAD)

    sp<Buffer> IPBuffer;
    int8_t ret;

    printMessage(
        "==================================================\n"
        "   set comet address = %s \n"
        "==================================================\n" ,argv[0]);


    if ( strstr (argv[0] , " ") != NULL)
       printMessage(  "==================================================\n"
              "  wrong format error !!! input without space  !! \n"
                "==================================================\n");
    else
        ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_string(CONFIGURATION_PATH, 2, "CometServerAddress", argv[0]);



#endif

    return true;


}


//---------------------------------------------------------------------------------------

bool handler_ngtp_set_max_udp_retry(int32_t argc, char **argv)
{

    #if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max retry number in UDP , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPUDPMaxRetry", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting max retry number in UDP failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   max retry number in UDP is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return true;

}


bool handler_ngtp_get_max_udp_retry(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp max retry number in UDP \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPUDPMaxRetry");

    printMessage(
        "===================================\n"
        "  max retry in UDP  : %d \n"
        "===================================\n",  num) ;// PortBuffer->data());

#endif
    return true;
}

bool handler_ngtp_set_udp_delay(int32_t argc, char **argv)
{
    #if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max delay in UDP , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPUDPDelay", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting udp delay in UDP failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   udp delay in UDP is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return true;

}


bool handler_ngtp_get_udp_delay(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)


    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp delay number in UDP \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPUDPDelay");

    printMessage(
        "===================================\n"
        "  max retry in UDP  :%d \n"
        "===================================\n",num) ;// PortBuffer->data());

#endif
    return true;
}

bool handler_ngtp_set_max_sms_retry(int32_t argc, char **argv)
{
   #if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max retry number in SMS , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPSMSMaxRetry", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting max retry number in SMS failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   max retry number in SMS is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return true;
}


bool handler_ngtp_get_max_sms_retry(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp max retry number in SMS \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPSMSMaxRetry");

    printMessage(
        "===================================\n"
        "  max retry in SMS  : %d \n"
        "===================================\n",  num) ;// PortBuffer->data());

#endif
    return true;
}


bool handler_ngtp_set_sms_delay(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max delay in SMS , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPSMSDelay", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting delay in SMS failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   sms delay is in SMS set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return true;

}


bool handler_ngtp_get_sms_delay(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)


    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp delay number in SMS\n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPSMSDelay");

    printMessage(
        "===================================\n"
        "  max retry in SMS  :%d \n"
        "===================================\n",num) ;// PortBuffer->data());

#endif
    return true;
}

bool handler_ngtp_set_max_http_retry(int32_t argc, char **argv)
{
  #if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max retry number in HTTP , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPHTTPMaxRetry", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting max retry number in HTTP failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   max retry number in HTTP is set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return true;
}


bool handler_ngtp_get_max_http_retry(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp max retry number in HTTP \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPHTTPMaxRetry");

    printMessage(
        "===================================\n"
        "  max retry in HTTP  : %d \n"
        "===================================\n",  num) ;

#endif
    return true;
}

bool handler_ngtp_set_http_delay(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set ngtp max delay in SMS , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPHTTPDelay", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting delay in HTTP failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   sms delay is in HTTP set to %s \n"
            "===================================\n", argv[0]);
        return true;
    }
#endif
    return false;

}

bool handler_ngtp_get_http_delay(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)


    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get ngtp delay number in HTTP \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPHTTPDelay");

    printMessage(
        "===================================\n"
        "  max retry in HTTP  :%d \n"
        "===================================\n",num) ;// PortBuffer->data());

#endif
    return true;
}


bool handler_ngtp_set_decoder_log(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)
    error_t ret;

    printMessage(
        "==================================================\n"
        "   set NGTP decoder log , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set_int(CONFIGURATION_PATH, 2, "NGTPDecoderLog", sl::atoi(argv[0]));
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  setting NGTP decoder log option failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "   NGTP decoder log option set to %s \n"
            "===================================\n", argv[0]);

        sp<INGTPManagerService> iNGTP = interface_cast<INGTPManagerService>
                 (defaultServiceManager()->getService(
                     String16(NGTP_SRV_NAME)));
        iNGTP->readConfiguration();

        return true;
    }
#endif
    return false;

}

bool handler_ngtp_get_decoder_log(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)


    uint32_t num;

    printMessage(
        "==================================================\n"
        "  get NGTP decoder log \n"
        "==================================================\n");

    num = SL_INTERFACE_GET(ConfigurationManagerService)->get_int(CONFIGURATION_PATH, 2, "NGTPDecoderLog");


    printMessage(
        "===================================\n"
        "  NGTP decoder log  :%d \n"
        "===================================\n",num) ;// PortBuffer->data());

#endif
    return true;
}


static bool handler_send_svc_data_to_app(int32_t argc, char **argv)
{

    uint32_t serviceType = sl::atoi(argv[0]);
    uint32_t length = sl::atoi(argv[1]);
    char * buffer = argv[2];
    char hexdata[1024];

    int count = 0;
    char num[3];
    for ( int i =0 ; i < length ; i+=2)
    {
        num[0] = buffer[i];
        num[1] = buffer[i+1];
        num[2] = '\0';

        int n = strtol(num, NULL, 16);
        hexdata[count++] = (unsigned char)n;
    }

    sp<INGTPManagerService> iNGTP = interface_cast<INGTPManagerService>
             (defaultServiceManager()->getService(
                 String16(NGTP_SRV_NAME)));
    iNGTP->sendSVCDataToApp(serviceType, length/2, hexdata );
     return true;
}

static bool handler_set_tops_flag(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)


    sp<ConfigurationBuffer> config_buffer = NULL;
    bool input = sl::atoi(argv[0]);

    config_buffer = new ConfigurationBuffer();

    if(config_buffer==NULL){
        return false;
    }

    config_buffer -> setType(ConfigurationBuffer::TYPE_BOOL);
    config_buffer -> setTo(input);




    int ret;
    printMessage(
        "==================================================\n"
        "   set TOPS flag , number = %s\n"
        "==================================================\n", argv[0]);

    ret = SL_INTERFACE_GET(ConfigurationManagerService)->change_set(CONFIGURATION_PATH, 2, "TopsEnabledFlag", config_buffer);
    if(ret == E_ERROR)
    {
        printMessage(
            "===================================\n"
            "  settting TOPS flag failed  \n"
            "===================================\n");
        return false;
    }
    else {
         printMessage(
            "===================================\n"
            "    TOPS flag is set to %s \n"
            "===================================\n",config_buffer->data());

        sp<INGTPManagerService> iNGTP = interface_cast<INGTPManagerService>
                 (defaultServiceManager()->getService(
                     String16(NGTP_SRV_NAME)));
        iNGTP->readConfiguration();

        return true;
    }
#endif
    return false;

}


bool handler_get_tops_flag(int32_t argc, char **argv)
{
#if defined(_DEVICE_NAD)

sp<ConfigurationBuffer> config_buffer ;
bool want_get_val;




    printMessage(
        "==================================================\n"
        "  get TOPS flag \n"
        "==================================================\n");

    config_buffer = SL_INTERFACE_GET(ConfigurationManagerService)->get(CONFIGURATION_PATH, 2, "TopsEnabledFlag");

    printMessage(
        "===================================\n"
        "  TOPS flag  :%s \n"
        "===================================\n",  config_buffer->data());

#endif
    return true;
}
