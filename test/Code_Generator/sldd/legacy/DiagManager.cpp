/**
* \file    DiagManager.cpp
* \brief     Implementation of DiagManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.10.23 UK time
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#define LOG_TAG "DiagManagerService"
#include "Log.h"

#include <stdlib.h>
#include "daemon/sldd/sldd_common.h"
#include "utils/atoi.h"
#include "daemon/sldd/telephony.h"
#include "daemon/sldd/man.h"

#include "daemon/sldd/DiagManager.h"
#include "TCUProtocol.h"

#include "services/DiagManagerService/DiagCommand.h"
#include "services/DiagManagerService/DiagManagerService.h"
#include "services/DiagManagerService/IDiagManagerService.h"
#include "services/SystemManagerService/ISystemManagerService.h"
#include "services/SystemManagerService/ISystemManagerServiceType.h"
#include "services/TelephonyManagerService/base/ServiceState.h"
#include "services/TelephonyManagerService/base/TelephonyManager.h"
#include "services/TelephonyManagerService/base/INetworkQueryService.h"
#include "services/TelephonyManagerService/base/INetworkQueryServiceCallback.h"
#include "services/TelephonyManagerService/base/RILConstants.h"
#include "services/TelephonyManagerService/opt/Call.h"
#include "services/TelephonyManagerService/opt/uicc/IccUtils.h"
#include "services/TelephonyManagerService/utils/ByteArray.h"
#include "services/TelephonyManagerService/utils/Util.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>

#include "daemon/sldd/man.h"

//Remove DebugManagerService by daehun.ju@lge.com #include "jlr/jlr_tf1_cmds_handler.h"

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))
#define printValues(...) (fscanf(stderr, __VA_ARGS__))

using namespace android;

enum diag_commands {
    DIAG_CMD_DEBUG_DTC_STATUS = 1,
    DIAG_CMD_DEBUG_PARTNUMBER,
    DIAG_CMD_DEBUG_SERVICE_STATUS,
};

static bool handler_diag_dtcstatus(int32_t argc, char **argv);
static bool handler_diag_dtcClear(int32_t argc, char **argv);
static bool dump_dtc_snapshot(int32_t argc, char **argv);
static uint8_t dtc_CaculateSnapshotRecordNumber(uint8_t a, uint8_t b);
static bool handler_diag_tcusvcstatus(int32_t argc, char **argv);
static bool handler_diag_trackingstatus(int32_t argc, char **argv);
static bool handler_diag_gnssparam(int32_t argc, char **argv);
static bool handler_diag_netswversion(int32_t argc, char **argv);
static bool handler_diag_iccnumber(int32_t argc, char **argv);
static bool handler_diag_gprsstatus(int32_t argc, char **argv);
static bool handler_diag_regstatus(int32_t argc, char **argv);
static bool handler_diag_resetdtc(int32_t argc, char **argv);
static bool handler_diag_testodst(int32_t argc, char **argv);
static bool handler_diag_testbub(int32_t argc, char **argv);
static bool handler_diag_testractorytest(int32_t argc, char **argv);
static bool handler_diag_testoperationmode(int32_t argc, char **argv);
static bool handler_diag_testwriteseckey(int32_t argc, char **argv);
bool diag_TUstatus(int32_t argc, char **argv);
static bool diag_bubstatus(int32_t argc, char **argv);
static bool diag_speakerstatus(int32_t argc, char **argv);
static bool diag_micstatus(int32_t argc, char **argv);
static bool diag_extbattstatus(int32_t argc, char **argv);

/**
*    Declare binder interface
*
*/
SL_INTERFACE_DECLARE(DiagManagerService);
SL_INTERFACE_DECLARE2(SystemManagerService, SYSTEM_SRV_NAME);

class DiagWaiter : public RefBase
{
public:
    DiagWaiter() : mTimeout(180), mResult(0) {}
    virtual ~DiagWaiter() {}

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

static sp<DiagWaiter> sDiagWaiter = new DiagWaiter();

uint8_t snapshotDataBuffer[SNAPSHOT_DATA_MAX_SIZE];

sldd_cmd_table_t diag_cmdTable[] = {
    {"dtcstatus", DIAG_CMD_DEBUG_DTC_STATUS, handler_diag_dtcstatus,
     "  sldd diag dtc [command]  - display dtc status such as status, adc and so on. \n"
     "     [command] \n"
     "     all<or blank>   - diplay all of DTC status, value and DTC result \n"
     "     result          - diplay DTC result only \n"
     "     <dtc name>      - antena \n" },
    {"dtcclear", DIAG_CMD_DEBUG_DTC_STATUS, handler_diag_dtcClear,
     "     DTC clear command. \n"
     "     By command Read DTC info you can check result. \n"},
    {"dtcsnapshot", DIAG_CMD_DEBUG_PARTNUMBER, dump_dtc_snapshot, NOT_IMPLEMENTED},

    /*Debug functions*/
    {"tustatus", DIAG_CMD_DEBUG_PARTNUMBER, diag_TUstatus, NOT_IMPLEMENTED},
    {"bubstatus", DIAG_CMD_DEBUG_PARTNUMBER, diag_bubstatus, NOT_IMPLEMENTED},
    {"speakerstatus", DIAG_CMD_DEBUG_PARTNUMBER, diag_speakerstatus, NOT_IMPLEMENTED},
    {"micstatus", DIAG_CMD_DEBUG_PARTNUMBER, diag_micstatus, NOT_IMPLEMENTED},
    {"extbattstatus", DIAG_CMD_DEBUG_PARTNUMBER, diag_extbattstatus, NOT_IMPLEMENTED},
    {"TCUsvcStatus", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_tcusvcstatus, NOT_IMPLEMENTED},
    {"trackingStatus", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_trackingstatus, NOT_IMPLEMENTED},
    {"GNSSparam", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_gnssparam, NOT_IMPLEMENTED},
    {"netSWversion", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_netswversion, NOT_IMPLEMENTED},
    {"ICCnumber", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_iccnumber, NOT_IMPLEMENTED},
    {"GPRSStatus", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_gprsstatus, NOT_IMPLEMENTED},
    {"RegStatus", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_regstatus, NOT_IMPLEMENTED},
    {"resetDTC", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_resetdtc, NOT_IMPLEMENTED},
    {"testBUB", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_testbub, NOT_IMPLEMENTED},
    {"testFactoryTest", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_testractorytest, NOT_IMPLEMENTED},
    {"testOperationalMode", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_testoperationmode, NOT_IMPLEMENTED},
    {"writeSecKey", DIAG_CMD_DEBUG_PARTNUMBER, handler_diag_testwriteseckey, NOT_IMPLEMENTED},
    {NULL, NULL, NULL, NULL}
};

bool commandActionDiag(int32_t argc, char **argv)
{
    if (argc == 0) {
        printMessage("%s", usage_diag(NULL));
        return true;
    }
    DO_HANDLER(diag_cmdTable, argv[0], argc-1, argv+1);
}

char* usage_diag(char *cmd)
{
    printMessage(" sldd diag [<command>]           - perform diag operation for TCU3 \n");
    printMessage("    <command> \n");
    PRINT_USAGE(diag_cmdTable);

    return NULL;
}

const char* dtc_status_str[] =
{ "Normal",
  "Short VBATT",
  "Short GND",
  "Open",
};

uint16_t dtc_snapshot_id[DTC_NUMBER_OF_SNAPSHOT_ID] =
{
    0xDD00,//SNAPSHOT_DID_GLOBAL_REAL_TIME  =
    0xDD01,//SNAPSHOT_DID_TOTAL_DISTANCE    =
    0xDD02,//SNAPSHOT_DID_MAIN_VBATT_VOL    =
    0xDD03,//SNAPSHOT_DID_ENGINE_STATUS     =
    0xDD04,//SNAPSHOT_DID_IN_CAR_TEMP       =
    0xDD05,//SNAPSHOT_DID_OUTDOOR_TEMP      =
    0xDD06,//SNAPSHOT_DID_POWER_MODE        =
           //SNAPSHOT_DID_POWER_MODE_QF     = 0xDD07,
    0xDD08,//SNAPSHOT_DID_ENGINE_RPM        =
    0xDD09,//SNAPSHOT_DID_VEHICLE_SPEED     =
    0xDD0A,//SNAPSHOT_DID_GPS_TIME          =
    0xD10E,//SNAPSHOT_DID_HSCAN_STATUS      =
    0xD702,//SNAPSHOT_DID_MSCAN_STATUS      =
    //0xD98A,//SNAPSHOT_DID_TCU_TEMP          =
    0xD110//SNAPSHOT_DID_MAIN_BATTERY       =
};

/*Sync with DTC_FILE_PATHS*/
const char DTC_FILE_PATHS[] = "/userdata/config/DTC_Backup.txt";
DTC_Backup_File_Format Diag_DTC_Backup;

error_t DTC_LoadToFile_sldd()
{
    LOGV("DTC_LoadToFile");

    FILE* f = fopen(DTC_FILE_PATHS, "r+");
    if(f == NULL) {
        LOGE("============ DTCLoadToFile : File is not exist. ============");
        return E_ERROR;
    } else {
        LOGI("============ Load To File start ============");

        uint16_t size = sizeof(DTC_Backup_File_Format);
        char dtcData[DIAG_DTC_BUFFER_MAX] = { 0, };
        fread((char*)dtcData, 1, size, f);
        fclose(f);

        /*while( EOF != (buffer = fgetc( f))) {
            if(count<size){
                dtcData[count]=buffer;
            }else{
                break;
            }
            count++;
        }*/

        memcpy((char*) (&Diag_DTC_Backup), (char*) dtcData, size);
        for (int32_t i = 0; i < (DTC_TOTAL_DTC_SIZE); i++) {
            Diag_DTC_Backup.DTCData[i].index = (uint8_t) i;
            Diag_DTC_Backup.DTCData[i].DID = DTC_ID[i]; // save to JLR P2 event type DID.
        }
        LOGI("============ Load To File end   ============");
    }
    return E_OK;
}

error_t DTC_SaveAllData_sldd()
{
    FILE* f = fopen(DTC_FILE_PATHS, "w+");

    if (f == NULL) {
        LOGE("Can not make state config file");
        return E_ERROR;
    } else {
        uint16_t size = sizeof(DTC_Backup_File_Format);
        LOGV("DTC backup buffer size is [%d]", size);
        char* save_ptr = new char[DIAG_DTC_BUFFER_MAX];
        memcpy((void*) save_ptr, (void*) (&Diag_DTC_Backup), size);
        //save integer date
        fwrite(save_ptr, 1, size, f);
        /*        for(uint16_t i = 0; i < size ; i++)
         {
         fprintf(f,"%d",&save_ptr[i]);
         }*/
        delete[] save_ptr;
    }
    fclose(f);
    LOGV("DTC_SaveAllData complete.");

    return E_OK;
}

error_t DTC_RemoveFile()
{
    int nResult = remove(DTC_FILE_PATHS);

    if (nResult == 0x00) {
        LOGV("DTC backup file removed.");
    } else {
        LOGE("DTC backup file removing failed.");
    }
}

static void get_snapshotData(uint8_t h, uint8_t i, uint8_t j, uint8_t k, uint16_t did, sp<Buffer> result)
{
    result->clear();
    if (did == DIAG_SNAPSHOT_DID_GLOBAL_REAL_TIME) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].GlobalRealTime, DID_DD00_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_TOTAL_DISTANCE) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].TotalDistance, DID_DD01_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_MAIN_VBATT_VOL) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].MainECUSupplyVoltage, DID_DD02_SIZE);
//    } else if (did == DIAG_SNAPSHOT_DID_ENGINE_STATUS) {
//        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].EngineStatus, DID_DD03_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_IN_CAR_TEMP) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].IncarTemp, DID_DD04_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_OUTDOOR_TEMP) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].OutDoorTemp, DID_DD05_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_POWER_MODE) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].PowerMode, DID_DD06_SIZE);
//    } else if (did == DIAG_SNAPSHOT_DID_ENGINE_RPM) {
//        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].EngineRPM, DID_DD08_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_VEHICLE_SPEED) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].VehicleSpeed, DID_DD09_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_GPS_TIME) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].GPS_Time, DID_DD0A_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_HSCAN_STATUS) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].HSCAN_Network_Management_State, DID_D10E_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_MSCAN_STATUS) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].MSCAN_Network_Management_State, DID_D702_SIZE);
    } else if (did == DIAG_SNAPSHOT_DID_MAIN_BATTERY) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].TCU_Main_Battery_Vol, DID_D110_SIZE);
    } else {

    }
//    printMessage("h[%d],i[%d],j[%d],k[%d,did[%4x],result[%s]\n",h,i,j,k,did,result->data());

}
bool diag_TUstatus(int32_t argc, char **argv)
{
    int32_t timeout=1;

    sp<TUStatusInfo> Info = new TUStatusInfo;

    sp<IDiagManagerService> gDiagInterface = interface_cast<IDiagManagerService>
            (defaultServiceManager()->getService(String16("service_layer.DiagManagerService")));

    gDiagInterface->SetUpTUStatusData(0x0D);

    sDiagWaiter->setTimeout(timeout);

    sDiagWaiter->wait();
    if (sDiagWaiter->getResult() == 0) {
        printMessage(" Scan complete...\n");
    }
    gDiagInterface->GetTUStatusData(Info);

    printMessage("Power[%d].\n",Info->TCUStatus.Power);
    printMessage("speaker[%d].\n",Info->TCUStatus.speaker);
    printMessage("mic[%d].\n",Info->TCUStatus.mic);
    printMessage("gsmExtAntenna[%d].\n",Info->TCUStatus.gsmExtAntenna);
    printMessage("gnssAntenna[%d].\n",Info->TCUStatus.gnssAntenna);
    printMessage("extPower[%d].\n",Info->TCUStatus.extPower);
    printMessage("intPower[%d].\n",Info->TCUStatus.intPower);
    printMessage("CAN[%d].\n",Info->TCUStatus.can);
    printMessage("button[%d].\n",Info->TCUStatus.button);
    printMessage("crashInput[%d].\n",Info->TCUStatus.crashInput);
    printMessage("intRtc[%d].\n",Info->TCUStatus.intRtc);
    printMessage("gsmModem[%d].\n",Info->TCUStatus.gsmModem);
    printMessage("vehicleBatteryVol[%d].\n",Info->TCUStatus.vehicleBatteryVol);
    printMessage("Primary_Charge_Percentage[%d].\n",Info->TCUStatus.Primary_Charge_Percentage);
    printMessage("Primary_Battery_Voltage[%d].\n",Info->TCUStatus.Primary_Battery_Voltage);
    printMessage("Secondary_Charge_Percentage[%d].\n",Info->TCUStatus.Secondary_Charge_Percentage);
    printMessage("Secondary_Battery_Voltage[%d].\n",Info->TCUStatus.Secondary_Battery_Voltage);
    printMessage("configVersion[%d].\n",Info->TCUStatus.configVersion);
    printMessage("activation[%d].\n",Info->TCUStatus.activation);
    printMessage("mobilePhoneConnected[%d].\n",Info->TCUStatus.mobilePhoneConnected);
    printMessage("Sleep_Cycles_Start_time[%d].\n",Info->TCUStatus.Sleep_Cycles_Start_time);
    printMessage("getPSimState[%d].\n",Info->TCUStatus.getPSimState);
    printMessage("getSimState[%d].\n",Info->TCUStatus.getSimState);

    printMessage("statusIndicatorFailur[%d].\n",Info->TCUStatus.statusIndicatorFailur);
    printMessage("batteryFailure[%d].\n",Info->TCUStatus.batteryFailure);
    printMessage("batteryVoltageLow[%d].\n",Info->TCUStatus.batteryVoltageLow);
    printMessage("commModuleInterfaceFailure[%d].\n",Info->TCUStatus.commModuleInterfaceFailure);
    printMessage("gnssReceiverFailure[%d].\n",Info->TCUStatus.gnssReceiverFailure);
    printMessage("commModuleFailure[%d].\n",Info->TCUStatus.commModuleFailure);
    printMessage("gsmDivAntenna[%d].\n",Info->TCUStatus.gsmDivAntenna);

}

bool diag_bubstatus(int32_t argc, char **argv)
{
    int32_t timeout=1;

    sp<TUStatusInfo> Info = new TUStatusInfo;

    sp<IDiagManagerService> gDiagInterface = interface_cast<IDiagManagerService>
            (defaultServiceManager()->getService(String16("service_layer.DiagManagerService")));

    gDiagInterface->SetUpTUStatusData(0x0D);

    sDiagWaiter->setTimeout(timeout);

    sDiagWaiter->wait();
    if (sDiagWaiter->getResult() == 0) {
        printMessage(" Scan complete...\n");
    }
    gDiagInterface->GetTUStatusData(Info);

    if(Info->TCUStatus.Primary_Battery_Voltage>30)
    {
        LOGV("Backup battery functioning.");
        printMessage("Backup battery functioning.\n");
        printMessage("Backup battery charging percentage is[%d].\n",Info->TCUStatus.Primary_Charge_Percentage);
        printMessage("Backup battery voltage is[%d].\n",Info->TCUStatus.Primary_Battery_Voltage);
    }else{
        //if(Info->TCUStatus.intPower==0x00)
        {
            LOGV("Backup battery broken.");
            printMessage("Backup battery broken.\n");
        }
    }

}
bool diag_speakerstatus(int32_t argc, char **argv)
{
    int32_t timeout=1;

    sp<TUStatusInfo> Info;

    sp<IDiagManagerService> gDiagInterface = interface_cast<IDiagManagerService>
            (defaultServiceManager()->getService(String16("service_layer.DiagManagerService")));

    gDiagInterface->SetUpTUStatusData(0x0D);

    sDiagWaiter->setTimeout(timeout);

    sDiagWaiter->wait();
    if (sDiagWaiter->getResult() == 0) {
        printMessage(" Scan complete...\n");
    }
    gDiagInterface->GetTUStatusData(Info);

    if(Info->TCUStatus.speaker==0x00){
        LOGV("Speaker broken.");
        printMessage("Speaker broken.\n");
    }else{
        LOGV("Speaker functioning.");
        printMessage("Speaker functioning.\n");
    }

/*
uint32_t  Power;
uint32_t  speaker;
uint32_t  mic;
uint32_t  gsmExtAntenna;
uint32_t  gnssAntenna;
uint32_t  extPower;
uint32_t  intPower;
uint32_t  can;
uint32_t  button;
uint32_t  crashInput;
uint32_t  intRtc;
uint32_t  gsmModem;

uint32_t  Primary_Charge_Percentage;
uint32_t  Primary_Battery_Voltage;
uint32_t  Secondary_Charge_Percentage;
uint32_t  Secondary_Battery_Voltage;
uint32_t  configVersion;
uint32_t  activation;

uint32_t  mobilePhoneConnected;
uint32_t  Sleep_Cycles_Start_time;

uint32_t  vehicleBatteryVol;

uint32_t getPSimState; // connect to PSIM or TSIM
uint32_t getSimState; // Status of SIM(P-SIM or T-SIM)

uint32_t statusIndicatorFailur; // ERA-GLONASS  statusIndicatorFailure                           (916E11, 916E15)
uint32_t batteryFailure;        // ERA-GLONASS batteryFailure                                           (E10909)
uint32_t batteryVoltageLow;     //ERA-GLONASS batteryVoltageLow                                    (E10991)
uint32_t commModuleInterfaceFailure; //ERA-GLONASS commModuleInterfaceFailure         (917904)
uint32_t gnssReceiverFailure;   //ERA-GLONASS gnssReceiverFailure                                (F00049)
uint32_t commModuleFailure;     //ERA-GLONASS commModuleFailure                              (9A5611, 9A5615, 9D5511, 9D5515)
uint32_t gsmDivAntenna;

*/
}
bool diag_micstatus(int32_t argc, char **argv)
{
    int32_t timeout=1;

    sp<TUStatusInfo> Info;

    sp<IDiagManagerService> gDiagInterface = interface_cast<IDiagManagerService>
            (defaultServiceManager()->getService(String16("service_layer.DiagManagerService")));

    gDiagInterface->SetUpTUStatusData(0x0D);

    sDiagWaiter->setTimeout(timeout);

    sDiagWaiter->wait();
    if (sDiagWaiter->getResult() == 0) {
        printMessage(" Scan complete...\n");
    }
    gDiagInterface->GetTUStatusData(Info);

    if(Info->TCUStatus.mic==0x00){
        LOGV("Mic broken.");
        printMessage("Mic broken.\n");
    }else{
        LOGV("Mic functioning.");
        printMessage("Mic functioning.\n");
    }

}
bool diag_extbattstatus(int32_t argc, char **argv)
{
    int32_t timeout=1;

    sp<TUStatusInfo> Info;

    sp<IDiagManagerService> gDiagInterface = interface_cast<IDiagManagerService>
            (defaultServiceManager()->getService(String16("service_layer.DiagManagerService")));

    gDiagInterface->SetUpTUStatusData(0x0D);

    sDiagWaiter->setTimeout(timeout);

    sDiagWaiter->wait();
    if (sDiagWaiter->getResult() == 0) {
        printMessage(" Scan complete...\n");
    }
    gDiagInterface->GetTUStatusData(Info);

    if(Info->TCUStatus.Power==0x00){ //        Power=svc::mainBattery
        LOGV("External battery functioning.");
        printMessage("External battery functioning.\n");
        printMessage("External battery voltage is[%d].\n",Info->TCUStatus.vehicleBatteryVol);
    }else{
        LOGV("External battery broken.");
        printMessage("External battery broken.\n");
    }

}

uint8_t getIndexByAccessId(uint16_t dtcBytes)
{
    uint8_t result = 0xFF;
    for (int i = 0; i < DTC_TOTAL_DTC_SIZE; i++) {
        if (Diag_DTC_Backup.DTCData[i].DID == dtcBytes) {
            result = Diag_DTC_Backup.DTCData[i].index;
            break;
        }
    }
    return result;
}

uint8_t getDTCfaultData(uint8_t index1, uint8_t index2)
{
    uint8_t status = 0x00;
    if (index1 < DTC_TOTAL_DTC_SIZE) {
        status = Diag_DTC_Backup.DTCData[index1].FaultType[index2].FaultData;
    } else {
        index1 = DTC_TOTAL_DTC_SIZE - 1;
        status = Diag_DTC_Backup.DTCData[index1].FaultType[index2].FaultData;
    }
    return status;
}

bool dump_dtc_snapshot(int32_t argc, char **argv)
{
    printMessage("0560: VBUB_TEMP_CH.      FaultType : 0x04\n");        // 0x00
//    printMessage("1: BCLED_CNTL_CH.\n");
    printMessage("916E: ECLED_CNTL_CH.     FaultType : 0x11, 0x15\n");
//    printMessage("3: BCBLED_CNTL_CH.\n");
//    printMessage("4: ECBLED_CNTL_CH.\n");
//    printMessage("5: MIC_VOL_N_CH.\n");
//    printMessage("6: MIC_VOL_P_CH.\n");
    printMessage("E109: VBATT_CH.          FaultType : 0x09, 0x91\n");
//    printMessage("8: VBUB_CH.\n");
//    printMessage("9: BUTTON_DETECT2_CH.\n");
    printMessage("916d: BUTTON_DETECT1_CH. FaultType : 0x11, 0x15, 0x97\n");    //10
//    printMessage("11: EXT_MIC_VOL_N_CH.\n");
    printMessage("917A: EXT_MIC_VOL_CH.    FaultType : 0x12, 0x14\n");
#ifdef DIAG_FEATURE_19MY
    printMessage("13: ANTENNA_WIFI2_CH.    FaultType : 0x11, 0x15\n");
#endif
    printMessage("94B5: ANTENNA_WIFI1_CH.  FaultType : 0x11, 0x15\n");
//    printMessage("15: BUB_STATUS_CH.\n");
    printMessage("90A2: AIRBAG_CH.         FaultType : 0x12, 0x14\n");
//    printMessage("17: HW_OPTION_CH.\n");
//    printMessage("18: RELAY_CH.\n");
    printMessage("C001: CAN_HS_CH.         FaultType : 0x87, 0x88\n");
    printMessage("C010: CAN_MS_CH.         FaultType : 0x87, 0x88\n");      //20
    printMessage("C064: CAN_SINGLE_CH.     FaultType : 0x87, 0x88\n");   //21
    printMessage("9A01: AUDIO_AMP_CH.      FaultType : 0x12, 0x14\n");
    printMessage("9A56: Antenna_Main.      FaultType : 0x11, 0x15\n" );        //23 Main
    printMessage("9D55: Antenna_Div.       FaultType : 0x11, 0x15\n");           //24 DIV
    printMessage("919F: GNSS_Antenna.      FaultType : 0x01, 0x13\n");    // 25 GNSS
    printMessage("916F: SIM_CARD.          FaultType : 0x09\n");    // 26 SIM Interface
    printMessage("9252: External_USB_Port. FaultType : 0x01, 0x13, 0x87\n"); //27
    printMessage("94A9: Ethernet_LINK_PLUS.FaultType : 0x13,  0x64\n"); //28
    printMessage("94A9: Ethernet_LINK_MINUS.FaultType : 0x13, 0x64\n"); //29
    printMessage("C300: Internal_Software_Incompatibility. FaultType : 0x00\n"); //30
    printMessage("E01A: ControlModule_Cal_Data. FaultType : 0x51, 0x54\n");  //31
    printMessage("9179: Integrated_GSMGPRS_Unit.FaultType : 0x04\n"); //32
    printMessage("F000: Internal_Control_Module.FaultType : 0x44, 0x49\n"); //33

    uint8_t snapshotRecordNumber = 0x00;
    uint8_t faultType=0x00;
    int32_t h;
    int32_t i;
    bool snapshot10 = false;
    bool snapshot11 = false;
    bool snapshot12 = false;
    bool snapshot13 = false;

    bool snapshot20 = false;
    bool snapshot21 = false;
    bool snapshot22 = false;
    bool snapshot23 = false;

    bool snapshot30 = false;
    bool snapshot31 = false;
    bool snapshot32 = false;
    bool snapshot33 = false;
    if(!strcmp("10", argv[1])){
        snapshot10 = true;
        snapshotRecordNumber=0x10;
    }else if(!strcmp("11", argv[1])){
        snapshot11 = true;
        snapshotRecordNumber=0x11;
    }else if(!strcmp("12", argv[1])){
        snapshot12 = true;
        snapshotRecordNumber=0x12;
    }else if(!strcmp("13", argv[1])){
        snapshot13 = true;
        snapshotRecordNumber=0x13;
    }else if(!strcmp("20", argv[1])){
        snapshot20 = true;
        snapshotRecordNumber=0x20;
    }else if(!strcmp("21", argv[1])){
        snapshot21 = true;
        snapshotRecordNumber=0x21;
    }else if(!strcmp("22", argv[1])){
        snapshot22 = true;
        snapshotRecordNumber=0x22;
    }else if(!strcmp("23", argv[1])){
        snapshot23 = true;
        snapshotRecordNumber=0x23;
    }else if(!strcmp("30", argv[1])){
        snapshot30 = true;
        snapshotRecordNumber=0x30;
    }else if(!strcmp("31", argv[1])){
        snapshot31 = true;
        snapshotRecordNumber=0x31;
    }else if(!strcmp("32", argv[1])){
        snapshot32 = true;
        snapshotRecordNumber=0x32;
    }else if(!strcmp("33", argv[1])){
        snapshot33 = true;
        snapshotRecordNumber=0x33;
    }else{

    }
    DTC_LoadToFile_sldd();

    uint16_t DID;
    if ((argc > 2) && (argc < DTC_TOTAL_DTC_SIZE)) {
        DID=(uint16_t)strtol(argv[0], NULL, 16);
        faultType=(uint8_t)strtol(argv[1], NULL, 16);
        h=getIndexByAccessId(DID);
        for ( i = 0; i < DTC_FAULT_TYPE_COUNT; i++) {
            if(faultType==getDTCfaultData(h,i)){ // find proper index of 'i'
                break;
            }
        }
        snapshotRecordNumber=strtol(argv[2], NULL, 16);
        printMessage("DID[%4x],faultType[%2x],snapshotRecordNumber[%2X].\n", DID,faultType,snapshotRecordNumber);
        printMessage("h[%d],i[%d],snapshotRecordNumber[%2X].\n", h,i,snapshotRecordNumber);
    } else {
        printMessage("Wrong parameter : sldd diag snapshot snapshotRecordNum(Upper-4bits) snapshotRecordNum(Lower-4bits) \n");
        printMessage("Example : sldd diag snapshot 1 4  => It means read snaphot data of the RecordNumber 0x14.\n");
        return false;
    }
    if((h>=DTC_TOTAL_DTC_SIZE)||(i>=DTC_FAULT_TYPE_COUNT)){
        printMessage("h[%d],i[%d],index is out of ranges.\n", h,i);
    }
    sp<Buffer> result = new Buffer;
    long tempData = 0;
    DID = Diag_DTC_Backup.DTCData[h].DID;
    if (DID != DTC_UNDEFINED_ID) {
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
        printMessage("DTC snapshot result.\n");
        printMessage("%s                   | %s          | %s         | %s    | %s    \n", "Index", "DID", "Fault Byte", "Snapshot ID", "Data");
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
        for (int32_t j = 0; j < SNAPSHOTRECORD_NUM_MAX; j++) {
            for (int32_t k = 0; k < SNAPSHOTRECORD_NUM_SET_CNT; k++) {

                if((snapshotRecordNumber==Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotRecordNumber)
                    ||(snapshotRecordNumber==0xff)
                ){
                    printMessage("SnapshotBufferEmpty info is [%2x].\n",Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotBufferEmpty);
                    for (int32_t m = 0; m < DTC_NUMBER_OF_SNAPSHOT_ID; m++) {
                        uint32_t size = 0;

                        for (int32_t count = 0; count < SNAPSHOT_DATA_MAX_SIZE; count++) { //clear snapshotDataBuffer
                            snapshotDataBuffer[count] = 0x00;
                        }

                        result->clear();
                        tempData=0;
                        get_snapshotData(h, i, j, k, dtc_snapshot_id[m],result);
                        size = result->size();
                        memcpy((void*) &snapshotDataBuffer[0], (void*) result->data(), (unsigned int) result->size());
                        if (size > 0) {
                            for (uint32_t n = 0; n < size; n++) {
                                tempData |= (long)(snapshotDataBuffer[n] << (8 * (size - n - 1)));
                            }
                        }
                        if ((DID != DTC_UNDEFINED_ID) && ((Diag_DTC_Backup.DTCData[h].FaultType[i].FaultData != 0x00) || ((h == Internal_Software_Incompatibility) && (i == 0x00)))) {
                            printMessage("Item Name[%20s]   | DID[0x%4x]   | FaultByte[0x%4x] | SnapshotRecordNum[0x%4x] | Snapshot ID[0x%4x] | Data[0x%8x].\n", dtc_index_name[h], DID,
                                    Diag_DTC_Backup.DTCData[h].FaultType[i].FaultData, Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotRecordNumber,
                                    dtc_snapshot_id[m], tempData);
                        }
                    }
                }else{
                    printMessage("SnapshotRecordNumber not matched. [%2x]!=[%2x]\n",snapshotRecordNumber,Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotRecordNumber);
                }
            }
        }
    }else{
        printMessage("Inserted index is not used for backup. ==> DID is [DTC_UNDEFINED_ID]\n");
    }
    printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    return true;
}

static void dump_dtc_status_byte()
{
    uint8_t FaultData;
    int8_t FaultDetectCounter;
    uint8_t StatusByte;
    DTC_LoadToFile_sldd();
    for (int32_t h = 0; h < DTC_TOTAL_DTC_SIZE; h++) {
        uint16_t DID = Diag_DTC_Backup.DTCData[h].DID;
        if (DID != DTC_UNDEFINED_ID) {
            printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
            printMessage("DTC Status check result.\n");
            printMessage("%s                   | %s          | %s         | %s    | %s    \n", "Index", "DID", "Fault Byte", "FaultDetectionCounter", "StatusByte" );
            printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
            for (int32_t i = 0; i < DTC_FAULT_TYPE_COUNT; i++) {
                FaultData = Diag_DTC_Backup.DTCData[h].FaultType[i].FaultData;
                FaultDetectCounter = (int8_t) Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.DTCFaultDetectionCounter;
                StatusByte = Diag_DTC_Backup.DTCData[h].FaultType[i].Status;
                printMessage("Item Name[%20s]       | DID[0x%04x]   | FaultByte[0x%04x]  | FaultDetectionCounter[0x%04x] | StatusByte[%4x] \n",
                    dtc_index_name[h],
                    DID,
                    FaultData,
                    FaultDetectCounter,
                    StatusByte);
            }
            printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
}

uint8_t dtc_CaculateSnapshotRecordNumber(uint8_t a, uint8_t b)
{
    uint8_t result=0x00;
    LOGV("diag_CaculateSnapshotRecordNumber([%2x],[%2x])",a,b);
    if((a<SNAPSHOTRECORD_NUM_MAX)&&(b<SNAPSHOTRECORD_NUM_SET_CNT)){
        result = (uint8_t)( ((a+1)<<4)|(b) );//0x10, 0x11, 0x12, 0x13 ~ 0x30, 0x31, 0x32, 0x33
    }else{
        LOGE("diag_CaculateSnapshotRecordNumber param overflow...a[%2x],b[%2x])",a,b);
    }
    return result;
}

bool handler_diag_dtcClear(int32_t argc, char **argv)
{
//    sp<Buffer> buf;
    uint16_t DID;
    uint8_t FaultData;
    int8_t FaultDetectCounter;
    uint8_t StatusByte;

    DTC_LoadToFile_sldd();

    printMessage("\n-------------------------------------------------------------------------------------------------------------------\n");
    printMessage("DTC clear result.\n");
    printMessage("%s                   | %s          | %s         | %s \n", "Index", "DID", "Fault Byte", "FaultDetectionCounter" );
    printMessage("---------------------------------------------------------------------------------------------------------------------\n");

    /*Do clear diag info*/
    // All Group of DTC.
    for (int32_t h = 0; h < (DTC_TOTAL_DTC_SIZE); h++) {
        for (int32_t i = 0; i < (DTC_FAULT_TYPE_COUNT); i++) {
            Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotRecordNumberOfIdentifiers=(uint8_t)DTC_NUMBER_OF_SNAPSHOT_ID; // It show for each DTC faultType, how many Global Snapshot Dataes are stored.
            for (int32_t j = 0; j < (SNAPSHOTRECORD_NUM_MAX); j++) {
#if 1
                for (int32_t k = 0; k < SNAPSHOTRECORD_NUM_SET_CNT; k++) {
                    Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotBufferEmpty = true;
                    Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotRecordNumber = dtc_CaculateSnapshotRecordNumber(j, k);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].GlobalRealTime[0]), 0x00, DID_DD00_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].TotalDistance[0]), 0x00, DID_DD01_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].MainECUSupplyVoltage[0]), 0x00, DID_DD02_SIZE);
//                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].EngineStatus[0]), 0x00, DID_DD03_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].IncarTemp[0]), 0x00, DID_DD04_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].OutDoorTemp[0]), 0x00, DID_DD05_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].PowerMode[0]), 0x00, DID_DD06_SIZE);
//                        memset((void*)&(Diag_DTC_Buffer.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].PowerModeQF[0]),0x00,DID_DD07_SIZE);
//                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].EngineRPM[0]), 0x00, DID_DD08_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].VehicleSpeed[0]), 0x00, DID_DD09_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].GPS_Time[0]), 0x00, DID_DD0A_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].HSCAN_Network_Management_State[0]), 0x00, DID_D10E_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].MSCAN_Network_Management_State[0]), 0x00, DID_D702_SIZE);
//                        memset((void*)&(Diag_DTC_Buffer.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].TCU_Temperature[0]),0x00,DID_D98A_SIZE);
                    memset((void*) &(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].TCU_Main_Battery_Vol[0]), 0x00, DID_D110_SIZE);
                }
                Diag_DTC_Backup.DTCData[h].FaultType[i].Status = DTC_CLEAR_VAL;
#endif
            }
            Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.MonitoringCycleCounter1 = DTC_CLEAR_VAL;
            Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.MonitoringCycleCounter2 = DTC_CLEAR_VAL;
            Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.MonitoringCycleCounter3 = DTC_CLEAR_VAL;
            Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.MonitoringCycleCounter4 = DTC_CLEAR_VAL;
            Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.DTCFaultDetectionCounter = DTC_CLEAR_VAL;
            DiagExtendedData_Symtom_Type temp = {0,};
            memcpy(Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.Symptom_Associated_with_DTC,temp,sizeof(DiagExtendedData_Symtom_Type));
            memcpy(Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.Symptom_Potentially_caused_by_DTC,temp,sizeof(DiagExtendedData_Symtom_Type));
        }
    }

    for (int32_t h = 0; h < DTC_TOTAL_DTC_SIZE; h++) {
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
        printMessage("DTC Status check result.\n");
        printMessage("%s                   | %s          | %s         | %s    | %s    \n", "Index", "DID", "Fault Byte", "FaultDetectionCounter", "StatusByte");
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
        for (int32_t i = 0; i < DTC_FAULT_TYPE_COUNT; i++) {
            DID = Diag_DTC_Backup.DTCData[h].DID;
            FaultData = Diag_DTC_Backup.DTCData[h].FaultType[i].FaultData;
            FaultDetectCounter = (int8_t) Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.DTCFaultDetectionCounter;
            StatusByte = Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.DTCFaultDetectionCounter;

            printMessage("Item Name[%20s]             | DID[0x%04x]    | FaultByte[0x%04x]  | FaultDetectionCounter[0x%04x] \n",
                dtc_index_name[h],
                DID,
                FaultData,
                FaultDetectCounter);
        }
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    }

    DTC_RemoveFile();
    return true;
}

bool handler_diag_dtcstatus(int32_t argc, char **argv)
{
    bool faultcount = false;
    bool snapshot = false;
    bool extenddata = false;
    bool statusbyte = false;
    if (argc == 0 || (argc == 1 && !strcmp("faultcount", argv[0]))) {
        faultcount = true;
    } else if (argc == 0 || (argc == 1 && !strcmp("snapshot", argv[0]))) {
        snapshot = true;
    } else if (argc == 0 || (argc == 1 && !strcmp("extenddata", argv[0]))) {
        extenddata = true;
    } else if (argc == 0 || (argc == 1 && !strcmp("statusbyte", argv[0]))) {
        statusbyte = true;
    }else{
        printMessage("Not crretct parameters are on.\n");
    }

    bool antenna = false;
    bool display_result = false;
    while (argc > 0) {
        if (!strcmp("antenna", argv[0])) {
            antenna = true;
        } else if (!strcmp("usb", argv[0])) {
            display_result = true;
        } else {
            /// do nothing
        }
        argc--;
        argv++;
    }

    printMessage("==============================================\n");
    printMessage("  Diagnostic DTC status\n");
    printMessage("----------------------------------------------\n");
    ant_status_t ant_status = {};
    int32_t set_ret = E_ERROR;
//    uint32_t status;
//    uint32_t value;
    if(antenna){
        printMessage("\nANTENNAs\n");
        handler_common_getActivePhoneAntenna(argc, argv);

        printMessage("%15s | %15s | %5s \n", "Device Name", "DTC Status", "value");
        printMessage("-----------------------------------------------\n");
        set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_MAIN, &ant_status);
//        status = SL_INTERFACE_GET(SystemManagerService)->readAntennaDTCStatus(ANTENNA_MAIN);  /* readAntennaDTCStatus was removed by SystemManagerService */
//        value = SL_INTERFACE_GET(SystemManagerService)->readAntennaADCValue(ANTENNA_MAIN);    /* readAntennaADCValue was removed by SystemManagerService */
        printMessage("%15s | %15s | %5d \n", "MAIN Antenna",
                          dtc_status_str[ant_status.status], ant_status.adc_value);

        ant_status.adc_value = 0;
        ant_status.detect_fault_cnt = 0;
        ant_status.status = DTC_STATUS_INIT;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_SUB, &ant_status);
//        status = SL_INTERFACE_GET(SystemManagerService)->readAntennaDTCStatus(ANTENNA_SUB);   /* readAntennaDTCStatus was removed by SystemManagerService */
//        value = SL_INTERFACE_GET(SystemManagerService)->readAntennaADCValue(ANTENNA_SUB);     /* readAntennaADCValue was removed by SystemManagerService */
        printMessage("%15s | %15s | %5d \n", "MAIN Div",
                          dtc_status_str[ant_status.status], ant_status.adc_value);

        ant_status.adc_value = 0;
        ant_status.detect_fault_cnt = 0;
        ant_status.status = DTC_STATUS_INIT;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_GNSS, &ant_status);
//        status = SL_INTERFACE_GET(SystemManagerService)->readAntennaDTCStatus(ANTENNA_GNSS);  /* readAntennaDTCStatus was removed by SystemManagerService */
//        value = SL_INTERFACE_GET(SystemManagerService)->readAntennaADCValue(ANTENNA_GNSS);   /* readAntennaADCValue was removed by SystemManagerService */
        printMessage("%15s | %15s | %5d \n", "GNSS Antenna",
                          dtc_status_str[ant_status.status], ant_status.adc_value);
    }

    if (statusbyte || display_result) {
        dump_dtc_status_byte();
    } else if (snapshot || display_result) {
        //dump_dtc_snapshot();
    } else if (extenddata || display_result) {
        //dump_dtc_result();
    } else if (faultcount || display_result) {
        dump_dtc_status_byte();
    } else {
        printMessage("Type option keyword. 'faultcount' or 'snapshot', 'extenddata', 'statusbyte'.");
    }
    return true;
}

bool handler_diag_tcusvcstatus(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_trackingstatus(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_gnssparam(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_netswversion(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_iccnumber(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_gprsstatus(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_regstatus(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_resetdtc(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_testodst(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_testbub(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_testractorytest(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_testoperationmode(int32_t argc, char **argv)
{
    return false;
}

bool handler_diag_testwriteseckey(int32_t argc, char **argv)
{
    return false;
}

void register_diag()
{
    registerCommands(MODULE_DIAG_MANAGER, NULL, diag_cmdTable);
}
