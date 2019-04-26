
#include <ctype.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>

#include "utils/Buffer.h"

#include <vifManagerService/IvifManagerService.h>
#include <vifManagerService/vifrxcansigval.h>

#include "man.h"
#include "sldd_common.h"
#include "cansignal.h"

#define LOG_TAG "sldd_can"

using std::atoi;

//SL_INTERFACE_DECLARE(vifManagerService);

typedef struct _sigvalandname_t{
    uint8_t sigidval;
    const char * name;
}sigvalandname_t;

enum CAN_Commands {
    CANCMD_START,               // Signal_RX_CANMsg_Start,
    //---------------------------------
    CANCMD_PowerMode,           // Signal_PowerMode_RX,
    CANCMD_RCMCrashStatus,      // Signal_CrashStatusRCM_RX,
    CANCMD_EPBEUCDStatus,       // Signal_EPBStatus_EUCD_RX,
    CANCMD_ODORollingCount,     // Signal_ODORollingCount2101_RX,

    //[2016.01.21 Seungah Jung] Add WiFi CAN Rx
    CANCMD_HotspotRequest,      // Signal_HotspotRequestHLDF_RX,
    CANCMD_PasswordRequest,     // Signal_PasswordRequestHLDF_RX,
    CANCMD_PasswordHotspot,     // Signal_PasswordHotspotHLDF_RX,
    CANCMD_RestorePwdDefault,   // Signal_RestorePwdDefaultHLDF_RX,

    //[2016.01.23 Joonho Park] Add MBC CAN Rx
    CANCMD_DataContract,                 //Signal_DataContractHLDF_RX,
    CANCMD_Roaming,                        //Signal_RoamingHLDF_RX,
    CANCMD_InfotainmentLanguage, //Signal_InfotainmentLanguage_RX,

    CANCMD_rx,
    CANCMD_tx,
    //---------------------------------
    CANCMD_END                  // Signal_RX_CANMsg_End
};

static struct CANCMD_TABLE {
    const char* name;
    //const uint8_t* name;
    uint16_t id;
    uint16_t sigid;
//    int (*_processFunPtr)(void* parg);
    void (*_processFunPtr)(uint16_t id);
} CANCmdTable[] = {
    {"powermode",           CANCMD_PowerMode,       Signal_PowerMode_RX,            CAN_PowerMode},
    {"rcmcrashstatus",      CANCMD_RCMCrashStatus,  Signal_CrashStatusRCM_RX,       CAN_RCMCrashStatus},
    {"epbeucdstatus",       CANCMD_EPBEUCDStatus,   Signal_EPBStatus_EUCD_RX,       CAN_EPBEUCDStatus},
    {"odorollingcount",     CANCMD_ODORollingCount, Signal_ODORollingCount2101_RX,  CAN_ODORollingCount},

    //[2016.01.21 Seungah Jung] Add WiFi CAN Rx
    {"hotspotrequest",      CANCMD_HotspotRequest,   Signal_HotspotRequestHLDF_RX,   CAN_HotspotRequest},
    {"passwordrequest",     CANCMD_PasswordRequest,  Signal_PasswordRequestHLDF_RX,  CAN_PasswordRequest},
    {"passwordhotspot",     CANCMD_PasswordHotspot,  Signal_PasswordHotspotHLDF_RX,  CAN_PasswordHotspot},
    {"restorepwddefault",   CANCMD_RestorePwdDefault, Signal_RestorePwdDefaultHLDF_RX, CAN_RestorePwdDefault},

    //[2016.01.23 Joonho Park] Add MBC CAN Rx
    {"datacontract",         CANCMD_DataContract,          Signal_DataContractHLDF_RX,  CAN_DataContract},
    {"roaming",              CANCMD_Roaming,               Signal_RoamingHLDF_RX,           CAN_Roaming},
    {"infotainmentlanguage", CANCMD_InfotainmentLanguage,  Signal_InfotainmentLanguage_RX,  CAN_InfotainmentLanguage},


    {"rx",                  CANCMD_rx,              Signal_END_OF_DEFINED,          CAN_getsignaldata},
//    {"tx",                  CANCMD_tx,              Signal_END_OF_DEFINED,          CAN_transmitsignaldata}
};


sldd_cmd_table_t can_cmdTable[] = {
    {"powermode",           CANCMD_PowerMode,       commandActionCAN,   "powermode          - To get PowerMode status\n"},
    {"rcmcrashstatus",      CANCMD_RCMCrashStatus,  commandActionCAN,   "rcmcrashstatus     - To get Crash Status RCM\n"},
    {"epbeucdstatus",       CANCMD_EPBEUCDStatus,   commandActionCAN,   "epbeucdstatus      - To get EPB EUCD Status\n"},
    {"odorollingcount",     CANCMD_ODORollingCount, commandActionCAN,   "odorollingcount    - To get ODORollingCount2101 signal\n"},

    //[2016.01.21 Seungah Jung] Add WiFi CAN Rx
    {"hotspotrequest",      CANCMD_HotspotRequest,   commandActionCAN,   "hotspotRequest     - To get Hotspot(WiFi) On/Off signal\n"},
    {"passwordrequest",     CANCMD_PasswordRequest,  commandActionCAN,   "passwordRequest    - To get Wifi PW Type signal\n"},
    {"passwordhotspot",     CANCMD_PasswordHotspot,  commandActionCAN,   "passwordHotspot    - To get WiFi PW Autogenerate value(0~255)\n"},
    {"restorepwddefault",   CANCMD_RestorePwdDefault, commandActionCAN,  "restorePwdDefault  - To get status of Restore WiFi PW to default\n"},

        //[2016.01.23 Joonho Park] Add MBC CAN Rx
    {"datacontract",         CANCMD_DataContract,         commandActionCAN,  "dataContract         - To get Data contract Allow/Not Allowed\n"},
    {"roaming",              CANCMD_Roaming,              commandActionCAN,  "roaming              - To get Roaming enable status\n"},
    {"infotainmentlanguage", CANCMD_InfotainmentLanguage, commandActionCAN,  "infotainmentLanguage - To get Infotainment Languege Code\n"},

    {"rx",                  CANCMD_rx,              commandActionCAN,   "RX signal data     - To get the received CAN Signal data using SignalId\n"},
//    {"tx",                  CANCMD_tx,              commandActionCAN,   "TX signal data     - To transmit the CAN Signal data using SignalId\n"},

    {NULL, NULL, NULL, NULL}
};

//-----------------------------------------------------------------------------


static bool CAN_getCANSignal(uint16_t sigid, sp<Buffer>& sigdatabuf, uint32_t GetKindOfCANData=0x00UL);

static char* usage_CAN_id(int32_t id);
static int32_t CAN_checkArguments(int32_t argc, char* argv[], int32_t& CANCMD_TABLE_idx);
static int32_t CAN_getCommandId(char* string, int32_t& CANCMD_TABLE_idx);
//static int32_t CAN_getArrIdxOfCommand(char* string);


//-----------------------------------------------------------------------------
/*
class Waiter : public RefBase
{
public:
    Waiter() : mTimeout(180), mResult(0) {}
    virtual ~Waiter() {}

    inline void setTimeout(int32_t timeout) {
        mTimeout = timeout;
    }

    inline void setResult(int32_t result) {
        mResult = result;
    }

    inline int32_t getResult() {
        return mResult;
    }

    inline void wait() {
        Mutex::Autolock autolock(mLock);
        mCondition.waitRelative(mLock, mTimeout*1000000000LL);

    }
    inline void notifyAll() {
        Mutex::Autolock autolock(mLock);
        mCondition.broadcast();
    }

private:
    Mutex mLock;
    Condition mCondition;
    int32_t mTimeout;
    int32_t mResult;
};

static sp<Waiter> sWaiter = new Waiter();
*/

void register_can() {
    registerCommands(MODULE_CAN, NULL, can_cmdTable);
}

//-----------------------------------------------------------------------------
bool CAN_getCANSignal(uint16_t sigid, sp<Buffer>& sigdatabuf, uint32_t GetKindOfCANData)
{
    bool err = false;
    uint32_t cansigreqnum = 1UL;
/*
    sp<vifCANSigRequest> cansigreq = new vifCANSigRequest(sigid);

    if (SL_INTERFACE_GET(vifManagerService)->getCANDataFromVifmgr(cansigreqnum, cansigreq, 0x00, GetKindOfCANData) == E_OK)
    {
        if (cansigreq->getCANSigDataSize() > 0 && cansigreq->getCANSigDataPtr() != NULL)
        {
            sigdatabuf->setSize(cansigreq->getCANSigDataSize());
            sigdatabuf->setTo(cansigreq->getCANSigDataPtr(),  cansigreq->getCANSigDataSize());
            err = true;
        }
        else
        {
            LOGE("SLDD : FAIL1 : getCANSignal : CAN sigdata size(%d) is 0 or sigdata ptr is NULL", cansigreq->getCANSigDataSize());
            printMessage("CAN sigdata size(%d) is 0 or sigdata ptr is NULL\n", cansigreq->getCANSigDataSize());
            err = false;
        }
    }
    else
    {
        LOGE("SLDD : FAIL2 : getCANSignal : CAN sigdata size(%d) is 0 or sigdata ptr is NULL", cansigreq->getCANSigDataSize());
        printMessage("CAN sigdata size(%d) is 0 or sigdata ptr is NULL\n", cansigreq->getCANSigDataSize());
        err = false;
    }
*/
    return err;
}

//-----------------------------------------------------------------------------

bool commandActionCAN(int32_t argc, char **argv)
{
    int32_t CANCMD_TABLE_idx = -1;
    int32_t result = CAN_checkArguments(argc, argv, CANCMD_TABLE_idx);
    if (result == -1) {
        printMessage("%s", usage_CAN_id(0));
        return true;
    } else if (result > 0) {
        printMessage("%s", usage_CAN_id(result));
        return true;
    } else {
        // normal case
    }

    if (CANCMD_TABLE_idx == -1) {
        printMessage("%s", usage_CAN_id(0));
        return true;
    }

    if (CANCmdTable[CANCMD_TABLE_idx].sigid == Signal_END_OF_DEFINED && CANCmdTable[CANCMD_TABLE_idx].id == CANCMD_rx)
    {
        int32_t sigid = atoi(argv[1]);
        if (sigid > Signal_RX_CANMsg_Start && sigid < Signal_RX_CANMsg_End) {
            CANCmdTable[CANCMD_TABLE_idx]._processFunPtr((uint16_t)sigid);
        }else{
            printMessage("CAN Signal ID(%d) is invalid\n", sigid);
            printMessage("The range : \" %d < CAN SiginalId < %d \"\n", Signal_RX_CANMsg_Start, Signal_RX_CANMsg_End);
        }
    }
    /*
    else if (CANCmdTable[CANCMD_TABLE_idx].sigid == Signal_END_OF_DEFINED && CANCmdTable[CANCMD_TABLE_idx].id == CANCMD_tx)
    {
        printMessage("Not implemented yet\n");
    }
    */
    else
        CANCmdTable[CANCMD_TABLE_idx]._processFunPtr(CANCmdTable[CANCMD_TABLE_idx].sigid);

    return true;
}

char* usage_CAN(char* cmd) {
    int32_t CANCMD_TABLE_idx = -1;
    return usage_CAN_id(CAN_getCommandId(cmd, CANCMD_TABLE_idx));
}

static int32_t CAN_checkArguments(int32_t argc, char* argv[], int32_t& CANCMD_TABLE_idx) {
    if (argc < 1) {
        return -1;
    }

    int32_t cmdId = CAN_getCommandId(argv[0], CANCMD_TABLE_idx);
    if (cmdId < 0) {
        return -1;
    }

    int32_t result = -1;

    if (cmdId > CANCMD_START && cmdId < CANCMD_END)
    {
        if (cmdId != CANCMD_rx && cmdId != CANCMD_tx && argc == 1)
            result = 0;
        else if (cmdId == CANCMD_rx && argc == 2)
            result = 0;
        else if (cmdId == CANCMD_tx && argc >= 2)
            result = 0;
        else
            result = cmdId;
    }
    else
    {
    }

    return result;
}

static int32_t CAN_getCommandId(char* string, int32_t& CANCMD_TABLE_idx) {
    int32_t length = (int32_t)(sizeof(CANCmdTable) / sizeof(struct CANCMD_TABLE));
    char * tstring = string;

    while(*tstring)
    {
        *tstring = tolower(*tstring);
        tstring++;
    }
    for (int32_t i=0; i<length; i++) {
        if (!strcmp(CANCmdTable[i].name, string)) {
            CANCMD_TABLE_idx = i;
            return CANCmdTable[i].id;
        }
    }
    return -1;
}


static char* usage_CAN_id(int32_t id) {
    if (id == CANCMD_PowerMode){
        return (char*)"sldd can powermode\n";
    }
    else if (id == CANCMD_RCMCrashStatus){
        return (char*)"sldd can rcmcrashstatus\n";
    }
    else if (id == CANCMD_EPBEUCDStatus){
        return (char*)"sldd can epbeucdstatus\n";
    }
    else if (id == CANCMD_ODORollingCount){
        return (char*)"sldd can odorollingcount\n";
    }
    //[2016.01.21 Seungah Jung] Add WiFi CAN Rx
    else if (id == CANCMD_HotspotRequest){
        return (char*)"sldd can hotspotRequest\n";
    }
    else if (id == CANCMD_PasswordRequest){
        return (char*)"sldd can passwordRequest\n";
    }
    else if (id == CANCMD_PasswordHotspot){
        return (char*)"sldd can passwordHotspot\n";
    }
    else if (id == CANCMD_RestorePwdDefault){
        return (char*)"sldd can restorePwdDefault\n";
    }

    //[2016.01.23 Joonho Park] Add MBC CAN Rx
    else if (id == CANCMD_DataContract){
        return (char*)"sldd can dataContract\n";
    }
    else if (id == CANCMD_Roaming){
        return (char*)"sldd can roaming\n";
    }
    else if (id == CANCMD_InfotainmentLanguage){
        return (char*)"sldd can infotainmentLanguage\n";
    }

    else if (id == CANCMD_rx){
        return (char*)"sldd can rx <CANSignalId> \n";// signal과 match되는 id 값이 무엇인지 디스플레이할까?
    }
    else if (id == CANCMD_tx){
        return (char*)"sldd can tx <CANSignalID> <data>\n";
    }
    else
    {
        return (char*)" sldd can [<command>]\n"
                "       <command> \n"
                "       powermode                               - To get PowerMode status \n"
                "       rcmcrashstatus                          - To get Crash Status RCM \n"
                "       epbeucdstatus                           - To get EPB EUCD Status \n"
                "       odorollingcount                         - To get ODORollingCount2101 signal \n"
                " \n--------------------------------------- WiFi Hotspot ---------------------------------------\n"
                "       hotspotRequest                          - To get Hotspot(WiFi) On/Off signal\n"
                "       passwordRequest                         - To get Wifi PW Type signal\n"
                "       passwordHotspot                         - To get WiFi PW Autogenerate value(0~255)\n"
                "       restorePwdDefault                       - To get status of Restore WiFi PW to default\n"
                " \n----------------------------------------MBC----------------------------------------------\n"
                "       dataContract                              -To get Data contract Allow/Not Allowed\n"
                "       roaming                                      -To get Roaming enable status\n"
                "       infotainmentLanguage               -To get Infotainment Languege Code\n"
                " -------------------------------------------------------------------------------------------\n"
                "       rx  <CANSignalId>                       - To get the received CAN Signal data using SignalId\n";
//                "       tx  <CANSignalId> <data : type is HEX>  - To transmit the CAN Signal data using SignalId\n";
    }
}

//------------------------------------------------------------------------------------

void CAN_PowerMode(uint16_t sigid)
{
    sigvalandname_t powermode_t[PowerMode_RX_values_e::Not_used2+1] = {
        {PowerMode_RX_values_e::KeyOut,             "KeyOut"},
        {PowerMode_RX_values_e::KeyeyRecentlyOut,   "KeyeyRecentlyOut"},
        {PowerMode_RX_values_e::KeyApproved_0,      "KeyApproved_0"},
        {PowerMode_RX_values_e::PostAccessory_0,    "PostAccessory_0"},
        {PowerMode_RX_values_e::Accessory_1,        "Accessory_1"},
        {PowerMode_RX_values_e::PostIgnition_1,     "PostIgnition_1"},
        {PowerMode_RX_values_e::IgnitionOn_2,       "IgnitionOn_2"},
        {PowerMode_RX_values_e::Running_2,          "Running_2"},
        {PowerMode_RX_values_e::Not_used1,          "Not_used1"},
        {PowerMode_RX_values_e::Crank_3,            "Crank_3"},
        {PowerMode_RX_values_e::Not_used2,          "Crank_3"}, // PowerModeMS_BO_values_e
    };

    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true)
    {
        uint8_t *d = sigdatabuf->data();

        if ( (*d >= PowerMode_RX_values_e::KeyOut && *d <= PowerMode_RX_values_e::Running_2)
                || *d == PowerMode_RX_values_e::Crank_3)
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_PowerMode \n");
            printMessage("     SUCCESS : %d:%s \n", *d, powermode_t[*d].name);
            printMessage("==============================\n");
        }
        else
        {
            printMessage("=====================================\n");
            printMessage("  getCANSignal_PowerMode \n");
            printMessage("     SUCCESS but NOT USED value : %d \n", *d);
            printMessage("=====================================\n");
        }
    }
    else
    {
        printMessage("==============================\n");
        printMessage("  getCANSignal_PowerMode \n");
        printMessage("     FAIL \n");
        printMessage("==============================\n");
    }
}

void CAN_RCMCrashStatus(uint16_t sigid)
{
    sigvalandname_t rcmcrashstatus_t[CrashStatusRCM_RX_values_e::Crash_condition+1] = {
        {CrashStatusRCM_RX_values_e::No_crash,             "No crash"},
        {CrashStatusRCM_RX_values_e::Crash_condition,      "Crash condition"},
    };

    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true)
    {
        uint8_t *d = sigdatabuf->data();

        if ( *d == CrashStatusRCM_RX_values_e::No_crash || *d == CrashStatusRCM_RX_values_e::Crash_condition)
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_RCMCrashStatus \n");
            printMessage("     SUCCESS : %d:%s \n", *d, rcmcrashstatus_t[*d].name);
            printMessage("==============================\n");
        }
        else
        {
            printMessage("======================================\n");
            printMessage("  getCANSignal_RCMCrashStatus \n");
            printMessage("     SUCCESS but Undefined value : %d \n", *d);
            printMessage("======================================\n");
        }
    }
    else
    {
        printMessage("==============================\n");
        printMessage("  getCANSignal_RCMCrashStatus \n");
        printMessage("     FAIL \n");
        printMessage("==============================\n");
    }
}

void CAN_EPBEUCDStatus(uint16_t sigid)
{
    sigvalandname_t sigvalname_t[EPBStatus_EUCD_RX_values_e::General_Faults_Or_Maintenance_mode+1] = {
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_1,             "Faulty combination"},//0
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_2,             "Faulty combination"},// 1
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_3,             "Faulty combination"},// 2
        {EPBStatus_EUCD_RX_values_e::Rear_Calliper_Closed_Rear_clamp_force_Full,             "Rear_Calliper_Closed,(Rear_clamp_force_Full)"},// 3
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_4,             "Faulty combination"},// 4

        {EPBStatus_EUCD_RX_values_e::Rear_Calliper_transition_between_OPEN_CLOSED_or_CLOSED_OPEN,             "Rear Calliper transition between OPEN-CLOSED or CLOSED-OPEN"},// 5
        {EPBStatus_EUCD_RX_values_e::RWU_By_EPB_Active_Rear_Clamp_force_is_equal_SOME,             "RWU By EPB Active, Rear Clamp_force = SOME"},// 6

        {EPBStatus_EUCD_RX_values_e::Faulty_combination_7,             "Faulty combination"},// 7
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_8,             "Faulty combination"},// 8
        {EPBStatus_EUCD_RX_values_e::Rear_Calliper_Open_Rear_Clamp_force_is_equal_NONE,             "Rear Calliper Open, Rear Clamp_force = NONE"},// 9
        {EPBStatus_EUCD_RX_values_e::EPB_Limphome_Active_Rear_Clamp_force_is_equal_Some,             "EPB Limphome Active, Rear Clamp force = Some"},// 10

        {EPBStatus_EUCD_RX_values_e::Faulty_combination_11,             "Faulty_combination "},// 11
        {EPBStatus_EUCD_RX_values_e::ECD_by_BCM_Active_Rear_Clamp_force_is_equal_Some_by_BCM, "ECD by BCM Active, Rear Clamp force = Some, by BCM"},//12

        {EPBStatus_EUCD_RX_values_e::Faulty_combination_13,             "Faulty combination"},// 13
        {EPBStatus_EUCD_RX_values_e::Faulty_combination_14,             "Faulty combination"},// 14
        {EPBStatus_EUCD_RX_values_e::General_Faults_Or_Maintenance_mode,             "General_Faults_Or_Maintenance_mode"},// 15
    };

    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true)
    {
        uint8_t *d = sigdatabuf->data();

        if ( *d >= EPBStatus_EUCD_RX_values_e::Faulty_combination_1 && *d <= EPBStatus_EUCD_RX_values_e::General_Faults_Or_Maintenance_mode)
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_EPBEUCDStatus \n");
            printMessage("     SUCCESS : %d:%s \n", *d, sigvalname_t[*d].name);
            printMessage("==============================\n");
        }
        else
        {
            printMessage("======================================\n");
            printMessage("  getCANSignal_EPBEUCDStatus \n");
            printMessage("     SUCCESS but Undefined value : %d \n", *d);
            printMessage("======================================\n");
        }
    }
    else
    {
        printMessage("==============================\n");
        printMessage("  getCANSignal_EPBEUCDStatus \n");
        printMessage("     FAIL \n");
        printMessage("==============================\n");
    }

}


void CAN_ODORollingCount(uint16_t sigid)
{
    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true)
    {
        uint8_t *d = sigdatabuf->data();
        uint8_t canvalue = 0x00U;
        float scaledval = 0x00F;

        if ( CAN_GetODORollingCount2101(d, canvalue, scaledval) != VIF_CAN_RXDATA_DEFAULT_ERROR_CONDITION_PRESENT)
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_ODORollingCount \n");
            printMessage("     SUCCESS : RawData(%d), ScaledData(%.6f)\n", canvalue, scaledval);
            printMessage("==============================\n");
        }
        else
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_RCMCrashStatus \n");
            printMessage("     SUCCESS but Default/Error condition Present : %d \n", canvalue);
            printMessage("==============================\n");
        }
    }
    else
    {
        printMessage("==============================\n");
        printMessage("  getCANSignal_RCMCrashStatus \n");
        printMessage("     FAIL \n");
        printMessage("==============================\n");
    }
}

//[2016.01.21 Seungah Jung] Add WiFi CAN Rx
void CAN_HotspotRequest(uint16_t sigid)
{
    sigvalandname_t hotspotRequest_t[HotspotRequestHLDF_BO_values_e::Hotspot_ON+1] = {
        {HotspotRequestHLDF_BO_values_e::Hotspot_OFF,     "Hotspot OFF"},
        {HotspotRequestHLDF_BO_values_e::Hotspot_ON,      "Hotspot ON"},
    };

    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true)
    {
        uint8_t *d = sigdatabuf->data();

        if ( *d == HotspotRequestHLDF_BO_values_e::Hotspot_OFF || *d == HotspotRequestHLDF_BO_values_e::Hotspot_ON )
        {
            printMessage("==============================\n");
            printMessage("  getCANSignal_HotspotRequestHLDF \n");
            printMessage("     SUCCESS : %d:%s \n", *d, hotspotRequest_t[*d].name);
            printMessage("==============================\n");
        }
        else
        {
            printMessage("======================================\n");
            printMessage("  getCANSignal_HotspotRequestHLDF \n");
            printMessage("     SUCCESS but Undefined value : %d \n", *d);
            printMessage("======================================\n");
        }
    }
    else
    {
        printMessage("==============================\n");
        printMessage("  getCANSignal_HotspotRequestHLDF \n");
        printMessage("     FAIL \n");
        printMessage("==============================\n");
    }
}

void CAN_PasswordRequest(uint16_t sigid)
{
    sigvalandname_t passwordRequest_t[PasswordRequestHLDF_BO_values_e::UserDefinedPassword+1] = {
         {PasswordRequestHLDF_BO_values_e::PasswordRequestHLDF,     "Default Password"},
         {PasswordRequestHLDF_BO_values_e::UserDefinedPassword,     "User Defined Password"},
     };

     sp<Buffer> sigdatabuf = new Buffer();

     if (CAN_getCANSignal(sigid, sigdatabuf) == true)
     {
         uint8_t *d = sigdatabuf->data();

         if ( *d == PasswordRequestHLDF_BO_values_e::PasswordRequestHLDF || *d == PasswordRequestHLDF_BO_values_e::UserDefinedPassword )
         {
             printMessage("==============================\n");
             printMessage("  getCANSignal_PasswordRequestHLDF \n");
             printMessage("     SUCCESS : %d:%s \n", *d, passwordRequest_t[*d].name);
             printMessage("==============================\n");
         }
         else
         {
             printMessage("======================================\n");
             printMessage("  getCANSignal_PasswordRequestHLDF \n");
             printMessage("     SUCCESS but Undefined value : %d \n", *d);
             printMessage("======================================\n");
         }
     }
     else
     {
         printMessage("==============================\n");
         printMessage("  getCANSignal_PasswordRequestHLDF \n");
         printMessage("     FAIL \n");
         printMessage("==============================\n");
     }
}

void CAN_RestorePwdDefault(uint16_t sigid)
{
    sigvalandname_t restorePwdDefault_t[RestorePwdDefaultHLDF_BO_values_e::Restore_to_Factory_Settings+1] = {
             {RestorePwdDefaultHLDF_BO_values_e::No_change_to_settings,     "No change to settings"},
             {RestorePwdDefaultHLDF_BO_values_e::Restore_to_Factory_Settings,  "Restore to Factory Settings"},
         };

         sp<Buffer> sigdatabuf = new Buffer();

         if (CAN_getCANSignal(sigid, sigdatabuf) == true)
         {
             uint8_t *d = sigdatabuf->data();

             if ( *d == RestorePwdDefaultHLDF_BO_values_e::No_change_to_settings || *d == RestorePwdDefaultHLDF_BO_values_e::Restore_to_Factory_Settings )
             {
                 printMessage("==============================\n");
                 printMessage("  getCANSignal_RestorePwdDefaultHLDF \n");
                 printMessage("     SUCCESS : %d:%s \n", *d, restorePwdDefault_t[*d].name);
                 printMessage("==============================\n");
             }
             else
             {
                 printMessage("======================================\n");
                 printMessage("  getCANSignal_RestorePwdDefaultHLDF \n");
                 printMessage("     SUCCESS but Undefined value : %d \n", *d);
                 printMessage("======================================\n");
             }
         }
         else
         {
             printMessage("==============================\n");
             printMessage("  getCANSignal_RestorePwdDefaultHLDF \n");
             printMessage("     FAIL \n");
             printMessage("==============================\n");
         }
}

void CAN_PasswordHotspot(uint16_t sigid)
{
     sp<Buffer> sigdatabuf = new Buffer();

     if (CAN_getCANSignal(sigid, sigdatabuf) == true)
     {
         uint8_t *d = sigdatabuf->data();

         if ( (*d >= 0) && (*d < 256))
         {
             printMessage("==============================\n");
             printMessage("  getCANSignal_PasswordHotspotHLDF \n");
             printMessage("     SUCCESS : %d \n", *d);
             printMessage("==============================\n");
         }
         else
         {
             printMessage("======================================\n");
             printMessage("  getCANSignal_PasswordHotspotHLDF \n");
             printMessage("     SUCCESS but Undefined value : %d \n", *d);
             printMessage("======================================\n");
         }
     }
     else
     {
         printMessage("==============================\n");
         printMessage("  getCANSignal_PasswordHotspotHLDF \n");
         printMessage("     FAIL \n");
         printMessage("==============================\n");
     }
}

//[2016.01.23 Joonho Park] Add MBC CAN Rx
void CAN_DataContract(uint16_t sigid)
{
    sigvalandname_t dataContract_t[DataContractHLDF_BO_values_e::Not_Allowed+1] = {
             {DataContractHLDF_BO_values_e::Allowed,     "Data contract Allowed"},
             {DataContractHLDF_BO_values_e::Not_Allowed, "Data contract Not Allowed"},
         };

         sp<Buffer> sigdatabuf = new Buffer();

         if (CAN_getCANSignal(sigid, sigdatabuf) == true)
         {
             uint8_t *d = sigdatabuf->data();

             if ( *d == DataContractHLDF_BO_values_e::Allowed || *d == DataContractHLDF_BO_values_e::Not_Allowed )
             {
                 printMessage("\n");
                 printMessage("==============================\n");
                 printMessage("  getCANSignal_DataContractHLDF \n");
                 printMessage("     SUCCESS : %d:%s \n", *d, dataContract_t[*d].name);
                 printMessage("==============================\n");
             }
             else
             {
                 printMessage("\n");
                 printMessage("======================================\n");
                 printMessage("  getCANSignal_DataContractHLDF \n");
                 printMessage("     SUCCESS but Undefined value : %d \n", *d);
                 printMessage("======================================\n");
             }
         }
         else
         {
             printMessage("\n");
             printMessage("==============================\n");
             printMessage("  getCANSignal_DataContractHLDF \n");
             printMessage("     FAIL \n");
             printMessage("==============================\n");
         }
}

void CAN_Roaming(uint16_t sigid)
{
    sigvalandname_t roaming_t[RoamingHLDF_BO_values_e::reserved+1] = {
             {RoamingHLDF_BO_values_e::Enabled,       "Roaming Enabled"},
             {RoamingHLDF_BO_values_e::Enabled_Once,  "Roaming Enable Once"},
             {RoamingHLDF_BO_values_e::Disabled,      "Roaming Disabled"},
             {RoamingHLDF_BO_values_e::reserved,      "Roaming reserved"},

         };

         sp<Buffer> sigdatabuf = new Buffer();

         if (CAN_getCANSignal(sigid, sigdatabuf) == true)
         {
             uint8_t *d = sigdatabuf->data();

             if ( *d >= RoamingHLDF_BO_values_e::Enabled && *d <= RoamingHLDF_BO_values_e::reserved )
             {
                 printMessage("\n");
                 printMessage("==============================\n");
                 printMessage("  getCANSignal_RoamingHLDF \n");
                 printMessage("     SUCCESS : %d:%s \n", *d, roaming_t[*d].name);
                 printMessage("==============================\n");
             }
             else
             {
                 printMessage("\n");
                 printMessage("======================================\n");
                 printMessage("  getCANSignal_RoamingHLDF \n");
                 printMessage("     SUCCESS but Undefined value : %d \n", *d);
                 printMessage("======================================\n");
             }
         }
         else
         {
             printMessage("\n");
             printMessage("==============================\n");
             printMessage("  getCANSignal_RoamingHLDF \n");
             printMessage("     FAIL \n");
             printMessage("==============================\n");
         }
}


void CAN_InfotainmentLanguage(uint16_t sigid)
{
    sigvalandname_t infotainmentLanguage_t[InfotainmentLanguage_BO_values_e::Chinese_Cantonese+2] = {
             {InfotainmentLanguage_BO_values_e::Undefined,                 "Undefined"},
             {InfotainmentLanguage_BO_values_e::Swedish,                   "Swedish"},
             {InfotainmentLanguage_BO_values_e::Danish,                    "Danish"},
             {InfotainmentLanguage_BO_values_e::Finnish,                   "Finnish"},
             {InfotainmentLanguage_BO_values_e::English,                   "English"},
             {InfotainmentLanguage_BO_values_e::German,                    "German"},
             {InfotainmentLanguage_BO_values_e::Dutch,                     "Dutch"},
             {InfotainmentLanguage_BO_values_e::French,                    "French"},
             {InfotainmentLanguage_BO_values_e::Italian,                   "Italian"},
             {InfotainmentLanguage_BO_values_e::Spanish,                   "Spanish"},
             {InfotainmentLanguage_BO_values_e::Portuguese,                "Portuguese"},
             {InfotainmentLanguage_BO_values_e::English_US,                "English_US"},
             {InfotainmentLanguage_BO_values_e::French_Canadian,           "French_Canadian"},
             {InfotainmentLanguage_BO_values_e::Japanese_Katakana,         "Japanese_Katakana"},
             {InfotainmentLanguage_BO_values_e::Portuguese_Brazil,         "Portuguese_Brazil"},
             {InfotainmentLanguage_BO_values_e::English_with_number_codes, "English_with_number_codes"},
             {InfotainmentLanguage_BO_values_e::Hungarian,                 "Hungarian"},
             {InfotainmentLanguage_BO_values_e::Turkish,                   "Turkish"},
             {InfotainmentLanguage_BO_values_e::Russian,                   "Russian"},
             {InfotainmentLanguage_BO_values_e::Polish,                    "Polish"},
             {InfotainmentLanguage_BO_values_e::Czech,                     "Czech"},
             {InfotainmentLanguage_BO_values_e::Norwegian,                 "Norwegian"},
             {InfotainmentLanguage_BO_values_e::Latin_Spanish,             "Latin_Spanish"},
             {InfotainmentLanguage_BO_values_e::English_Australian,        "English_Australian"},
             {InfotainmentLanguage_BO_values_e::Thai,                      "Thai"},
             {InfotainmentLanguage_BO_values_e::Ford_Asia_Pacific_English, "Ford_Asia_Pacific_English"},
             {InfotainmentLanguage_BO_values_e::Chinese_Simplified,        "Chinese_Simplified"},
             {InfotainmentLanguage_BO_values_e::Taiwanese,                 "Taiwanese"},
             {InfotainmentLanguage_BO_values_e::Chinese_Traditional,       "Chinese_Traditional"},
             {InfotainmentLanguage_BO_values_e::Flemish,                   "Flemish"},
             {InfotainmentLanguage_BO_values_e::Bulgarian,                 "Bulgarian"},
             {InfotainmentLanguage_BO_values_e::Estonian,                  "Estonian"},
             {InfotainmentLanguage_BO_values_e::Greek,                     "Greek"},
             {InfotainmentLanguage_BO_values_e::Latvian,                   "Latvian"},
             {InfotainmentLanguage_BO_values_e::Lithuanian,                "Lithuanian"},
             {InfotainmentLanguage_BO_values_e::Romanian,                  "Romanian"},
             {InfotainmentLanguage_BO_values_e::Slovakian,                 "Slovakian"},
             {InfotainmentLanguage_BO_values_e::Slovene,                   "Slovene"},
             {InfotainmentLanguage_BO_values_e::Korean,                    "Korean"},
             {InfotainmentLanguage_BO_values_e::Croatian,                  "Croatian"},
             {InfotainmentLanguage_BO_values_e::Malaysian,                 "Malaysian"},
             {InfotainmentLanguage_BO_values_e::Arabic,                    "Arabic"},
             {InfotainmentLanguage_BO_values_e::Chinese_Mandarin,          "Chinese_Mandarin"},
             {InfotainmentLanguage_BO_values_e::Chinese_Wu,                "Chinese_Wu"},
             {InfotainmentLanguage_BO_values_e::Chinese_Cantonese,         "Chinese_Cantonese"},
             {InfotainmentLanguage_BO_values_e::ErrorCode,                 "ErrorCode"},

         };

         sp<Buffer> sigdatabuf = new Buffer();

         if (CAN_getCANSignal(sigid, sigdatabuf) == true)
         {
             uint8_t *d = sigdatabuf->data();

             if ( *d >=InfotainmentLanguage_BO_values_e::Undefined && *d <= InfotainmentLanguage_BO_values_e::Chinese_Cantonese) {
                 printMessage("\n");
                 printMessage("==============================\n");
                 printMessage("  getCANSignal_InfotainmentLanguage \n");
                 printMessage("     SUCCESS : %d:%s \n", *d, infotainmentLanguage_t[*d].name);
                 printMessage("==============================\n");
             } else if (*d == InfotainmentLanguage_BO_values_e::ErrorCode) {//2016.07.16. hyeyoung.chu. fix stack overflow
                 printMessage("\n");
                 printMessage("==============================\n");
                 printMessage("  getCANSignal_InfotainmentLanguage \n");
                 printMessage("     SUCCESS : %d:ErrorCode \n", *d);
                 printMessage("==============================\n");
             } else {
                 printMessage("\n");
                 printMessage("======================================\n");
                 printMessage("  getCANSignal_InfotainmentLanguage \n");
                 printMessage("     SUCCESS but Undefined value : %d \n", *d);
                 printMessage("======================================\n");
             }
         }
         else
         {
             printMessage("\n");
             printMessage("==============================\n");
             printMessage("  getCANSignal_InfotainmentLanguage \n");
             printMessage("     FAIL \n");
             printMessage("==============================\n");
         }
}


void CAN_getsignaldata(uint16_t sigid)
{
    bool getsuccess = false;
    sp<Buffer> sigdatabuf = new Buffer();

    if (CAN_getCANSignal(sigid, sigdatabuf) == true) {
        getsuccess = true;
    }
    else
    {
        printMessage("  Try to get the last valid signal to be stored in previous IGN or initialized with initial value\n");
        if (CAN_getCANSignal(sigid, sigdatabuf, vifmgrGetKindOfCANData_e::LastValid_Data) == true) {
            getsuccess = true;
        } else {
            printMessage("  CAN signal data using SignalId\n");
            printMessage("      FAIL \n");
        }
    }

    if (getsuccess == true) {
        char buf[7];
        uint8_t* ptr = sigdatabuf->data();

        String8 dump_data;

        for(uint32_t j=0 ; j<sigdatabuf->size() ; j++){// max size is 8.
            snprintf(buf, 7UL, "[0x%02X]", *ptr++);
            dump_data.append(buf);
        }
        printMessage("  CAN signal data using SignalId\n");
        printMessage("      SUCCESS : SignalId(%d) ==> RawData : %s\n", sigid, dump_data.string());

        dump_data.clear();
    }

}

//-----------------------------------------------------------------------------
// for example,
// sldd can tx 301 01 02 03 04 05 06 07 08. ==> 301 is signal id, 01 02 03 04 05 06 07 08 is data of signal to be transmitted.
// the parameter of argv is started from 01.
void CAN_transmitsignaldata(uint16_t sigid, char **argv)// argv is signal data
{
    printMessage("  Not implemented yet\n");
}

