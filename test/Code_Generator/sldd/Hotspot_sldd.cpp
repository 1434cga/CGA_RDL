
#define LOG_TAG "sldd"
#include "Log.h"

#include "daemon/sldd/Hotspot_sldd.h"
#include "daemon/sldd/sldd_common.h"
#include "daemon/sldd/man.h"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include "utils/atoi.h"
#include "services/vifManagerService/IvifManagerServiceType.h"

using namespace android;

enum Commands {
    CMD_SET_CANSIG_DATACONTRACT_TCU_Allow = 1,
    CMD_SET_CANSIG_DATACONTRACT_TCU_NOT_Allow,
    CMD_SET_CANSIG_ROAMING_TCU_ENABLE_ONCE,
    CMD_SET_CANSIG_ROAMING_TCU_ENABLED,
    CMD_SET_CANSIG_ROAMING_TCU_DISABLED,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_1,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_2,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_3,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_4,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_5,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_6,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_7,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_8,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_9,
    CMD_SET_CANSIG_CELLULARSTATE_TCU_10,
    CMD_SET_CANSIG_POPUPREQUEST_TCU_0,
    CMD_SET_CANSIG_POPUPREQUEST_TCU_1,
    CMD_SET_CANSIG_POPUPREQUEST_TCU_2,
    CMD_SET_CANSIG_POPUPREQUEST_TCU_3,

    /////Wifi Hotspot
    CMD_SET_CANSIG_HOTSPOTSTATE_TCU_OFF,
    CMD_SET_CANSIG_HOTSPOTSTATE_TCU_ON,
    CMD_SET_CANSIG_PASSWORDTYPE_TCU_DEFAULT,
    CMD_SET_CANSIG_PASSWORDTYPE_TCU_USER,
    CMD_SET_CANSIG_WIFISTATE_TCU_OFF,
    CMD_SET_CANSIG_WIFISTATE_TCU_TRANSITIONAL,
    CMD_SET_CANSIG_WIFISTATE_TCU_ON,
    CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_OFF,
    CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_ON,

    };

sldd_cmd_table_t cmd_MBC[] = {
    {"canDataContractAllow", CMD_SET_CANSIG_DATACONTRACT_TCU_Allow, commandActionMBC,
     "sldd mbc canDataContractAllow - CAN subscribed canDataContractAllow Signal\n"},
    {"canDataContractNotAllow", CMD_SET_CANSIG_DATACONTRACT_TCU_NOT_Allow, commandActionMBC,
    "sldd mbc canDataContractNotAllow - CAN subscribed canDataContractNotAllow Signal\n"},
    {"canRoamingEnableOnce", CMD_SET_CANSIG_ROAMING_TCU_ENABLE_ONCE, commandActionMBC,
    "sldd mbc canRoamingEnableOnce - CAN subscribed canRoamingEnableOnce Signal\n"},
    {"canRoamingEnabled", CMD_SET_CANSIG_ROAMING_TCU_ENABLED, commandActionMBC,
     "sldd mbc canRoamingEnabled - CAN subscribed canRoamingEnabled Signal\n"},
     {"canRoamingDisabled", CMD_SET_CANSIG_ROAMING_TCU_DISABLED, commandActionMBC,
     "sldd mbc canRoamingDisabled - CAN subscribed canRoamingDisabled Signal\n"},
     {"canCellularState1", CMD_SET_CANSIG_CELLULARSTATE_TCU_1, commandActionMBC,
     "sldd mbc canCellularState1 - CAN subscribed canCellularState1 Signal\n"},
     {"canCellularState2", CMD_SET_CANSIG_CELLULARSTATE_TCU_2, commandActionMBC,
     "sldd mbc canCellularState2 - CAN subscribed canCellularState2 Signal\n"},
     {"canCellularState3", CMD_SET_CANSIG_CELLULARSTATE_TCU_3, commandActionMBC,
     "sldd mbc canCellularState3 - CAN subscribed canCellularState3 Signal\n"},
     {"canCellularState4", CMD_SET_CANSIG_CELLULARSTATE_TCU_4, commandActionMBC,
     "sldd mbc canCellularState4 - CAN subscribed canCellularState4 Signal\n"},
     {"canCellularState5", CMD_SET_CANSIG_CELLULARSTATE_TCU_5, commandActionMBC,
     "sldd mbc canCellularState5 - CAN subscribed canCellularState5 Signal\n"},
     {"canCellularState6", CMD_SET_CANSIG_CELLULARSTATE_TCU_6, commandActionMBC,
     "sldd mbc canCellularState6 - CAN subscribed canCellularState6 Signal\n"},
     {"canCellularState7", CMD_SET_CANSIG_CELLULARSTATE_TCU_7, commandActionMBC,
     "sldd mbc canCellularState7 - CAN subscribed canCellularState7 Signal\n"},
     {"canCellularState8", CMD_SET_CANSIG_CELLULARSTATE_TCU_8, commandActionMBC,
     "sldd mbc canCellularState8 - CAN subscribed canCellularState8 Signal\n"},
     {"canCellularState9", CMD_SET_CANSIG_CELLULARSTATE_TCU_9, commandActionMBC,
     "sldd mbc canCellularState9 - CAN subscribed canCellularState9 Signal\n"},
     {"canCellularState10", CMD_SET_CANSIG_CELLULARSTATE_TCU_10, commandActionMBC,
     "sldd mbc canCellularState10 - CAN subscribed canCellularState10 Signal\n"},
     {"canPopupRequest0", CMD_SET_CANSIG_POPUPREQUEST_TCU_0, commandActionMBC,
      "sldd mbc canPopupRequest0 - CAN subscribed canPopupRequest0 Signal\n"},
     {"canPopupRequest1", CMD_SET_CANSIG_POPUPREQUEST_TCU_1, commandActionMBC,
     "sldd mbc canPopupRequest1 - CAN subscribed canPopupRequest1 Signal\n"},
     {"canPopupRequest2", CMD_SET_CANSIG_POPUPREQUEST_TCU_2, commandActionMBC,
     "sldd mbc canPopupRequest2 - CAN subscribed canPopupRequest2 Signal\n"},
     {"canPopupRequest3", CMD_SET_CANSIG_POPUPREQUEST_TCU_3, commandActionMBC,
     "sldd mbc canPopupRequest3 - CAN subscribed canPopupRequest3 Signal\n"},

    //Wifi Hotspot
    {"hotspotStateOff", CMD_SET_CANSIG_HOTSPOTSTATE_TCU_OFF, commandActionMBC,
     "sldd mbc hotspotStateOff - CAN subscribed hotspotStateOff Signal\n"},
    {"hotspotStateOn", CMD_SET_CANSIG_HOTSPOTSTATE_TCU_ON, commandActionMBC,
     "sldd mbc hotspotStateOn - CAN subscribed hotspotStateOn Signal\n"},
    {"pwdTypeDefault", CMD_SET_CANSIG_PASSWORDTYPE_TCU_DEFAULT, commandActionMBC,
     "sldd mbc pwdTypeDefault - CAN subscribed pwdTypeDefault Signal\n"},
    {"pwdTypeUser", CMD_SET_CANSIG_PASSWORDTYPE_TCU_USER, commandActionMBC,
     "sldd mbc pwdTypeUser - CAN subscribed pwdTypeUser Signal\n"},
    {"wifiStateOff", CMD_SET_CANSIG_WIFISTATE_TCU_OFF, commandActionMBC,
     "sldd mbc wifiStateOff - CAN subscribed wifiStateOff Signal\n"},
    {"wifiStateTransitional", CMD_SET_CANSIG_WIFISTATE_TCU_TRANSITIONAL, commandActionMBC,
     "sldd mbc wifiStateTransitional - CAN subscribed wifiStateTransitional Signal\n"},
    {"wifiStateOn", CMD_SET_CANSIG_WIFISTATE_TCU_ON, commandActionMBC,
     "sldd mbc wifiStateOn - CAN subscribed wifiStateOn Signal\n"},
    {"restorePwdDefaultOff", CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_OFF, commandActionMBC,
     "sldd mbc restorePwdDefaultOff - CAN subscribed restorePwdDefaultOff Signal\n"},
    {"restorePwdDefaultOn", CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_ON, commandActionMBC,
     "sldd mbc restorePwdDefaultOn - CAN subscribed restorePwdDefaultOn Signal\n"},

    {NULL, NULL, NULL, NULL}
};

sldd_cmd_table_t cmd_view_mbc_man_list[] = {
    {"canDataContractAllow", CMD_SET_CANSIG_DATACONTRACT_TCU_Allow, commandActionMBC,
     "sldd mbc canDataContractAllow - CAN subscribed canDataContractAllow Signal\n"},
     {"canDataContractNotAllow", CMD_SET_CANSIG_DATACONTRACT_TCU_NOT_Allow, commandActionMBC,
    "sldd mbc canDataContractNotAllow - CAN subscribed canDataContractNotAllow Signal\n"},
    {"canRoamingEnableOnce", CMD_SET_CANSIG_ROAMING_TCU_ENABLE_ONCE, commandActionMBC,
    "sldd mbc canRoamingEnableOnce - CAN subscribed canRoamingEnableOnce Signal\n"},
    {"canRoamingEnabled", CMD_SET_CANSIG_ROAMING_TCU_ENABLED, commandActionMBC,
     "sldd mbc canRoamingEnabled - CAN subscribed canRoamingEnabled Signal\n"},
     {"canRoamingDisabled", CMD_SET_CANSIG_ROAMING_TCU_DISABLED, commandActionMBC,
     "sldd mbc canRoamingDisabled - CAN subscribed canRoamingDisabled Signal\n"},
     {"canCellularState1", CMD_SET_CANSIG_CELLULARSTATE_TCU_1, commandActionMBC,
     "sldd mbc canCellularState1 - CAN subscribed canCellularState1 Signal\n"},
     {"canCellularState2", CMD_SET_CANSIG_CELLULARSTATE_TCU_2, commandActionMBC,
     "sldd mbc canCellularState2 - CAN subscribed canCellularState2 Signal\n"},
     {"canCellularState3", CMD_SET_CANSIG_CELLULARSTATE_TCU_3, commandActionMBC,
     "sldd mbc canCellularState3 - CAN subscribed canCellularState3 Signal\n"},
     {"canCellularState4", CMD_SET_CANSIG_CELLULARSTATE_TCU_4, commandActionMBC,
     "sldd mbc canCellularState4 - CAN subscribed canCellularState4 Signal\n"},
     {"canCellularState5", CMD_SET_CANSIG_CELLULARSTATE_TCU_5, commandActionMBC,
     "sldd mbc canCellularState5 - CAN subscribed canCellularState5 Signal\n"},
     {"canCellularState6", CMD_SET_CANSIG_CELLULARSTATE_TCU_6, commandActionMBC,
     "sldd mbc canCellularState6 - CAN subscribed canCellularState6 Signal\n"},
     {"canCellularState7", CMD_SET_CANSIG_CELLULARSTATE_TCU_7, commandActionMBC,
     "sldd mbc canCellularState7 - CAN subscribed canCellularState7 Signal\n"},
     {"canCellularState8", CMD_SET_CANSIG_CELLULARSTATE_TCU_8, commandActionMBC,
     "sldd mbc canCellularState8 - CAN subscribed canCellularState8 Signal\n"},
     {"canCellularState9", CMD_SET_CANSIG_CELLULARSTATE_TCU_9, commandActionMBC,
     "sldd mbc canCellularState9 - CAN subscribed canCellularState9 Signal\n"},
     {"canCellularState10", CMD_SET_CANSIG_CELLULARSTATE_TCU_10, commandActionMBC,
     "sldd mbc canCellularState10 - CAN subscribed canCellularState10 Signal\n"},
     {"canPopupRequest0", CMD_SET_CANSIG_POPUPREQUEST_TCU_0, commandActionMBC,
      "sldd mbc canPopupRequest0 - CAN subscribed canPopupRequest0 Signal\n"},
     {"canPopupRequest1", CMD_SET_CANSIG_POPUPREQUEST_TCU_1, commandActionMBC,
     "sldd mbc canPopupRequest1 - CAN subscribed canPopupRequest1 Signal\n"},
     {"canPopupRequest2", CMD_SET_CANSIG_POPUPREQUEST_TCU_2, commandActionMBC,
     "sldd mbc canPopupRequest2 - CAN subscribed canPopupRequest2 Signal\n"},
     {"canPopupRequest3", CMD_SET_CANSIG_POPUPREQUEST_TCU_3, commandActionMBC,
     "sldd mbc canPopupRequest3 - CAN subscribed canPopupRequest3 Signal\n"},

    //Wifi Hotspot
    {"hotspotStateOff", CMD_SET_CANSIG_HOTSPOTSTATE_TCU_OFF, commandActionMBC,
     "sldd mbc hotspotStateOff - CAN subscribed hotspotStateOff Signal\n"},
    {"hotspotStateOn", CMD_SET_CANSIG_HOTSPOTSTATE_TCU_ON, commandActionMBC,
     "sldd mbc hotspotStateOn - CAN subscribed hotspotStateOn Signal\n"},
    {"pwdTypeDefault", CMD_SET_CANSIG_PASSWORDTYPE_TCU_DEFAULT, commandActionMBC,
     "sldd mbc pwdTypeDefault - CAN subscribed pwdTypeDefault Signal\n"},
    {"pwdTypeUser", CMD_SET_CANSIG_PASSWORDTYPE_TCU_USER, commandActionMBC,
     "sldd mbc pwdTypeUser - CAN subscribed pwdTypeUser Signal\n"},
    {"wifiStateOff", CMD_SET_CANSIG_WIFISTATE_TCU_OFF, commandActionMBC,
     "sldd mbc wifiStateOff - CAN subscribed wifiStateOff Signal\n"},
    {"wifiStateTransitional", CMD_SET_CANSIG_WIFISTATE_TCU_TRANSITIONAL, commandActionMBC,
     "sldd mbc wifiStateTransitional - CAN subscribed wifiStateTransitional Signal\n"},
    {"wifiStateOn", CMD_SET_CANSIG_WIFISTATE_TCU_ON, commandActionMBC,
     "sldd mbc wifiStateOn - CAN subscribed wifiStateOn Signal\n"},
    {"restorePwdDefaultOff", CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_OFF, commandActionMBC,
     "sldd mbc restorePwdDefaultOff - CAN subscribed restorePwdDefaultOff Signal\n"},
    {"restorePwdDefaultOn", CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_ON, commandActionMBC,
     "sldd mbc restorePwdDefaultOn - CAN subscribed restorePwdDefaultOn Signal\n"},
};


//-----------------------------------------------------------------------------

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))

char* usage_MBC_id(int32_t id);
static int32_t checkArguments(int32_t argc, char* argv[]);
static int32_t getCommandId(char* string);


//-----------------------------------------------------------------------------


void register_MBC()
{
    registerCommands(MODULE_MBC, NULL, cmd_view_mbc_man_list);
}


bool commandActionMBC(int32_t argc, char **argv) {
    sp<IvifManagerService> cs = interface_cast<IvifManagerService>
                (defaultServiceManager()->getService(
                    String16(VIF_SRV_NAME)));

    if (cs == NULL) {
        printMessage("vif service is not ready, try later\n");
        return false;
    }

    int32_t result = checkArguments(argc, argv);
    if (result == -1) {
        printMessage("%s", usage_MBC_id(0));
        return true;
    } else if (result > 0) {
        printMessage("%s", usage_MBC_id(result));
        return true;
    } else {
        // normal case
    }

    int32_t id = getCommandId(argv[0]);
    if (id == -1) {
        printMessage("%s", usage_MBC_id(0));
        return true;
    }

    uint8_t tmpdata = 0x00U;
    sp<vifCANContainer> indata = new vifCANContainer();

    switch(id) {
#if defined(_DEVICE_NAD)

        case CMD_SET_CANSIG_DATACONTRACT_TCU_Allow: {
            LOGD("MBC sldd mbc canDataContractAllow");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC DATACONTRACT TCU ALLOW SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_DataContractTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_DataContractTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canDataContractAllow Finish!!");
        } break;

        case CMD_SET_CANSIG_DATACONTRACT_TCU_NOT_Allow: {
            LOGD("MBC sldd mbc canDataContractNotAllow");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC DATACONTRACT TCU NOT ALLOW SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_DataContractTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_DataContractTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canDataContractNotAllow Finish!!");
        } break;

        case CMD_SET_CANSIG_ROAMING_TCU_ENABLE_ONCE: {
            LOGD("MBC sldd mbc canRoamingEnableOnce");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC ROAMING TCU ENABLE ONCE SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_RoamingTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_RoamingTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canRoamingEnableOnce Finish!!");
        } break;

        case CMD_SET_CANSIG_ROAMING_TCU_ENABLED: {
            LOGD("MBC sldd mbc canRoamingEnabled");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC ROAMING TCU ENABLED SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_RoamingTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_RoamingTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canRoamingEnabled Finish!!");
        } break;

        case CMD_SET_CANSIG_ROAMING_TCU_DISABLED: {
            LOGD("MBC sldd mbc canRoamingDisabled");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC ROAMING TCU DISABLED SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x02U;
            indata->setData(Signal_RoamingTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_RoamingTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canRoamingDisabled Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_1: {
            LOGD("MBC sldd mbc canCellularState1");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE NO SIM SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState1 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_2: {
            LOGD("MBC sldd mbc canCellularState2");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE NO CONNECTION SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState2 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_3: {
            LOGD("MBC sldd mbc canCellularState3");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE TRANSITIONAL STATE SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x02U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState3 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_4: {
            LOGD("MBC sldd mbc canCellularState4");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE SIM RESTRICTED SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x03U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState4 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_5: {
            LOGD("MBC sldd mbc canCellularState5");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE SIM NOT SUPPORTED SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x04U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState5 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_6: {
            LOGD("MBC sldd mbc canCellularState6");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE SIM LOCKED SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x05U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState6 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_7: {
            LOGD("MBC sldd mbc canCellularState7");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE 2G SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x06U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState7 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_8: {
            LOGD("MBC sldd mbc canCellularState8");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE 3G SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x07U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState8 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_9: {
            LOGD("MBC sldd mbc canCellularState9");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE EMERGENCY SIM SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x08U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState9 Finish!!");
        } break;

        case CMD_SET_CANSIG_CELLULARSTATE_TCU_10: {
            LOGD("MBC sldd mbc canCellularState10");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC CELLULAR STATE 4G SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x09U;
            indata->setData(Signal_CellularStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_CellularStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canCellularState10 Finish!!");
        } break;

        case CMD_SET_CANSIG_POPUPREQUEST_TCU_0: {
            LOGD("MBC sldd mbc canPopupRequest0");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP REQUEST NO POPUP SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_PopupRequestTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PopupRequestTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canPopupRequest0 Finish!!");
        } break;

        case CMD_SET_CANSIG_POPUPREQUEST_TCU_1: {
            LOGD("MBC sldd mbc canPopupRequest1");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP REQUEST POPUP_1 SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_PopupRequestTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PopupRequestTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canPopupRequest1 Finish!!");
        } break;

        case CMD_SET_CANSIG_POPUPREQUEST_TCU_2: {
            LOGD("MBC sldd mbc canPopupRequest2");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP REQUEST POPUP_2 SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x02U;
            indata->setData(Signal_PopupRequestTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PopupRequestTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canPopupRequest2 Finish!!");
        } break;

        case CMD_SET_CANSIG_POPUPREQUEST_TCU_3: {
            LOGD("MBC sldd mbc canPopupRequest3");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP REQUEST POPUP_3 SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x03U;
            indata->setData(Signal_PopupRequestTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PopupRequestTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc canPopupRequest3 Finish!!");
        } break;


        ////Wifi Hotspot
        case CMD_SET_CANSIG_HOTSPOTSTATE_TCU_OFF:{
            LOGD("MBC sldd mbc hotspotStateOff");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP HOTSPOT_STATE_OFF SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_HotspotStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_HotspotStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc hotspotStateOff Finish!!");
        }break;

        case CMD_SET_CANSIG_HOTSPOTSTATE_TCU_ON:{
            LOGD("MBC sldd mbc hotspotStateOn");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP HOTSPOT_STATE_ON SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_HotspotStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_HotspotStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc hotspotStateOn Finish!!");
        }break;

        case CMD_SET_CANSIG_PASSWORDTYPE_TCU_DEFAULT:{
            LOGD("MBC sldd mbc pwdTypeDefault");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP PASSWORD_TYPE_DEFAULT SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_PasswordTypeTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PasswordTypeTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc pwdTypeDefault Finish!!");
        }break;

        case CMD_SET_CANSIG_PASSWORDTYPE_TCU_USER:{
            LOGD("MBC sldd mbc pwdTypeUser");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP PASSWORD_TYPE_USER SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_PasswordTypeTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_PasswordTypeTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc pwdTypeUser Finish!!");
        }break;

        case CMD_SET_CANSIG_WIFISTATE_TCU_OFF:{
            LOGD("MBC sldd mbc wifiStateOff");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP WIFI_STATE_OFF SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_WiFiStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_WiFiStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc wifiStateOff Finish!!");
        }break;

        case CMD_SET_CANSIG_WIFISTATE_TCU_TRANSITIONAL:{
            LOGD("MBC sldd mbc wifiStateTransitional");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP WIFI_STATE_TRANSITIONAL SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_WiFiStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_WiFiStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc wifiStateTransitional Finish!!");
        }break;

        case CMD_SET_CANSIG_WIFISTATE_TCU_ON:{
            LOGD("MBC sldd mbc wifiStateOn");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP WIFI_STATE_ON SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x02U;
            indata->setData(Signal_WiFiStateTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_WiFiStateTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc wifiStateOn Finish!!");
        }break;

        case CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_OFF:{
            LOGD("MBC sldd mbc restorePwdDefaultOff");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP RESTORE_PWD_DEFAULT_OFF SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x00U;
            indata->setData(Signal_RestorePwdDefaultTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_RestorePwdDefaultTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc restorePwdDefaultOff Finish!!");
        }break;

        case CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_ON:{
            LOGD("MBC sldd mbc restorePwdDefaultOn");
            printMessage("\n-------------------------------------------------\n");
            printMessage("    SET MBC POPUP RESTORE_PWD_DEFAULT_ON SIGNAL\n");
            printMessage("-------------------------------------------------\n");
            tmpdata = 0x01U;
            indata->setData(Signal_RestorePwdDefaultTCU_TX, GET_CAN_SIGNAL_VALUE_BYTE_SIZE(Signal_RestorePwdDefaultTCU_TX), &tmpdata);
            cs->send_dataToVif(indata->NumofSignals, indata);
            LOGD("MBC sldd mbc restorePwdDefaultOff Finish!!");
        }break;

#endif

        default:
            printMessage("Not command : %d\n", sl::atoi(argv[0]));
            break;

    }

    return true;
}

char* usage_MBC(char* cmd) {
    return usage_MBC_id(getCommandId(cmd));
}

static int32_t checkArguments(int32_t argc, char* argv[]) {
    if (argc < 1) {
        LOGD("MBC sldd checkArguments argc < 1");
        return -1;
    }

    int32_t cmdId = getCommandId(argv[0]);
    if (cmdId < 0) {
        return -1;
    }

    int32_t result = -1;
    switch(cmdId) {
#if defined(_DEVICE_NAD)

    case CMD_SET_CANSIG_DATACONTRACT_TCU_Allow:
    case CMD_SET_CANSIG_DATACONTRACT_TCU_NOT_Allow:
    case CMD_SET_CANSIG_ROAMING_TCU_ENABLE_ONCE:
    case CMD_SET_CANSIG_ROAMING_TCU_ENABLED:
    case CMD_SET_CANSIG_ROAMING_TCU_DISABLED:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_1:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_2:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_3:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_4:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_5:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_6:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_7:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_8:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_9:
    case CMD_SET_CANSIG_CELLULARSTATE_TCU_10:
    case CMD_SET_CANSIG_POPUPREQUEST_TCU_0:
    case CMD_SET_CANSIG_POPUPREQUEST_TCU_1:
    case CMD_SET_CANSIG_POPUPREQUEST_TCU_2:
    case CMD_SET_CANSIG_POPUPREQUEST_TCU_3:
    case CMD_SET_CANSIG_HOTSPOTSTATE_TCU_OFF:
    case CMD_SET_CANSIG_HOTSPOTSTATE_TCU_ON:
    case CMD_SET_CANSIG_PASSWORDTYPE_TCU_DEFAULT:
    case CMD_SET_CANSIG_PASSWORDTYPE_TCU_USER:
    case CMD_SET_CANSIG_WIFISTATE_TCU_OFF:
    case CMD_SET_CANSIG_WIFISTATE_TCU_TRANSITIONAL:
    case CMD_SET_CANSIG_WIFISTATE_TCU_ON:
    case CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_OFF:
    case CMD_SET_CANSIG_RESTOREPWDDEFAULT_TCU_ON:
        result = 0;
        break;

#endif

        default:
            break;
    }

    return result;
}

static int32_t getCommandId(char* string) {
    for (int32_t i=0; cmd_MBC[i].id != -1; i++) {
        if (!strcmp(cmd_MBC[i].name, string)) {
                LOGD("MBC sldd getCommnadID return %d", cmd_MBC[i].id);
            return cmd_MBC[i].id;
        }
    }
    LOGD("MBC sldd getCommnadID return -1");
    return -1;
}

char* usage_MBC_id(int32_t id) {
        LOGD("MBC sldd usage_MBC_id");
#if(_DEVICE_NAD)
    return "\n sldd mbc [<command>] \n"
            " <command>                             <description> \n"
            "-------------------------------------------------------------------------------------------------------------------------\n"
            " canDataContractAllow                CAN subscribed canDataContractAllow Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canDataContractNotAllow           CAN subscribed canDataContractNotAllow Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canRoamingEnableOnce             CAN subscribed canRoamingEnableOnce Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canRoamingEnabled                   CAN subscribed canRoamingEnabled Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canRoamingDisabled                  CAN subscribed canRoamingDisabled Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState1                       CAN subscribed canCellularState1 NO SIM Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState2                       CAN subscribed canCellularState2 GSM Registration Only Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState3                       CAN subscribed canCellularState3 Searching for network Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState4                       CAN subscribed canCellularState4 SIM Restricted Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState5                       CAN subscribed canCellularState5 SIM Not Supported Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState6                       CAN subscribed canCellularState6 SIM Locked Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState7                       CAN subscribed canCellularState7 2G Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState8                       CAN subscribed canCellularState8 3G Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState9                       CAN subscribed canCellularState9 Emergency SIM Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canCellularState10                     CAN subscribed canCellularState10 4G Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canPopupRequest0                    CAN subscribed canPopupRequest0 No Popup Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canPopupRequest1                    CAN subscribed canPopupRequest1 Popup 1 Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canPopupRequest2                    CAN subscribed canPopupRequest2 Popup 2 Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "canPopupRequest3                    CAN subscribed canPopupRequest3 Popup 3 Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "hotspotStateOff                     CAN subscribed hotspotStateOff Hotspot Off Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "hotspotStateOn                      CAN subscribed hotspotStateOn Hotspot On Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "pwdTypeDefault                      CAN subscribed pwdTypeDefault DefaultPassword Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "pwdTypeUser                         CAN subscribed pwdTypeUser UserDefinedPassword Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "wifiStateOff                        CAN subscribed wifiStateOff WiFi OFF Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "wifiStateTransitional               CAN subscribed wifiStateTransitional Transitional State Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "wifiStateOn                         CAN subscribed wifiStateOn WiFi ON Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "restorePwdDefaultOff                CAN subscribed restorePwdDefaultOff RestorePwdDefault OFF Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
            "restorePwdDefaultOn                 CAN subscribed restorePwdDefaultOn RestorePwdDefault ON Signal\n"
            "--------------------------------------------------------------------------------------------------------------------------\n"
    ;
#endif
}
