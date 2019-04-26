
#define LOG_TAG "sldd"
#include "Log.h"

#include "daemon/sldd/Wifi.h"
#include "daemon/sldd/sldd_common.h"
#include "daemon/sldd/man.h"

#include <stdlib.h>

#include "services/WifiManagerService/IWifiManagerService.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include "utils/atoi.h"

#if defined(_DEVICE_NAD)
#include <wlan_hal.h>
#endif
#include "services/WifiManagerService/WifiData.h"


using namespace android;

enum state {
    DISABLE = 0,
    ENABLE = 1,
};

enum Commands {
    CMD_CONNECT_STATUS = 1,
    CMD_SCAN_INTERVAL,
    CMD_SCAN_RESULT,
    CMD_CONNECT_AP,
    CMD_CONNECT_HIDDEN_AP,
    CMD_DELETE_WIFI,
    CMD_GET_SAVE_LIST,
    CMD_SET_WIFI_STATE,
    CMD_GET_WIFI_STATE,
    CMD_SET_AUTO_CONNECT,
    CMD_GET_MAC_ADDRESS,
    CMD_SET_MAC_ADDRESS,
    CMD_SEARCH_REPORT_NOTI,
    CMD_CONNECT_STATUS_NOTI,
    CMD_CONNECT_FAIL_NOTI,
    CMD_LOAD_WIFI_DRIVER,
    CMD_UNLOAD_WIFI_DRIVER,
    CMD_LOAD_WIFI_FACTORY_DRIVER,
    CMD_ENABLE_STATION,
    CMD_DISABLE_STATION,
    CMD_SET_ALL_NETWORK_STATE,
    CMD_SET_DHCPCD_STATE,
    CMD_GET_SIGNAL_STATUS,
    CMD_SEND_EVENT_RECEIVER,


    AP_CMD_DISABLE_HOTSPOT,
    AP_CMD_LOAD_WLAN_DRIVER,
    AP_CMD_LOAD_FTM_WLAN_DRIVER,
    AP_CMD_UNLOAD_WLAN_DRIVER,
    AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD,
    AP_CMD_GET_HOTSPOT_CONFIG,
    AP_CMD_GET_AP_VERSION,
    AP_CMD_SET_HOTSPOT_MAC_ADDR,
    AP_CMD_RESET_HOTSPOT_CONFIG,
    AP_CMD_ENABLE_SOFTAP,
    AP_CMD_GET_IMX_STATUS,
    AP_CMD_GET_HOTSPOT_MAC_ADDR,
    AP_CMD_SET_HOTSPOT_SSID,

    SOMEIP_SET_WIFI_SEARCH,
    SOMEIP_SET_CONNECT_TO_WIFI,
    SOMEIP_SET_CONNECT_TO_H_WIFI,
    SOMEIP_SET_DELETE_WIFI,
    SOMEIP_SET_WIFI_STATE,
    SOMEIP_GET_WIFI_STATE,
    SOMEIP_SET_AUTO_CONNECT,
    SOMEIP_GET_MAC_ADDRESS,
    SOMEIP_REVERT_CONFIG,

    FACTORY_REQUEST,
    FACTORY_RECEIVE,
    AP_FACTORY_REQUEST,
    AP_FACTORY_RECEIVE,

    //wifi client
    CMD_ENABLE_STATION_EVENT,
    CMD_DISABLE_STATION_EVENT,

};

#if defined(_DEVICE_NAD)

#define MAX_PAYLOAD_SIZE 1024
struct wpa_scan_res scanRes[MAX_NET_LIST_SIZE];
struct wlan_net_config netConf;
struct wlan_net_status netStatus;
struct wlan_signal_poll sigPoll;
struct wpa_list_net listNet[MAX_NET_LIST_SIZE];
struct mdm_mac_addresst macAddr;

#endif

sldd_cmd_table_t cmd_wifi[] = {
    {"connectStatus", CMD_CONNECT_STATUS, commandActionWifi,
     "sldd wifi connectStatus - Get connected AP informations\n"},
    {"scanResult", CMD_SCAN_RESULT, commandActionWifi,
     "sldd wifi scanResult - Scan AP and return scanned AP lists\n"},
    {"connectAp", CMD_CONNECT_AP, commandActionWifi,
     "sldd wifi connectAp <ssid> <password> <key_mgmt> <security_type> - Enter some informations to connect to the AP\n"},
    {"getSavedList", CMD_GET_SAVE_LIST, commandActionWifi,
     "sldd wifi getSavedList - Get saved lists in wpa_supplicnat.conf\n"},
    {"setWifiState", CMD_SET_WIFI_STATE, commandActionWifi,
     "sldd wifi setWifiState - Enable or disable WiFi station\n"},

    {"deleteWifi", CMD_DELETE_WIFI, commandActionWifi,
     "sldd wifi deleteWifi - Enter the Network ID that you want to remove\n"},
    {"getMacAddress", CMD_GET_MAC_ADDRESS, commandActionWifi,
     "sldd wifi getMacAddress - Display MODEM_AP MAC Address in the target board"},


    {"setMacAddress", CMD_SET_MAC_ADDRESS, commandActionWifi,
     "sldd wifi setMacAddress - Enter the MacAddress that is a 17 digit. ex)a1:b2:c3:d4:e5:f6"},
    {"connectHiddenAp", CMD_CONNECT_HIDDEN_AP, commandActionWifi,
     "sldd wifi connectAp <ssid> <password> <key_mgmt> <security_type> <hidden>- Enter some informations to connect to the AP\n"},
    {"getWifiState", CMD_GET_WIFI_STATE, commandActionWifi,
     "sldd wifi getWifiState - Display WiFi State\n"},
    {"getSignalStatus", CMD_GET_SIGNAL_STATUS, commandActionWifi,
     "sldd wifi getSignalStatus- Get Signal(RSSI, LINK_SPEED, FREQUENCY) Status\n"},
    {"sendEvent", CMD_SEND_EVENT_RECEIVER, commandActionWifi,
     "send event data to Receiver\n"},


    {"scanInterval", CMD_SCAN_INTERVAL, commandActionWifi, "test"},

    {"autoConnect", CMD_SET_AUTO_CONNECT, commandActionWifi, "test"},
    {"searchReportNoti", CMD_SEARCH_REPORT_NOTI, commandActionWifi, "test"},
    {"connectStatusNoti", CMD_CONNECT_STATUS_NOTI, commandActionWifi, "test"},
    {"connectFailNoti", CMD_CONNECT_FAIL_NOTI, commandActionWifi, "test"},
    {"loadWifiDriver", CMD_LOAD_WIFI_DRIVER, commandActionWifi, "test"},
    {"unloadWifiDriver", CMD_UNLOAD_WIFI_DRIVER, commandActionWifi, "test"},
    {"loadFactoryDriver", CMD_LOAD_WIFI_FACTORY_DRIVER, commandActionWifi, "test"},

    {"enableStation", CMD_ENABLE_STATION, commandActionWifi, "test"},
    {"disableStation", CMD_DISABLE_STATION, commandActionWifi, "test"},

    {"enableStationEvent", CMD_ENABLE_STATION_EVENT, commandActionWifi, "test"},
    {"disableStationEvent", CMD_DISABLE_STATION_EVENT, commandActionWifi, "test"},

    {"setAllNetworkState", CMD_SET_ALL_NETWORK_STATE, commandActionWifi, "test"},
    {"setDhcpcdState", CMD_SET_DHCPCD_STATE, commandActionWifi, "test"},

//--------------------------------------------------------------------------------
    {"loadWlanDriver", AP_CMD_LOAD_WLAN_DRIVER, commandActionWifi, "test"},
    {"loadFtmWlanDriver", AP_CMD_LOAD_FTM_WLAN_DRIVER, commandActionWifi, "test"},
    {"unloadWlanDriver", AP_CMD_UNLOAD_WLAN_DRIVER, commandActionWifi, "test"},
    {"disableHotspot", AP_CMD_DISABLE_HOTSPOT, commandActionWifi,
     "sldd wifi disableHotspot - Disable WiFi Hotspot\n"},
    {"setHotspotUserPW", AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD, commandActionWifi,
     "sldd wifi setHotspotUserPW <password> - Change WiFi Hotspot password\n"},
    {"getHotspotConf", AP_CMD_GET_HOTSPOT_CONFIG, commandActionWifi,
     "sldd wifi getHotspotConf - Get Hotspot Configuration status\n"},
    {"getApVersion", AP_CMD_GET_AP_VERSION, commandActionWifi,
     "sldd wifi getApVersion - Get Hotspot Configuration status\n"},
    {"setHotspotMACAddress", AP_CMD_SET_HOTSPOT_MAC_ADDR, commandActionWifi,
     "sldd wifi setHotspotMACAddress <address> - Set Hotspot WLAN MAC address\n"},
    {"resetHotspotConf", AP_CMD_RESET_HOTSPOT_CONFIG, commandActionWifi,
     "sldd wifi resetHotspotConf - Reset Hotspot Configuration(SSID, PW, Encryption Type)\n"},
    {"enableSoftAP", AP_CMD_ENABLE_SOFTAP, commandActionWifi,
     "sldd wifi enableSoftAP - Enable WiFi Hotspot by OPEN mode\n"},
    {"getImxManagerStatus", AP_CMD_GET_IMX_STATUS, commandActionWifi,
     "sldd wifi getImxManagerStatus - Get Imx(WiFi Hotspot) information (e.g. SSID, PW, ...)\n"},

    {"getHotspotMACAddress", AP_CMD_GET_HOTSPOT_MAC_ADDR, commandActionWifi,
     "sldd wifi getHotspotMACAddress - Get Hotspot MAC ADDRESS\n"},
    {"setHotspotSSID", AP_CMD_SET_HOTSPOT_SSID, commandActionWifi,
     "sldd wifi setHotspotSSID - set Hotspot SSID forcibly (until 20 chrs)\n"},


//someip-----------------------------------------------------------------------------------
    {"search", SOMEIP_SET_WIFI_SEARCH, commandActionWifi, "SOMEIP"},
    {"connect", SOMEIP_SET_CONNECT_TO_WIFI, commandActionWifi, "SOMEIP"},
    {"connectH", SOMEIP_SET_CONNECT_TO_H_WIFI, commandActionWifi, "SOMEIP"},
    {"delete", SOMEIP_SET_DELETE_WIFI, commandActionWifi, "SOMEIP"},
    {"setState", SOMEIP_SET_WIFI_STATE, commandActionWifi, "SOMEIP"},
    {"getState", SOMEIP_GET_WIFI_STATE, commandActionWifi, "SOMEIP"},
    {"setAuto", SOMEIP_SET_AUTO_CONNECT, commandActionWifi, "SOMEIP"},
    {"getMac", SOMEIP_GET_MAC_ADDRESS, commandActionWifi, "SOMEIP"},
    {"revert", SOMEIP_REVERT_CONFIG, commandActionWifi, "SOMEIP"},

//factory----------------------------------------------------------------------------------
    {"request", FACTORY_REQUEST, commandActionWifi, "FACTORY"},
    {"receive", FACTORY_RECEIVE, commandActionWifi, "FACTORY"},

    {"requestAp", AP_FACTORY_REQUEST, commandActionWifi, "AP_FACTORY"},
    {"receiveAp", AP_FACTORY_RECEIVE, commandActionWifi, "AP_FACTORY"},




    {NULL, NULL, NULL, NULL}
};

sldd_cmd_table_t cmd_view_man_list[] = {
    {"connectStatus", CMD_CONNECT_STATUS, commandActionWifi,
     "sldd wifi connectStatus - Get connected AP informations\n"},
    {"scanResult", CMD_SCAN_RESULT, commandActionWifi,
     "sldd wifi scanResult - Scan AP and return scanned AP lists\n"},
    {"connectAp", CMD_CONNECT_AP, commandActionWifi,
     "sldd wifi connectAp <ssid> <password> <key_mgmt> <security_type> - Enter some informations to connect to the AP\n"},
    {"getSavedList", CMD_GET_SAVE_LIST, commandActionWifi,
     "sldd wifi getSavedList - Get saved lists in wpa_supplicnat.conf\n"},
    {"setWifiState", CMD_SET_WIFI_STATE, commandActionWifi,
     "sldd wifi setWifiState - Enable or disable WiFi station\n"},
    {"deleteWifi", CMD_DELETE_WIFI, commandActionWifi,
     "sldd wifi deleteWifi - Enter the Network ID that you want to remove\n"},
    {"getMacAddress", CMD_GET_MAC_ADDRESS, commandActionWifi,
     "sldd wifi getMacAddress - Display MODEM_AP MAC Address in the target board"},
//2015.11.26
    {"setMacAddress", CMD_SET_MAC_ADDRESS, commandActionWifi,
     "sldd wifi setMacAddress - Enter the MacAddress that is a 12 digit"},
    {"connectHiddenAp", CMD_CONNECT_HIDDEN_AP, commandActionWifi,
     "sldd wifi connectHiddenAp <ssid> <password> <key_mgmt> <security_type> <hidden>- Enter some informations to connect to the AP\n"},
    {"getSignalStatus", CMD_GET_SIGNAL_STATUS, commandActionWifi,
     "sldd wifi getSignalStatus- Get Signal(RSSI, LINK_SPEED, FREQUENCY) Status\n"},

// hotspot
    {"enableSoftAP", AP_CMD_ENABLE_SOFTAP, commandActionWifi,
     "sldd wifi enableSoftAP - Enable WiFi Hotspot by OPEN mode\n"},
    {"disableHotspot", AP_CMD_DISABLE_HOTSPOT, commandActionWifi,
     "sldd wifi disableHotspot - Disable WiFi Hotspot\n"},
    {"setHotspotUserPW", AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD, commandActionWifi,
     "sldd wifi setHotspotUserPW <password> - change WiFi Hotspot password\n"},
    {"setHotspotMACAddress", AP_CMD_SET_HOTSPOT_MAC_ADDR, commandActionWifi,
     "sldd wifi setHotspotMACAddress <address> - Set Hotspot WLAN MAC address\n"},
    {"resetHotspotConf", AP_CMD_RESET_HOTSPOT_CONFIG, commandActionWifi,
     "sldd wifi resetHotspotConf - Reset Hotspot Configuration(SSID, PW, Encryption Type)\n"},
    {"enableSoftAP", AP_CMD_ENABLE_SOFTAP, commandActionWifi,
     "sldd wifi enableSoftAP - Enable WiFi Hotspot by OPEN mode\n"},
    {"getImxManagerStatus", AP_CMD_GET_IMX_STATUS, commandActionWifi,
     "sldd wifi getImxManagerStatus - get Imx(WiFi Hotspot) information (e.g. SSID, ...)\n"},

    {"getHotspotMACAddress", AP_CMD_GET_HOTSPOT_MAC_ADDR, commandActionWifi,
     "sldd wifi getHotspotMACAddress - Get Hotspot MAC ADDRESS\n"},
    {"setHotspotSSID", AP_CMD_SET_HOTSPOT_SSID, commandActionWifi,
     "sldd wifi setHotspotSSID - set Hotspot SSID forcibly (until 20 chrs)\n"},

};


//-----------------------------------------------------------------------------

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))

char* usage_wifi_id(int32_t id);
static int32_t checkArguments(int32_t argc, char* argv[]);
static int32_t getCommandId(char* string);


//-----------------------------------------------------------------------------


void register_wifi()
{
    registerCommands(MODULE_WIFI, NULL, cmd_view_man_list);
}


bool commandActionWifi(int32_t argc, char **argv) {
    sp<IWifiManagerService> cs = interface_cast<IWifiManagerService>
                (defaultServiceManager()->getService(
                    String16("service_layer.WifiManagerService")));

    if (cs == NULL) {
        printMessage("Wifi service is not ready, try later\n");
        return false;
    }

    int32_t result = checkArguments(argc, argv);
    if (result == -1) {
        printMessage("%s", usage_wifi_id(0));
        return true;
    } else if (result > 0) {
        printMessage("%s", usage_wifi_id(result));
        return true;
    } else {
        // normal case
    }

    int32_t id = getCommandId(argv[0]);
    if (id == -1) {
        printMessage("%s", usage_wifi_id(0));
        return true;
    }

    switch(id) {
#if defined(_DEVICE_NAD)

// some/ip interface -----------------------------------------------------------------------------------------

        case SOMEIP_SET_WIFI_SEARCH: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_SET_WIFI_SEARCH\n");
           printMessage("-------------------------------------------------\n");

           int32_t ret = cs->iGetWiFiState();
           if(ret == DISABLE) {
               printMessage("WiFi is off. you must run WiFi.\n");
               break;
           }

           if(!strcasecmp(argv[1] ,"on")) {
               cs->iSetWiFiSearchReportingState(1);
               printMessage("WiFi scan is started.\n");
           }else if(!strcasecmp(argv[1],"off")) {
               cs->iSetWiFiSearchReportingState(0);
               printMessage("WiFi scan is stopped.\n");
           }else {
               printMessage("please check command.\n");
           }

        } break;

        case SOMEIP_SET_CONNECT_TO_WIFI: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_SET_CONNECT_TO_WIFI\n");
           printMessage("-------------------------------------------------\n");
           int32_t cnt = 0;
           int32_t ret = cs->iGetWiFiState();
           if(ret == DISABLE) {
               printMessage("WiFi is off. you must run WiFi.\n");
               break;
           }

           uint64_t MacAddr = 0;
           MacAddr = strtoull(argv[1], NULL, HEXADECIMAL);
           cs->iSetConnectToWiFi(MacAddr, argv[2], sl::atoi(argv[3]), argv[4], sl::atoi(argv[5]));

           while(1) {
                memset(&netStatus, 0, sizeof(wlan_net_status));
                netStatus = cs->getWiFiConnectivityStatus();
                if(!strcmp(netStatus.wpa_state, "COMPLETED"))
                {
                    printMessage("Connected network informations\n"
                                "NET_ID           : %d\n"
                                "SSID             : %s\n"
                                "BSSID            : %s\n"
                                "KEY_MGMT         : %s\n"
                                "FREQUENCY        : %s\n"
                                "PAIRWISE_CIPHER  : %s\n"
                                "MAC_ADDRESS      : %s\n"
                                "WPA_STATE        : %s\n",netStatus.net_id,netStatus.ssid, netStatus.bssid, netStatus.key_mgmt, netStatus.freq,
                                netStatus.pairwise_cipher, netStatus.mac_address, netStatus.wpa_state);
                    break;
                }else {
                    printMessage("Connecting....\n");
                    sleep(1);
                    cnt++;
                    if(cnt>CONNECT_RETRY_CNT){
                        printMessage("failed to connect WiFi.\n");
                        break;
                    }

                }
           }


        } break;

        case SOMEIP_SET_CONNECT_TO_H_WIFI: {
            printMessage("-------------------------------------------------\n");
            printMessage("    SOMEIP_SET_CONNECT_TO_H_WIFI\n");
            printMessage("-------------------------------------------------\n");
            int32_t cnt = 0;
            int32_t ret = cs->iGetWiFiState();
            if(ret == DISABLE) {
                printMessage("WiFi is off. you must run WiFi.\n");
                break;
            }

            cs->iSetConnectToHiddenWiFi(argv[1], sl::atoi(argv[2]), argv[3], sl::atoi(argv[4]));

            while(1) {
                memset(&netStatus, 0, sizeof(wlan_net_status));
                netStatus = cs->getWiFiConnectivityStatus();
                if(!strcmp(netStatus.wpa_state, "COMPLETED"))
                {
                    printMessage("Connected network informations\n"
                                "NET_ID           : %d\n"
                                "SSID             : %s\n"
                                "BSSID            : %s\n"
                                "KEY_MGMT         : %s\n"
                                "FREQUENCY        : %s\n"
                                "PAIRWISE_CIPHER  : %s\n"
                                "MAC_ADDRESS      : %s\n"
                                "WPA_STATE        : %s\n",netStatus.net_id,netStatus.ssid, netStatus.bssid, netStatus.key_mgmt, netStatus.freq,
                                netStatus.pairwise_cipher, netStatus.mac_address, netStatus.wpa_state);
                    break;
                }else {
                    printMessage("Connecting....\n");
                    sleep(1);
                    cnt++;
                    if(cnt>CONNECT_RETRY_CNT){
                        printMessage("failed to connect WiFi.\n");
                        break;
                    }

                }
           }

        } break;

        case SOMEIP_SET_DELETE_WIFI: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_SET_DELETE_WIFI\n");
           printMessage("-------------------------------------------------\n");

           int32_t ret = cs->iGetWiFiState();
           if(ret == DISABLE) {
               printMessage("WiFi is off. you must run WiFi.\n");
               break;
           }

           int32_t cnt;
           int32_t i;
           memset(listNet, 0, sizeof(wpa_list_net)*MAX_NET_LIST_SIZE);
           cnt = cs->getSavedNetworkLists(listNet);
           if(cnt == 0) {
               printMessage("the list of WiFi is empty.\n");
           }else if(cnt == E_ERROR) {
               printMessage("WiFi is off. you must run WiFi.\n");
           }else {
               for(i=0;i<cnt;i++){
                   printMessage("ID : %3d\t SSID : %20s\t FLAGS : %10s\t \n", listNet[i].id, listNet[i].ssid, listNet[i].flags);
               }
               printMessage("please enter the ID that you want to remove.\n");
               int32_t id;
               scanf("%d",&id);
               for(i=0;i<cnt;i++){
                    if(listNet[i].id == id) {
                        cs->iSetDeleteKnownWiFi(listNet[i].ssid);
                        printMessage("removed ssid : %s\n", listNet[i].ssid);
                        break;
                    }
               }

               printMessage("successfully, list of WiFi was deleted.\n");
               /* BEGIN --
               if(ret == 0){
                   printMessage("successfully, list of WiFi was deleted.\n");
               }else {
                   printMessage("failed to delete the list of WiFi .\n");
               }
               */ //END --
           }


        } break;

        case SOMEIP_SET_WIFI_STATE: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_SET_WIFI_STATE\n");
           printMessage("-------------------------------------------------\n");

           if(!strcasecmp(argv[1] ,"on")) {
               cs->loadWiFiDriver();
               cs->iSetWiFiState(1);
           }else if(!strcasecmp(argv[1],"off")) {
               cs->iSetWiFiState(0);
               cs->unloadWiFiDriver();
           }else {
               printMessage("please check command.\n");
           }

           int32_t ret = cs->iGetWiFiState();

           if(ret == ENABLE)
           {
               printMessage("WIFI ON\n");
               if(cs->getSavedNetworkLists(listNet)){
                   memset(&netStatus, 0, sizeof(wlan_net_status));
                   netStatus = cs->getWiFiConnectivityStatus();
                   if(strlen(netStatus.ssid) != 0)
                   {
                       printMessage("Connected network informations\n"
                                   "NET_ID           : %d\n"
                                   "SSID             : %s\n"
                                   "BSSID            : %s\n"
                                   "KEY_MGMT         : %s\n"
                                   "FREQUENCY        : %s\n"
                                   "PAIRWISE_CIPHER  : %s\n"
                                   "MAC_ADDRESS      : %s\n"
                                   "WPA_STATE        : %s\n",netStatus.net_id,netStatus.ssid, netStatus.bssid, netStatus.key_mgmt, netStatus.freq,
                                   netStatus.pairwise_cipher, netStatus.mac_address, netStatus.wpa_state);
                   }
               }
           }else {
               printMessage("WIFI OFF\n");
           }

        } break;

        case SOMEIP_GET_WIFI_STATE: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_GET_WIFI_STATE\n");
           printMessage("-------------------------------------------------\n");

           int32_t ret = cs->iGetWiFiState();
           if(ret == ENABLE)
           {
               printMessage("WiFi state is ON\n");
           }else {
               printMessage("WiFi state is OFF\n");
           }
        } break;

        case SOMEIP_SET_AUTO_CONNECT: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_SET_AUTO_CONNECT\n");
           printMessage("-------------------------------------------------\n");

           int32_t ret = cs->iGetWiFiState();
           if(ret == DISABLE) {
               printMessage("WiFi is off. you must run WiFi.\n");
               break;
           }

           if(!strcasecmp(argv[1] ,"on")) {
               cs->iSetWiFiAutoConnectState(1);
               printMessage("auto connection is ON\n");
           }else if(!strcasecmp(argv[1],"off")) {
               cs->iSetWiFiAutoConnectState(0);
               printMessage("auto connection is OFF\n");
           }else {
               printMessage("please check command.\n");
           }

        } break;

        case SOMEIP_REVERT_CONFIG: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_REVERT_CONFIG\n");
           printMessage("-------------------------------------------------\n");
           int32_t ret = cs->wiFiRevertConf();
           if(ret == 0){
               printMessage("it was successfully initialized.\n");
           }else {
               printMessage("it failed to initialize\n");
           }
        } break;

        case SOMEIP_GET_MAC_ADDRESS: {
           printMessage("-------------------------------------------------\n");
           printMessage("    SOMEIP_GET_MAC_ADDRESS\n");
           printMessage("-------------------------------------------------\n");
           uint64_t mac_addr;
           mac_addr = cs->iGetMACAddress();
           uint8_t* ts = (uint8_t*)&mac_addr;
           printMessage("MAC ADDRESS : %02X:%02X:%02X:%02X:%02X:%02X\n",ts[2],ts[3],ts[4],ts[5],ts[6],ts[7]);

        } break;

        #if 0
        case CMD_SET_MAC_ADDRESS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_MAC_ADDRESS\n");
            printMessage("-------------------------------------------------\n");
            //test
            mdm_mac_addresst macAddr;
            memset(&macAddr, 0, sizeof(mdm_mac_addresst));
            if(strlen(argv[1]) == 12) {
                strcpy(macAddr.mac_address1,argv[1]);
                int32_t ret = cs->setMACAddress(&macAddr);
                if(ret >= 0){
                    printMessage("sucess\n");
                }else {
                    printMessage("fail\n");
                }
            }else {
                printMessage("please check command.\n");
            }
        } break;
        #endif

        case CMD_SET_MAC_ADDRESS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_MAC_ADDRESS\n");
            printMessage("-------------------------------------------------\n");
            //test
            char macAddr[MAX_BSSID_SIZE]={0,};
            if(strlen(argv[1]) == 17) {
                strncpy(macAddr,argv[1],MAX_BSSID_SIZE-1);
                int32_t ret = cs->setMacAddr(macAddr);
                if(ret >= 0){
                    printMessage("sucess\n");
                }else {
                    printMessage("fail\n");
                }
            }else {
                printMessage("please check command.\n");
            }
        } break;

        case CMD_LOAD_WIFI_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_LOAD_WIFI_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->loadWiFiDriver();
            if(ret == 1){
                printMessage("Normal WiFi driver was loaded successfully!!\n");
            }else if(ret == 0){
                printMessage("Normal WiFi driver failed to load!!\n");
            }else {
                printMessage("Normal WiFi driver failed to load\n");
            }
        } break;

        case CMD_UNLOAD_WIFI_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_UNLOAD_WIFI_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->unloadWiFiDriver();
            if(ret == 1){
                printMessage("WiFi driver was unloaded successfully!!\n");
            }else if(ret == 0){
                printMessage("Normal WiFi driver failed to load!!\n");
            }else {
                printMessage("Normal WiFi driver failed to load\n");
            }
        } break;

        case CMD_SEARCH_REPORT_NOTI: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SEARCH_REPORT_NOTI\n");
            printMessage("-------------------------------------------------\n");
            printMessage("temporary blocking state\n");
            //cs->wiFiSearchReportingNotification();
        } break;

        case CMD_CONNECT_STATUS_NOTI: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_CONNECT_STATUS_NOTI\n");
            printMessage("-------------------------------------------------\n");
            printMessage("temporary blocking state\n");
            //cs->wiFiConnectivityStatusNotification();
        } break;

        case CMD_CONNECT_FAIL_NOTI: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_CONNECT_FAIL_NOTI\n");
            printMessage("-------------------------------------------------\n");
            printMessage("temporary blocking state\n");
            //cs->wiFiConnectivityFailureNotification();
        } break;


        case AP_CMD_DISABLE_HOTSPOT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    DISABLE HOTSPOT\n");
            printMessage("-------------------------------------------------\n");
            cs->slddDisableHotspot();
        } break;

        case AP_CMD_LOAD_WLAN_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_LOAD_WLAN_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->slddLoadWlanDriver();
            if(ret == 1){
                printMessage("Normal hotspot driver was loaded successfully!!\n");
            }else if(ret == 0){
                printMessage("Normal hotspot driver was failed!!\n");
            }else {
                printMessage("Normal hotspot driver failed\n");
            }
        } break;

        case AP_CMD_LOAD_FTM_WLAN_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_LOAD_FTM_WLAN_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->slddLoadFtmWlanDriver();
            if(ret == 1){
                printMessage("Factory hotspot driver was loaded successfully!!\n");
            }else if(ret == 0){
                printMessage("Factory hotspot driver was failed!!\n");
            }else {
                printMessage("Factory hotspot driver failed\n");
            }

        } break;


        case AP_CMD_UNLOAD_WLAN_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_UNLOAD_WLAN_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->slddUnloadWlanDriver();
            if(ret == 1){
                printMessage("hotspot driver was unloaded successfully!!\n");
            }else if(ret == 0){
                printMessage("hotspot driver was failed!!\n");
            }else {
                printMessage("hotspot driver failed\n");
            }

        } break;

        case AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD\n");
            printMessage("-------------------------------------------------\n");
            cs->slddSetHotspotUserDefinedPasswd(argv[1]);

        } break;

        case AP_CMD_GET_AP_VERSION: {
                    printMessage("-------------------------------------------------\n");
                    printMessage("    AP_CMD_GET_AP_VERSION\n");
                    printMessage("-------------------------------------------------\n");
                    char config[MAX_CONFIG_SIZE];
                    memset(config, 0, MAX_CONFIG_SIZE);
                    cs->slddGetApVersion(config);
                    printMessage("AP VERSION : %s\n",config);

        } break;

        case AP_CMD_GET_HOTSPOT_CONFIG: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_GET_HOTSPOT_CONFIG\n");
            printMessage("-------------------------------------------------\n");
            char config[MAX_CONFIG_SIZE];
            memset(config, 0, MAX_CONFIG_SIZE);
            cs->slddGetHotspotConfig(config);
            printMessage("HOTSPOT CONFIG : %s\n",config);

        } break;


        case AP_CMD_GET_HOTSPOT_MAC_ADDR: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_GET_HOTSPOT_MAC_ADDR\n");
            printMessage("-------------------------------------------------\n");
            char config[MAX_CONFIG_SIZE];
            memset(config, 0, MAX_CONFIG_SIZE);
            cs->slddGetHotspotMacAddr(config);
            printMessage("HOTSPOT MAC ADDRESS : %s\n",config);

        } break;

        case AP_CMD_SET_HOTSPOT_MAC_ADDR:   {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_SET_HOTSPOT_MAC_ADDR\n");
            printMessage("-------------------------------------------------\n");
            if(strlen(argv[1]) == 17) {
                cs->slddSetHotspotMACAddr(argv[1]);
                printMessage("success!!\n");
            }
            else {
                printMessage("check MAC address\n");
            }
        } break;

        case AP_CMD_SET_HOTSPOT_SSID: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_SET_HOTSPOT_SSID\n");
            printMessage("-------------------------------------------------\n");
            cs->slddSetSSID(argv[1]);
        }break;

        case AP_CMD_RESET_HOTSPOT_CONFIG: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_RESET_HOTSPOT_CONFIG\n");
            printMessage("-------------------------------------------------\n");
            cs->slddResetHotspotConfig();
        } break;

        case AP_CMD_ENABLE_SOFTAP: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_ENABLE_SOFTAP\n");
            printMessage("-------------------------------------------------\n");
            cs->slddEnableSoftAP();
        } break;

        case AP_CMD_GET_IMX_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_CMD_GET_IMX_STATUS\n");
            printMessage("-------------------------------------------------\n");
            imx_status_info imxStatusInfo;

            cs->slddGetImxManagerStatus(&imxStatusInfo);
            printMessage("WLAN Driver Status : %s\n", imxStatusInfo.wlan_driver_state);
            printMessage("WiFi Hotspot Run Status : %s\n", imxStatusInfo.wifi_run_state);
            printMessage("WiFi Hotspot SSID : %s\n", imxStatusInfo.ssid);
            printMessage("WiFi Hotspot PW Type : %s\n", imxStatusInfo.pw_type);
            printMessage("WiFi Hotspot PW : %s\n", imxStatusInfo.pw);
            printMessage("WiFi Hotspot PW Seed : %s\n", imxStatusInfo.pw_seed);
            printMessage("WiFi Hotspot Encryption Mode : %s\n", imxStatusInfo.mode);
            printMessage("WiFi Hotspot Channel : %s\n", imxStatusInfo.ch);
            printMessage("WiFi Hotspot MAC Address: %s\n", imxStatusInfo.mac);
            printMessage("-------------------------------------------------\n");
        } break;

///////////////////////////////////////////////////////////////////////////////////////////////

        case CMD_CONNECT_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_CONNECT_STATUS\n");
            printMessage("-------------------------------------------------\n");
            memset(&netStatus, 0, sizeof(wlan_net_status));
            netStatus = cs->getWiFiConnectivityStatus();
            if(strlen(netStatus.ssid) != 0)
            {
                printMessage("NET_ID           : %d\n"
                            "SSID             : %s\n"
                            "BSSID            : %s\n"
                            "KEY_MGMT         : %s\n"
                            "FREQUENCY        : %s\n"
                            "PAIRWISE_CIPHER  : %s\n"
                            "MAC_ADDRESS      : %s\n"
                            "WPA_STATE        : %s\n",netStatus.net_id,netStatus.ssid, netStatus.bssid, netStatus.key_mgmt, netStatus.freq, netStatus.pairwise_cipher, netStatus.mac_address, netStatus.wpa_state);
            }else {
                printMessage("Not connected\n");
            }

        } break;

        case CMD_SCAN_INTERVAL: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SCAN_INTERVAL\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret;
            ret = cs->setScanInterval(sl::atoi(argv[1]));
            if(ret == 0){
                printMessage("Success setting scan interval \n");
            }else if(ret == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }
            else {
                printMessage("Fail setting scan interval \n");
            }
            printMessage("-------------------------------------------------\n");

        } break;

        case CMD_SCAN_RESULT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SCAN_RESULT\n");
            printMessage("-------------------------------------------------\n");
            int32_t i = 0;
            int32_t scanCnt = 0;
            memset(scanRes, 0, sizeof(wpa_scan_res)*MAX_NET_LIST_SIZE);
            scanCnt = cs->setWiFiScanAndResult(scanRes);

            if(scanCnt == E_ERROR) {
                printMessage("Check wifi state!!\n");
                break;
            }

            for(i=0;i<scanCnt;i++){
                printMessage("ID : %3d\t SSID : %20s\t BSSID : %10s\t FREQUENCY : %8s\t LEVEL : %8s\t FLAGS : %10s\n", scanRes[i].id, scanRes[i].ssid, scanRes[i].bssid, scanRes[i].freq, scanRes[i].level,scanRes[i].flags);
            }

        }break;

        case CMD_CONNECT_AP: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_CONNECT_AP\n");
            printMessage("-------------------------------------------------\n");
            printMessage(" -> ConnectAP : SSID :      %s\n", argv[1]);
            printMessage(" -> ConnectAP : PASSWORD :  %s\n", argv[2]);
            printMessage(" -> ConnectAP : WPA-PSK :   %s\n", argv[3]);
            printMessage(" -> ConnectAP : SECURITY_TYPE : %s\n", argv[4]);
            printMessage("-------------------------------------------------\n");
            int32_t ret;
            ret = cs->setConnectToWiFi(argv[1], argv[2], argv[3], sl::atoi(argv[4]));

            if(ret == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }
        } break;

        case CMD_CONNECT_HIDDEN_AP: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_CONNECT_HIDDEN_AP\n");
            printMessage("-------------------------------------------------\n");
            printMessage("    parameter : SSID, PASSWORD, WPA-PSK, TYPE 1, HIDDEN_ON 1 \n");
            printMessage("    ex) rLGE lgegnrnd00 WPA-PSK 1 1       \n" );
            printMessage("-------------------------------------------------\n");
            printMessage(" -> ConnectAP : SSID :      %s\n", argv[1]);
            printMessage(" -> ConnectAP : PASSWORD :  %s\n", argv[2]);
            printMessage(" -> ConnectAP : WPA-PSK :   %s\n", argv[3]);
            printMessage(" -> ConnectAP : SECURITY_WPA : %s\n", argv[4]);
            printMessage(" -> ConnectAP : HIDDEN ON :  %s\n", argv[5]);
            printMessage("-------------------------------------------------\n");
            int32_t ret;
            ret = cs->setConnectToHiddenWiFi(argv[1], argv[2], argv[3], atoi(argv[4]), sl::atoi(argv[5]));

            if(ret == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }

        } break;

        case CMD_DELETE_WIFI: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_DELETE_WIFI\n");
            printMessage("-------------------------------------------------\n");
            //get saved network list
            int32_t cnt;
            int32_t i;
            memset(listNet, 0, sizeof(wpa_list_net)*MAX_NET_LIST_SIZE);
            cnt = cs->getSavedNetworkLists(listNet);
            if(cnt == 0) {
                printMessage("Empty network list\n");
            }else if(cnt == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }else {
                for(i=0;i<cnt;i++){
                    printMessage("ID : %3d\t SSID : %20s\t FLAGS : %10s\t \n", listNet[i].id, listNet[i].ssid, listNet[i].flags);
                }
                printMessage("Enter the ID that you want to remove.\n");
                int32_t id;
                error_t ret;
                scanf("%d",&id);
                //ret = cs->setDeleteKnownWiFi(sl::atoi(argv[1]));
                ret = cs->setDeleteKnownWiFi(id);
                if(ret == 0){
                    printMessage("removed network list in wpa_supplicant.conf \n");
                }else {
                    printMessage("failed removing network list in wpa_supplicant.conf \n");
                }
            }

        } break;

        case CMD_GET_SAVE_LIST: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_GET_SAVED_LIST\n");
            printMessage("-------------------------------------------------\n");
            int32_t i;
            int32_t cnt = 0;
            memset(listNet, 0, sizeof(wpa_list_net)*MAX_NET_LIST_SIZE);
            cnt = cs->getSavedNetworkLists(listNet);
            if(cnt == 0) {
                printMessage("Empty network list\n");
            }else if(cnt == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }else {
                for(i=0;i<cnt;i++){
                    printMessage("ID : %3d\t SSID : %20s\t FLAGS : %10s\t \n", listNet[i].id, listNet[i].ssid, listNet[i].flags);
                }
            }

        } break;

        case CMD_SET_WIFI_STATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_WIFI_STATE\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret;
            if(!strcasecmp(argv[1] ,"on")) {
                ret = cs->setWiFiState(1);
            }else if(!strcasecmp(argv[1],"off")) {
                ret = cs->setWiFiState(0);
            }else {
                ret = -1;
            }
            if(ret == ENABLE)
            {
                if(cs->getSavedNetworkLists(listNet)){
                    memset(&netStatus, 0, sizeof(wlan_net_status));
                    netStatus = cs->getWiFiConnectivityStatus();
                    if(strlen(netStatus.ssid) != 0)
                    {
                        printMessage("Connected network informations\n"
                                    "NET_ID           : %d\n"
                                    "SSID             : %s\n"
                                    "BSSID            : %s\n"
                                    "KEY_MGMT         : %s\n"
                                    "FREQUENCY        : %s\n"
                                    "PAIRWISE_CIPHER  : %s\n"
                                    "MAC_ADDRESS      : %s\n"
                                    "WPA_STATE        : %s\n",netStatus.net_id,netStatus.ssid, netStatus.bssid, netStatus.key_mgmt, netStatus.freq, netStatus.pairwise_cipher, netStatus.mac_address, netStatus.wpa_state);
                    }else {
                        printMessage("Not connect because available network list is empty.\n");
                    }
                }else {
                    printMessage("Not connect because available network list is empty.\n");
                }
            }else if(ret == DISABLE){
                printMessage("Disconnect\n");
            }else {
                printMessage("check parameter\n");
            }

        } break;

        case CMD_GET_WIFI_STATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_GET_WIFI_STATE\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->getWiFiState();
            if(ret == ENABLE)
            {
                printMessage("WiFi Client ON\n");
            }else {
                printMessage("WiFi Client OFF\n");
            }

        } break;

        case CMD_SET_AUTO_CONNECT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_AUTO_CONNECT\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->setWiFiAutoConnectState(sl::atoi(argv[1]));
            printMessage("return value : %d\n1 : Enable\t 0: Disable\n",ret);
        } break;

        case CMD_GET_MAC_ADDRESS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_GET_MAC_ADDRESS\n");
            printMessage("-------------------------------------------------\n");
            //char mac_address[MAC_ADDRESS_SIZE];
            char mac_address[MAX_BSSID_SIZE];
            cs->getMACAddress(mac_address);
            printMessage("MODEM_AP MAC ADDRESS : %s\n",mac_address);
            //printMessage("MODEM_AP MAC ADDRESS : %02x : %02x : %02x : %02x : %02x : %02x\n",mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
        } break;

        case CMD_LOAD_WIFI_FACTORY_DRIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_LOAD_WIFI_FACTORY_DRIVER\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->loadWifiFactoryDriver();
            if(ret == 1){
                printMessage("Factory WiFi driver was loaded successfully!!\n");
            }else if(ret == 0){
                printMessage("Factory WiFi driver was failed!!\n");
            }else {
                printMessage("Factory WiFi driver failed\n");
            }
        } break;

        case CMD_ENABLE_STATION: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_ENABLE_STATION\n");
            printMessage("-------------------------------------------------\n");
            cs->enableWpaSupplicant();
        } break;

        case CMD_DISABLE_STATION: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_DISABLE_STATION\n");
            printMessage("-------------------------------------------------\n");
            cs->disableWpaSupplicant();
        } break;

        case CMD_ENABLE_STATION_EVENT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_ENABLE_STATION_EVENT\n");
            printMessage("-------------------------------------------------\n");
            printMessage("WIFI status : %d\n",cs->enableWifiWithEvent());
        } break;

        case CMD_DISABLE_STATION_EVENT: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_DISABLE_STATION_EVENT\n");
            printMessage("-------------------------------------------------\n");
            printMessage("WIFI status : %d\n",cs->disableWifiWithEvent());
        } break;


        case CMD_SET_ALL_NETWORK_STATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_ALL_NETWORK_STATE\n");
            printMessage("-------------------------------------------------\n");
            cs->setAllNetworkState(sl::atoi(argv[1]));
        } break;

        case CMD_SET_DHCPCD_STATE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SET_DHCPCD_STATE\n");
            printMessage("-------------------------------------------------\n");
            cs->setDhcpcdState(sl::atoi(argv[1]));
        } break;

        case CMD_GET_SIGNAL_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_GET_SIGNAL_STATUS\n");
            printMessage("-------------------------------------------------\n");
            wlan_signal_poll sig;
            memset(&sig, 0, sizeof(wlan_signal_poll));
            int32_t ret = cs->getSignalStatus(&sig);
            if(ret == E_ERROR) {
                printMessage("Check wifi state!!\n");
            }
            else {
                printMessage("RSSI :        %s dBm\n"
                    "LINK SPEED :   %s Mb\n"
                    "FREQUENCY :    %s Hz\n",sig.RSSI, sig.LINKSPEED, sig.FREQUENCY);
            }
        } break;

        case CMD_SEND_EVENT_RECEIVER: {
            printMessage("-------------------------------------------------\n");
            printMessage("    CMD_SEND_EVENT_RECEIVER\n");
            printMessage("-------------------------------------------------\n");
            char tmp[MAX_PAYLOAD_SIZE];
            sp<Buffer> payload = new Buffer();
            memset(tmp, 0, MAX_PAYLOAD_SIZE);
            memcpy(tmp, argv[2], strlen(argv[2])+1);
            printMessage("send event payload : %s\n", tmp);
            payload->setSize(strlen(tmp)+1);
            payload->setTo(tmp, strlen(tmp)+1);
            cs->sendEventState(sl::atoi(argv[1]), payload);
        } break;

        case FACTORY_REQUEST: {
            printMessage("-------------------------------------------------\n");
            printMessage("    FACTORY_REQUEST\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->reqFactoryPayload(argv[1]);
            //printMessage("factory return value : %d\n", ret);
            switch(ret){
                case 0: {
                    printMessage("FACTORY_RX_OFF_PASS\n");
                }break;
                case 1: {
                    printMessage("FACTORY_RX_OFF_FAIL\n");
                }break;
                case 2: {
                    printMessage("FACTORY_RX_ON_PASS\n");
                }break;
                case 3: {
                    printMessage("FACTORY_RX_ON_FAIL\n");
                }break;
                case 4: {
                    printMessage("FACTORY_TX_OFF_PASS\n");
                }break;
                case 5: {
                    printMessage("FACTORY_TX_OFF_FAIL\n");
                }break;
                case 6: {
                    printMessage("FACTORY_TX_ON_PASS\n");
                }break;
                case 7: {
                    printMessage("FACTORY_TX_ON_FAIL\n");
                }break;

                default: {
                    printMessage("ERROR\n");
                }break;
            }
        } break;

        case FACTORY_RECEIVE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    FACTORY_RECEIVE\n");
            printMessage("-------------------------------------------------\n");
            char payload[MAX_FACTORY_PAYLOAD_SIZE];
            memset(payload, 0, MAX_FACTORY_PAYLOAD_SIZE);
            cs->wiFiRxResultSend(payload);
            printMessage("factory result payload : %s\n", payload);

        } break;


        case AP_FACTORY_REQUEST: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_FACTORY_REQUEST\n");
            printMessage("-------------------------------------------------\n");
            int32_t ret = cs->reqFactoryPayloadAp(argv[1]);
            switch(ret){
                case 0: {
                    printMessage("AP_FACTORY_RX_OFF_PASS\n");
                }break;
                case 1: {
                    printMessage("AP_FACTORY_RX_OFF_FAIL\n");
                }break;
                case 2: {
                    printMessage("AP_FACTORY_RX_ON_PASS\n");
                }break;
                case 3: {
                    printMessage("AP_FACTORY_RX_ON_FAIL\n");
                }break;
                case 4: {
                    printMessage("AP_FACTORY_TX_OFF_PASS\n");
                }break;
                case 5: {
                    printMessage("AP_FACTORY_TX_OFF_FAIL\n");
                }break;
                case 6: {
                    printMessage("AP_FACTORY_TX_ON_PASS\n");
                }break;
                case 7: {
                    printMessage("AP_FACTORY_TX_ON_FAIL\n");
                }break;

                default: {
                    printMessage("ERROR\n");
                }break;
            }
        } break;

        case AP_FACTORY_RECEIVE: {
            printMessage("-------------------------------------------------\n");
            printMessage("    AP_FACTORY_RECEIVE\n");
            printMessage("-------------------------------------------------\n");
            char payload[MAX_FACTORY_PAYLOAD_SIZE];
            memset(payload, 0, MAX_FACTORY_PAYLOAD_SIZE);
            cs->wiFiRxResultSendAp(payload);
            printMessage("ap factory result payload : %s\n", payload);

        } break;

#endif

        default:
            printMessage("Not command : %d\n", sl::atoi(argv[0]));
            break;

    }

    return true;
}

char* usage_wifi(char* cmd) {
    return usage_wifi_id(getCommandId(cmd));
}

static int32_t checkArguments(int32_t argc, char* argv[]) {
    if (argc < 1) {
        return -1;
    }

    int32_t cmdId = getCommandId(argv[0]);
    if (cmdId < 0) {
        return -1;
    }

    int32_t result = -1;
    switch(cmdId) {
#if defined(_DEVICE_NAD)

        case AP_CMD_DISABLE_HOTSPOT:
        case AP_CMD_LOAD_WLAN_DRIVER:
        case AP_CMD_LOAD_FTM_WLAN_DRIVER:
        case AP_CMD_UNLOAD_WLAN_DRIVER:
        case AP_CMD_GET_HOTSPOT_CONFIG:
        case AP_CMD_GET_HOTSPOT_MAC_ADDR:
        case AP_CMD_GET_AP_VERSION:
        case AP_CMD_RESET_HOTSPOT_CONFIG:
        case AP_CMD_ENABLE_SOFTAP:
        case AP_CMD_GET_IMX_STATUS:
        case CMD_SCAN_RESULT:
        case CMD_CONNECT_STATUS:
        case CMD_DELETE_WIFI:
        case CMD_GET_WIFI_STATE:
        case CMD_GET_MAC_ADDRESS:
        case CMD_SEARCH_REPORT_NOTI:
        case CMD_CONNECT_STATUS_NOTI:
        case CMD_CONNECT_FAIL_NOTI:
        case CMD_LOAD_WIFI_DRIVER:
        case CMD_LOAD_WIFI_FACTORY_DRIVER:
        case CMD_UNLOAD_WIFI_DRIVER:
        case CMD_ENABLE_STATION:
        case CMD_DISABLE_STATION:
        case CMD_ENABLE_STATION_EVENT:
        case CMD_DISABLE_STATION_EVENT:
        case CMD_GET_SAVE_LIST:
        case CMD_GET_SIGNAL_STATUS:
        /* BEGIN --
        case APP_HOTSPOT_STATE_DISABLE:
        case APP_HOTSPOT_STATE_ENABLE:
        case APP_PASSWORD_TYPE_DEFAULT:
        case APP_PASSWORD_TYPE_USER:
        case APP_WIFI_STATE_DISABLE:
        case APP_WIFI_STATE_TRANSITIONAL:
        case APP_WIFI_STATE_ENABLE:
        case APP_RESTORE_PWD_DEFAULT:
        */ //END --
        case SOMEIP_GET_WIFI_STATE: //someip
        case SOMEIP_SET_DELETE_WIFI://someip
        case SOMEIP_GET_MAC_ADDRESS: //someip
        case SOMEIP_REVERT_CONFIG: //someip
        case FACTORY_RECEIVE:
        case AP_FACTORY_RECEIVE:
            if(argc == 1) {
               result = 0;
            }break;

        case CMD_SCAN_INTERVAL:
        case CMD_SET_AUTO_CONNECT:
        case CMD_SET_WIFI_STATE:
        case CMD_SET_ALL_NETWORK_STATE:
        case CMD_SET_DHCPCD_STATE:
        case AP_CMD_SET_HOTSPOT_USER_DEFINED_PASSWD:
        case AP_CMD_SET_HOTSPOT_MAC_ADDR:
        case AP_CMD_SET_HOTSPOT_SSID:
        case CMD_SET_MAC_ADDRESS:
        case SOMEIP_SET_WIFI_SEARCH://someip
        case SOMEIP_SET_WIFI_STATE://someip
        case SOMEIP_SET_AUTO_CONNECT://someip
        case FACTORY_REQUEST:
        case AP_FACTORY_REQUEST:
            if(argc == 2){
                result = 0;
            }break;

        case CMD_SEND_EVENT_RECEIVER:
            if(argc == 3){
                result = 0;
            }break;

        case CMD_CONNECT_AP:
        case SOMEIP_SET_CONNECT_TO_H_WIFI: //someip
            if(argc == 5){
                result = 0;
            }break;

        case CMD_CONNECT_HIDDEN_AP:
        case SOMEIP_SET_CONNECT_TO_WIFI: //someip
            if(argc == 6){
                result = 0;
            }break;
#endif

        default:
            break;
    }

    return result;
}

static int32_t getCommandId(char* string) {
    for (int32_t i=0; cmd_wifi[i].id != -1; i++) {
        if (!strcmp(cmd_wifi[i].name, string)) {
            return cmd_wifi[i].id;
        }
    }
    return -1;
}

char* usage_wifi_id(int32_t id) {
#if(_DEVICE_NAD)
    if(checkMDMVariant() == HIGH_VARIANT) {
        return " sldd wifi [<command>] [parameters] \n"
                " <command>              <parameter>                             <description> \n"
                "-------------------------------------------------------------------------------------------------------------------------\n"
                " setState                on, off                                  on : enable WIFI Station(include function for load wifi driver) \n"
                "                                                                  off : disable WIFI Station(include function for unload wifi driver)\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " getState                NONE                                     display WiFi state\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " setAuto                 on, off                                  on : set auto connection is ON \n"
                "                                                                  off : set auto connection is OFF \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " connect                BSSID, SSID, SECURE_TYPE, PASSWORD,       enter some informations to connect to the AP \n"
                "                        KNOWN_NETWORK                             BSSID : MAC ADDRESS for AP\n"
                "                                                                  SSID : name of AP \n"
                "                                                                  SECURITY_NONE : 0, SECURITY_WEP : 1, SECURITY_WPA : 2 SECURITY_WPA2 : 3 \n"
                "                                                                  PASSWORD = password of AP if OPEN, PASSWORD : 0\n"
                "                                                                  KNOWN_NETWORK   : 0 \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " connectH                SSID, SECURE_TYPE, PASSWORD,             enter some informations to connect to the AP. \n"
                "                         KNOWN_NETWORK                            SSID : name of AP \n"
                "                                                                  SECURITY_NONE : 0, SECURITY_WEP : 1, SECURITY_WPA : 2 SECURITY_WPA2 : 3 \n"
                "                                                                  PASSWORD = password of AP if OPEN, PASSWORD : 0\n"
                "                                                                  KNOWN_NETWORK   : 0 \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " delete                  NONE                                     enter the Network ID that you want to remove\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " search                  on, off                                  on : start searching ap \n"
                "                                                                  off : stop searching ap \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " getMac                  NONE                                     display MODEM_AP MAC address\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " revert                  NONE                                     configuration is initialized\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"

    ;

    }else {
        return " sldd wifi [<command>] [parameters] \n"
                " <command>              <parameter>                             <description> \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " enableSoftAP            NONE                                   - Enable WIFI Hotspot\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " disableHotspot          NONE                                   - Disable WIFI Hotspot\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " setHotspotUserPW        PASSWORD                               - Set WIFI Hotspot user defined password (up to 15 chrs) \n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " setHotspotMACAddress    ADDRESS                                - Set Hotspot WLAN MAC address(17 chrs). ex)a1:b2:c3:d4:e5:f6\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " resetHotspotConf        NONE                                   - Reset Hotspot Configuration(SSID, PW, Encryption Type)\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " getImxManagerStatus     NONE                                   - Get Imx(WiFi Hotspot) information (e.g. SSID, PW, ...)\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
                " setHotspotSSID          ID                                     - set Hotspot SSID forcibly (until 20 chrs)\n"
                "--------------------------------------------------------------------------------------------------------------------------\n"
    ;
    }

#endif
    return NULL;
}
