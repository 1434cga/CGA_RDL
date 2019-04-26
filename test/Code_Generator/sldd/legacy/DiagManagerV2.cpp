/*
 * DiagManagerV2.cpp
 *
 *  Created on: 2016. 1. 12.
 *      Author: daeup.kim
 */


#define LOG_TAG "DiagManagerV2"
#include "Log.h"

#include <stdlib.h>

#include "daemon/sldd/sldd_common.h"
#include "utils/atoi.h"
#include "daemon/sldd/man.h"

#include "daemon/sldd/DiagManagerV2.h"

#include "services/DiagManagerService/DiagCommand.h"
//jkbyun compile error #include "services/DiagManagerService/DiagManagerService.h"
#include "services/DiagManagerService/IDiagManagerService.h"
#include "services/SystemManagerService/ISystemManagerService.h"
//#include "services/SystemManagerService/SystemUtils.h" // yoojs compile error
#include "services/SystemManagerService/ISystemManagerServiceType.h"//yoojs compile error


//jkbyun compile error #include "services/DiagManagerService/DiagSlddBridge.h"
//jkbyun compile error #include "services/DiagManagerService/DiagService.h"

//Remove DebugManagerService by daehun.ju@lge.com #include "jlr/jlr_tf1_cmds_handler.h"

#define DIAG_SUPPORT_STORE_TSIM_PROFILE // PCR011, Backup TSIM profiles. In case of multi profile, backup 2 kinds of profile information - Regular / Emergency(for russia)

enum Commands {
    CMD_DIAG_MODE = 0,
    CMD_DIAG_INIT = 1,
    CMD_DTC_STATUS,
    CMD_DTC_CLEAR,
    CMD_DTC_SNAPSHOT,
    CMD_DTC_CONTROL_SETTING,
    CMD_ECU_RESET,
    CMD_IO_CONTROL,
    CMD_READ_DATA,
    CMD_SET_DTC_NOTI,
#ifdef DIAG_SUPPORT_STORE_TSIM_PROFILE
    CMD_GET_TSIM_PROFILE,
#endif
    CMD_ROUTINE_CONTROL,
    CMD_SESSION_CONTROL,
    CMD_TESTER_PRESENT,
    CMD_WRITE_DATA,
    CMD_TU_STATUS,
    CMD_BUB_STATUS,
    CMD_SPEAKER_STATUS,
    CMD_MIC_STATUS,
    CMD_EXTBATT_STATUS,
    CMD_READ_DTC_INFO,

    CMD_MCU_WRITE,
    CMD_MCU_READ,
    SLDD_DIAG_CMD,
    CMD_SET_BUBTEST_WAITTIME

};

#ifdef DIAG_SUPPORT_STORE_TSIM_PROFILE
const char sldd_SIM_PROFILE_PATH[] = "/userdata/config/SIMProfile.txt";
#endif

const char sldd_TIME_INFO_PATH[] = "/userdata/config/DiagTimeInfo.txt";

static bool diagDebugMode(int32_t argc, char **argv);
static bool diagInit(int32_t argc, char **argv);
static bool diagDtcStatus(int32_t argc, char **argv);
static bool diagDtcClear(int32_t argc, char **argv);
static bool diagDtcSnapshot(int32_t argc, char **argv);
static bool diagControlSetting(int32_t argc, char **argv);
static bool diagECUReset(int32_t argc, char **argv);
static bool diagIOControl(int32_t argc, char **argv);
static bool diagReadData(int32_t argc, char **argv);
static bool diagRoutineControl(int32_t argc, char **argv);
static bool diagSessionControl(int32_t argc, char **argv);
static bool diagTesterPresent(int32_t argc, char **argv);
static bool diagWriteData(int32_t argc, char **argv);
#ifdef DIAG_SUPPORT_STORE_TSIM_PROFILE
static bool diagGetTSIMProfile(int32_t argc, char **argv);
#endif
static bool diagSetDTCNoti(int32_t argc, char **argv);
static bool diagReadDTCInfo(int32_t argc, char **argv);


static bool diagTUstatus(int32_t argc, char **argv);
static bool diagBubstatus(int32_t argc, char **argv);
static bool diagSpeakerstatus(int32_t argc, char **argv);
static bool diagMicstatus(int32_t argc, char **argv);
static bool diagExtbattstatus(int32_t argc, char **argv);

//
static bool diagTcuSvcStatus(int32_t argc, char **argv);
static bool diagTrackingStatus(int32_t argc, char **argv);
static bool diagGnssParam(int32_t argc, char **argv);
static bool diagNetSwVersion(int32_t argc, char **argv);
static bool diagIccNumber(int32_t argc, char **argv);
static bool diagGprsStatus(int32_t argc, char **argv);
static bool diagRegStatus(int32_t argc, char **argv);
static bool diagResetDtc(int32_t argc, char **argv);
static bool diagTestOdst(int32_t argc, char **argv);
static bool diagTestBub(int32_t argc, char **argv);
static bool diagTestRactorytest(int32_t argc, char **argv);
static bool diagTestOperationMode(int32_t argc, char **argv);
static bool diagTestWriteSeckey(int32_t argc, char **argv);

static bool diagCmdMcuWrite(int32_t argc, char **argv);
static bool diagCmdMcuRead(int32_t argc, char **argv);
static bool slddDiagCmd(int32_t argc, char **argv);
//

static uint8_t* diagBridgeDataSet(uint8_t &cmd, uint8_t &cmd2, char **argv);
static uint8_t dtc_CaculateSnapshotRecordNumber(uint8_t a, uint8_t b);

static error_t DiagLoadTimeInfo();
static error_t Diag_SetTimeInfo(uint8_t index, time_t timesec );
static uint32_t compareTime(time_t refTime);
static time_t getCurrentTime();
static bool slddDiagSetBUBTestWaitTime(int32_t argc, char **argv);

SL_INTERFACE_DECLARE2(SystemManagerService, SYSTEM_SRV_NAME);

uint8_t snapshotDataBuffer[SNAPSHOT_DATA_MAX_SIZE];
sldd_cmd_table_t diag_cmdTable[] = {
    {"CMD_DIAG_MODE", CMD_DIAG_MODE, diagDebugMode, "Diag Sldd Debug Mode!D\n"},
    {"CMD_DIAG_INIT", CMD_DIAG_INIT, diagInit, "Diag Sldd Init!D\n"},
    {"CMD_DTC_STATUS", CMD_DTC_STATUS, diagDtcStatus, "DTC STATUS CMD\n"},
    {"CMD_DTC_CLEAR", CMD_DTC_CLEAR, diagDtcClear, "DTC CLEAR CMD\n"},
    {"CMD_DTC_SNAPSHOT", CMD_DTC_SNAPSHOT, diagDtcSnapshot, "DTC SNAPSHOT\n"},
    {"CMD_DTC_CONTROL_SETTING", CMD_DTC_CONTROL_SETTING, diagControlSetting, "DTC CONTROL SETTING\n"},
    {"CMD_ECU_RESET", CMD_ECU_RESET, diagECUReset, "ECU RESET\n"},
    {"CMD_IO_CONTROL", CMD_IO_CONTROL, diagIOControl, "IO CONTROL\n"},
    {"CMD_READ_DATA", CMD_READ_DATA, diagReadData, "READ DATA BY ID\n"},
    {"CMD_ROUTINE_CONTROL", CMD_ROUTINE_CONTROL, diagRoutineControl, "ROUTINE CONTROL\n"},
    {"CMD_SESSION_CONTROL", CMD_SESSION_CONTROL, diagSessionControl, "SESSION CONTROL\n"},
    {"CMD_TESTER_PRESENT", CMD_TESTER_PRESENT, diagTesterPresent, "TESTER PRESENT\n"},
    {"CMD_WRITE_DATA", CMD_WRITE_DATA, diagWriteData, "WRITE DATA BY ID\n"},
//  {"CMD_READ_DATA", CMD_READ_DATA, diagReadData, "READ DATA BY ID\n"},
    {"CMD_SET_DTC_NOTI", CMD_SET_DTC_NOTI, diagSetDTCNoti, "CMD SET DTC NOTI\n"},
#ifdef DIAG_SUPPORT_STORE_TSIM_PROFILE
    {"CMD_GET_TSIM_PROFILE", CMD_GET_TSIM_PROFILE, diagGetTSIMProfile, "CMD GET TSIM PROFILE\n"},
#endif
    {"CMD_READ_DTC_INFO", CMD_READ_DTC_INFO, diagReadDTCInfo, "CMD READ DTC INFO\n"},

    {"CMD_TU_STATUS", CMD_TU_STATUS, diagTUstatus, "TU STATUS\n"},
    {"CMD_BUB_STATUS", CMD_BUB_STATUS, diagBubstatus, "BUB STATUS\n"},
    {"CMD_SPEAKER_STATUS", CMD_SPEAKER_STATUS, diagSpeakerstatus, "SPEAKER STATUS\n"},
    {"CMD_MIC_STATUS", CMD_MIC_STATUS, diagMicstatus, "MIC STATUS\n"},
    {"CMD_EXTBATT_STATUS", CMD_EXTBATT_STATUS, diagExtbattstatus, "EXT BATTERY STATUS\n"},

    {"CMD_MCU_WRITE", CMD_MCU_WRITE, diagCmdMcuWrite, "SLDD MCU WRITE CMD\n"},
    {"CMD_MCU_READ", CMD_MCU_READ, diagCmdMcuRead, "SLDD MCU READ CMD\n"},
    {"SLDD_DIAG_CMD", SLDD_DIAG_CMD, slddDiagCmd, "SLDD DIAG DEBUG CMD\n"},
    {"CMD_SET_BUBTEST_WAITTIME", CMD_SET_BUBTEST_WAITTIME, slddDiagSetBUBTestWaitTime, "CMD_SET_BUBTEST_WAITTIME\n"},

    { NULL, NULL, NULL, NULL}
};

bool commandActionDiag(int32_t argc, char **argv)
{
    if (argc == 0) {
        printMessage("%s", usage_diag(NULL));
        return true;
    }
    //DO_HANDLER(diag_cmdTable, argv[0], argc-1, argv+1);
    for (int32_t i=0; i<(sizeof(diag_cmdTable) / sizeof(sldd_cmd_table_t))-1; i++) {
        if (!strcasecmp(diag_cmdTable[i].name, argv[0])) {
            bool ret = diag_cmdTable[i].handler(argc-1, argv+1);
            if(ret == false)
                printMessage(diag_cmdTable[i].simple_usage);
            return true;
        }
    }
    return false;
}

char* usage_diag(char *cmd)
{
    printMessage(" sldd diag [<command>]           - perform diag operation for TCU3 \n");
    printMessage("    <command> \n");
    for (int32_t i=0; i<(sizeof(diag_cmdTable) / sizeof(sldd_cmd_table_t))-1; i++) {
        printMessage("  [command] %s \n", diag_cmdTable[i].name );
        printMessage("    [usage]\n %s \n", diag_cmdTable[i].simple_usage );
    }
    return NULL;
}

const char* dtc_status_str[] =
{
    "Normal",
    "Short VBATT",
    "Short GND",
    "Open",
};

uint16_t dtc_snapshot_id[DTC_NUMBER_OF_SNAPSHOT_ID] =
{
    0xDD00,//SNAPSHOT_DID_GLOBAL_REAL_TIME  =
    0xDD01,//SNAPSHOT_DID_TOTAL_DISTANCE    =
    0xDD02,//SNAPSHOT_DID_MAIN_VBATT_VOL    =
//    0xDD03,//SNAPSHOT_DID_ENGINE_STATUS     =
    0xDD04,//SNAPSHOT_DID_IN_CAR_TEMP       =
    0xDD05,//SNAPSHOT_DID_OUTDOOR_TEMP      =
    0xDD06,//SNAPSHOT_DID_POWER_MODE        =
           //SNAPSHOT_DID_POWER_MODE_QF     = 0xDD07,
//    0xDD08,//SNAPSHOT_DID_ENGINE_RPM        =
    0xDD09,//SNAPSHOT_DID_VEHICLE_SPEED     =
    0xDD0A,//SNAPSHOT_DID_GPS_TIME          =
    0xD10E,//SNAPSHOT_DID_HSCAN_STATUS      =
    0xD702,//SNAPSHOT_DID_MSCAN_STATUS      =
    //0xD98A,//SNAPSHOT_DID_TCU_TEMP          =
    0xD110,//SNAPSHOT_DID_MAIN_BATTERY       =
    0xD020//SNAPSHOT_DID_MAIN_BATTERY       =
};

/*Sync with DTC_FILE_PATHS*/
const char DTC_FILE_PATHS[] = "/userdata/config/DTC_Backup.txt";
DTC_Backup_File_Format Diag_DTC_Backup;

uint8_t properIndex1(uint8_t index1)
{
    uint8_t result=0x00;
    if (index1 >= DTC_TOTAL_DTC_SIZE){
        result = DTC_TOTAL_DTC_SIZE-1;
    }else{
        result = index1;
    }
    return result;
}

uint8_t properIndex2(uint8_t index2)
{
    uint8_t result=0x00;
    if (index2 >= DTC_FAULT_TYPE_COUNT){
        result = DTC_FAULT_TYPE_COUNT-1;
    }else{
        result = index2;
    }
    return result;
}


error_t DTC_LoadToFile_sldd()
{
    LOGV("DTC_LoadToFile");

    int32_t retVal = 0;
    FILE* f = fopen(DTC_FILE_PATHS, "r+");
    if(f == NULL) {
        LOGE("============ DTCLoadToFile : File is not exist. ============");
        return E_ERROR;
    } else {
        LOGI("============ Load To File start ============");

        uint16_t size = sizeof(DTC_Backup_File_Format);
        char* dtcData = (char *)malloc(sizeof(char) * DIAG_DTC_BUFFER_MAX);
        if(dtcData == NULL) {
            LOGE("DTC_LoadToFile_sldd() Failed memory allocation");
            fclose(f);
            return E_ERROR;
        }

        retVal = fread((char*)dtcData, 1, size, f);
        fclose(f);

        /*while( EOF != (buffer = fgetc( f))) {
            if(count<size){
                dtcData[count]=buffer;
            }else{
                break;
            }
            count++;
        }*/

        memcpy((char*)(&Diag_DTC_Backup), (char*)dtcData, size);

        free(dtcData);

        for (int32_t i = 0; i < (DTC_TOTAL_DTC_SIZE); i++) {
            Diag_DTC_Backup.DTCData[i].index = (uint8_t) i;
            Diag_DTC_Backup.DTCData[i].DID = 0; //DTC_ID[i]; // save to JLR P2 event type DID. // FIXME by cht-t@lge.com
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
        memset((void*) save_ptr, 0x00 , DIAG_DTC_BUFFER_MAX);
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
    return nResult;
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
    } else if (did == DIAG_SNAPSHOT_DID_NETWORK_OP_SYSTEM) {
        result->setTo(Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].NetworkingOperatingSystem, DID_D020_SIZE);
    } else {
    }
//    printMessage("h[%d],i[%d],j[%d],k[%d,did[%4x],result[%s]\n",h,i,j,k,did,result->data());
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
    result=properIndex1(result);
    return result;
}

uint8_t getDTCfaultData(uint8_t index1, uint8_t index2)
{
    uint8_t status = 0x00;
    uint8_t param1;
    uint8_t param2;
    param1=properIndex1(index1);
    param2=properIndex2(index2);
    status = Diag_DTC_Backup.DTCData[param1].FaultType[param2].FaultData;
    return status;
}

bool diagDtcSnapshot(int32_t argc, char **argv)
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
    DTC_LoadToFile_sldd();
    uint8_t param1;
    uint8_t param2;
    uint16_t DID;
    if ((argc > 2) && (argc < DTC_TOTAL_DTC_SIZE)) {
        DID=(uint16_t)strtol(argv[0], NULL, 16);
        faultType=(uint8_t)strtol(argv[1], NULL, 16);
        param1=getIndexByAccessId(DID);
        h=properIndex1(param1);
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
    param1=properIndex1(h);
    param2=properIndex2(i);
    h=param1;
    i=param2;
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
// FIXME by cht-t@lge.com
;
//                            printMessage("Item Name[%20s]   | DID[0x%4x]   | FaultByte[0x%4x] | SnapshotRecordNum[0x%4x] | Snapshot ID[0x%4x] | Data[0x%8x].\n", dtc_index_name[h], DID,
//                                    Diag_DTC_Backup.DTCData[h].FaultType[i].FaultData, Diag_DTC_Backup.DTCData[h].FaultType[i].SnapshotData[j].SshotData[k].SnapshotRecordNumber,
//                                    dtc_snapshot_id[m], tempData);
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
// FIXME by cht-t@lge.com
//                printMessage("Item Name[%20s]       | DID[0x%04x]   | FaultByte[0x%04x]  | FaultDetectionCounter[0x%04x] | StatusByte[%4x] \n",
//                    dtc_index_name[h],
//                    DID,
//                    FaultData,
//                    FaultDetectCounter,
//                    StatusByte);
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

bool diagDtcClear(int32_t argc, char **argv)
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
            memcpy(&Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.Symptom_Associated_with_DTC,&temp,sizeof(DiagExtendedData_Symtom_Type));
            memcpy(&Diag_DTC_Backup.DTCData[h].FaultType[i].ExtendedData.ExData.Symptom_Potentially_caused_by_DTC,&temp,sizeof(DiagExtendedData_Symtom_Type));
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
// FIXME by cht-t@lge.com
//            printMessage("Item Name[%20s]             | DID[0x%04x]    | FaultByte[0x%04x]  | FaultDetectionCounter[0x%04x] \n",
//                dtc_index_name[h],
//                DID,
//                FaultData,
//                FaultDetectCounter);
        }
        printMessage("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    }

    DTC_RemoveFile();
    return true;
}

bool diagDtcStatus(int32_t argc, char **argv)
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
//        status = SL_INTERFACE_GET(SystemManagerService)->readAntennaDTCStatus(ANTENNA_SUB);  /* readAntennaDTCStatus was removed by SystemManagerService */
//        value = SL_INTERFACE_GET(SystemManagerService)->readAntennaADCValue(ANTENNA_SUB);    /* readAntennaADCValue was removed by SystemManagerService */
        printMessage("%15s | %15s | %5d \n", "MAIN Div",
                          dtc_status_str[ant_status.status], ant_status.adc_value);

        ant_status.adc_value = 0;
        ant_status.detect_fault_cnt = 0;
        ant_status.status = DTC_STATUS_INIT;
        set_ret = SL_INTERFACE_GET(SystemManagerService)->getAntennaStatus(ANTENNA_GNSS, &ant_status);
//        status = SL_INTERFACE_GET(SystemManagerService)->readAntennaDTCStatus(ANTENNA_GNSS);  /* readAntennaDTCStatus was removed by SystemManagerService */
//        value = SL_INTERFACE_GET(SystemManagerService)->readAntennaADCValue(ANTENNA_GNSS);    /* readAntennaADCValue was removed by SystemManagerService */
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

bool diagTcuSvcStatus(int32_t argc, char **argv)
{
    return false;
}

bool diagTrackingStatus(int32_t argc, char **argv)
{
    return false;
}

bool diagGnssParam(int32_t argc, char **argv)
{
    return false;
}

bool diagNetSwVersion(int32_t argc, char **argv)
{
    return false;
}

bool diagIccNumber(int32_t argc, char **argv)
{
    return false;
}

bool diagGprsStatus(int32_t argc, char **argv)
{
    return false;
}

bool diagRegStatus(int32_t argc, char **argv)
{
    return false;
}

bool diagResetDtc(int32_t argc, char **argv)
{
    return false;
}

bool diagTestOdst(int32_t argc, char **argv)
{
    return false;
}

bool diagTestBub(int32_t argc, char **argv)
{
    return false;
}

bool diagTestRactorytest(int32_t argc, char **argv)
{
    return false;
}

bool diagTestOperationMode(int32_t argc, char **argv)
{
    return false;
}

bool diagTestWriteSeckey(int32_t argc, char **argv)
{
    return false;
}


uint8_t* diagBridgeDataSet(uint8_t &cmd, uint8_t &cmd2, char **argv)
{
    LOGV("Diag SLDD Bridge diagBridgeDataSet +");
    char *endptr;
    cmd = (uint8_t) strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t) strtol(argv[1], &endptr, 16);
    uint8_t accessID_H = (uint8_t) strtol(argv[2], &endptr, 16);
    uint8_t accessID_L = (uint8_t) strtol(argv[3], &endptr, 16);

    uint8_t NAD_SELF = SESSION_NAD_SELF;

    static uint8_t dataSet[3] = { 0, };
    dataSet[0] = accessID_H;
    dataSet[1] = accessID_L;
    dataSet[2] = NAD_SELF;

    printMessage("    INPUT DATA : CMD[0x%2x], CMD2[0x%2x], ACCESS ID H[0x%2x], ACCESS ID L[0x%2x], SESSION ID[%d] \n", cmd, cmd2, dataSet[0], dataSet[1], dataSet[2]);

    LOGV("Diag SLDD Bridge diagBridgeDataSet -");
    return dataSet;
}

bool slddDiagCmd(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    SLDD DIAG DBUEG COMMAND(DebugModeType:2) \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;
    char *endptr;
    static uint8_t len = 0;
    static uint8_t dataSet[20] = {0, };

    if (argc < 2)
        return false;

    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);

    for (uint8_t i = 0 ; i < (argc - 2) ; i++)
    {
        dataSet[i] = (uint8_t)strtol(argv[i + 2], &endptr, 16);
        len++;
    }

    LOGV("SLDD CMD [cmd:%X][cmd2:%X]-[%X][%X][%X][%X][%X][%X][%X][%X][%X][%X]", cmd, cmd2,
        dataSet[0], dataSet[1], dataSet[2], dataSet[3], dataSet[4], dataSet[5], dataSet[6], dataSet[7], dataSet[8], dataSet[9]);

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::SLDD_DIAG_CMD, cmd, cmd2, dataSet, len);
    return true;

}

bool diagDebugMode(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD Mode Setting \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0xFD;
    uint8_t cmd2 = 0x00;
    uint8_t buf =0x00;

    if (!strcmp(argv[0], "enable")) {
        cmd2 = 0x01;
        printMessage("    DIAG SLDD Mode enable  \n");
        DiagSlddBridge::getInstance().init();
    } else {
        cmd2 = 0x00;
        printMessage("    DIAG SLDD Mode disable  \n");
    }

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::SLDD_TEST_MODEID, cmd, cmd2, &buf,1);

    return true;
}

bool diagInit(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD PREPARE INIT \n");
    printMessage("-------------------------------------------------------\n");
    DiagSlddBridge::getInstance().init();
    return true;
}

bool diagControlSetting(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD Control Setting \n");
    printMessage("    ex)  cmd[0x85], cmd2[0x01/0x02/0x81/0x82], payload[0x00] \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);
    uint8_t data1 = (uint8_t)strtol(argv[2], &endptr, 16);

    static uint8_t len = 1;
    static uint8_t dataSet[1] = {0, };
    dataSet[0] = data1;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::CONTROL_DTC_SETTING, cmd, cmd2, dataSet, len);

    return false;
}

bool diagECUReset(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD ECU RESET \n");
    printMessage("    ex) cmd[0x11], cmd2[0x01/0x02/0x03/0x04/0x05/0x81/0x82/0x83/0x84/0x85],payload[0x00] \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);

    uint8_t data1 = (uint8_t)strtol(argv[2], &endptr, 16);

    static uint8_t len = 1;
    static uint8_t dataSet[1] = {0, };
    dataSet[0] = data1;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::ECU_RESET, cmd, cmd2, dataSet, len);

    return false;
}

bool diagIOControl(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD IO CONTROL BY ID \n");
    printMessage("    ex) cmd[0x2F], cmd2[0x00], payload[0xD9D7/0xD999/0xD99B] option[0x00/0x02/0x03], data[0x00] \n");
    printMessage("-------------------------------------------------------\n");
    // cmd = 0x2F, cmd2 = 0x00, access id = 0xD9D7 (LED), 0xD999(ecall/bcall), 0xD99B(gsm Antenna), 0x00/0x02/0x03, 0x00 ~ 0x64
    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);
    uint8_t accessID_H = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t accessID_L = (uint8_t)strtol(argv[3], &endptr, 16);

    uint8_t data1 = (uint8_t)strtol(argv[4], &endptr, 16);   //Control Option
    uint8_t data2 = (uint8_t)strtol(argv[5], &endptr, 16);   // data byte

    static uint8_t len = 4;
    static uint8_t dataSet[4] = {0, };
    dataSet[0] = accessID_H;
    dataSet[1] = accessID_L;
    dataSet[2] = data1;
    dataSet[3] = data2;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::IO_CONTROL_BY_ID, cmd, cmd2, dataSet, len);

    return false;
}

bool diagReadData(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD READ DATA BY ID \n");
    printMessage("    ex) cmd[0x22], cmd2[0x00],DID(2byte), \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0U;
    uint8_t cmd2 = 0U;

    uint8_t* buf = diagBridgeDataSet(cmd, cmd2, argv);
    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::READ_DATA_BY_ID, cmd, cmd2, buf,3);

    return false;
}

#ifdef DIAG_SUPPORT_STORE_TSIM_PROFILE

static DiagSIMProfile_Type slddsimProfileInfo;

bool diagGetTSIMProfile(int32_t argc, char **argv)
{
    bool ret=true;
    int32_t retVal = 0;

    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD GET TSIM PROFILE \n");
    printMessage("-------------------------------------------------------\n");
    FILE* Fp = fopen(sldd_SIM_PROFILE_PATH, "r");
    if (Fp == NULL) {
        printMessage("============ LoadSIMProfile : File is not exist. ============\n");
        //clear slddsimProfileInfo buffer
        memset(&slddsimProfileInfo,'\0',sizeof(DiagSIMProfile_Type));
        ret = false;
    } else {
        //mPower.acquire();
        uint16_t size = sizeof(slddsimProfileInfo);
        char dtcData[(DIAG_SIM_PROFILE_BUFFER_SIZE*8)] = { 0, };
        printMessage("============ LoadSIMProfile : start ============\n");
        retVal = fread((char*) &dtcData[0], 1, size, Fp);
        fclose(Fp);
        printMessage("============ LoadSIMProfile : end   ============\n");
        memcpy((char*) (&slddsimProfileInfo), (char*) &dtcData[0], size);
        if(slddsimProfileInfo.profileAllFilled==DIAG_SIM_PROFILE_FILLED){
            printMessage("imei      : [%s]\n", (char*)&slddsimProfileInfo.imei[0]);
            printMessage("imsi1     : [%s]\n", (char*)&slddsimProfileInfo.imsi1[0]);
            printMessage("msisdn1   : [%s]\n", (char*)&slddsimProfileInfo.msisdn1[0]);
            printMessage("iccid1    : [%s]\n", (char*)&slddsimProfileInfo.iccid1[0]);
            printMessage("imsi2     : [%s]\n", (char*)&slddsimProfileInfo.imsi2[0]);
            printMessage("msisdn2   : [%s]\n", (char*)&slddsimProfileInfo.msisdn2[0]);
            printMessage("iccid2    : [%s]\n", (char*)&slddsimProfileInfo.iccid2[0]);
        }else{
            printMessage("imei      : [%s]\n", (char*)&slddsimProfileInfo.imei[0]);
            if(slddsimProfileInfo.profile1Filled==DIAG_SIM_PROFILE_FILLED){
                printMessage("imsi1     : [%s]\n", (char*)&slddsimProfileInfo.imsi1[0]);
                printMessage("msisdn1   : [%s]\n", (char*)&slddsimProfileInfo.msisdn1[0]);
                printMessage("iccid1    : [%s]\n", (char*)&slddsimProfileInfo.iccid1[0]);
            }
            if(slddsimProfileInfo.profile2Filled==DIAG_SIM_PROFILE_FILLED){
                printMessage("imsi2     : [%s]\n", (char*)&slddsimProfileInfo.imsi2[0]);
                printMessage("msisdn2   : [%s]\n", (char*)&slddsimProfileInfo.msisdn2[0]);
                printMessage("iccid2    : [%s]\n", (char*)&slddsimProfileInfo.iccid2[0]);
            }
        }
        ret = true;
    }
    return ret;
}
#endif /*DIAG_SUPPORT_STORE_TSIM_PROFILE*/

bool diagSetDTCNoti(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD SET DTC NOTI \n");
    printMessage("    ex) cmd[DIAG_SLDD_DTC_SET_CMD(0x18)], cmd2[NULL], Index(1byte), Status(1byte), Count(4bytes) \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = DIAG_SLDD_DTC_SET_CMD;
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);

    uint8_t Index = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t Status = (uint8_t)strtol(argv[3], &endptr, 16);

    uint8_t len = DIAG_SLDD_DTC_SET_PARAMETER_SIZE;
    uint8_t dataSet[DIAG_SLDD_DTC_SET_PARAMETER_SIZE] = {0, };

    dataSet[0] = Index;
    dataSet[1] = Status;
    dataSet[2] = (uint8_t)strtol(argv[4], &endptr, 16);
    dataSet[3] = (uint8_t)strtol(argv[5], &endptr, 16);
    dataSet[4] = (uint8_t)strtol(argv[6], &endptr, 16);
    dataSet[5] = (uint8_t)strtol(argv[7], &endptr, 16);

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::READ_DTC_INFO, cmd, cmd2, dataSet, len);

    return false;
}

bool diagReadDTCInfo(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD READ DTC INFO \n");
    printMessage("    ex) cmd[NULL], cmd2[NULL], Index(1byte), Status(1byte), Count(4bytes) \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);

    uint8_t Index = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t Status = (uint8_t)strtol(argv[3], &endptr, 16);

    uint8_t len = 1;
    uint8_t dataSet[1] = {0, };

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::READ_DTC_INFO, cmd, cmd2, dataSet, len);

    return false;
}


bool diagRoutineControl(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD ROUTINE Control \n");
    printMessage("    ex) cmd[0x31], cmd2[0x01/0x02/0x03/5/0x81/0x82/0x83/0x84/0x85], DID(2byte) \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);
//  uint8_t option = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t accessID_H = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t accessID_L = (uint8_t)strtol(argv[3], &endptr, 16);

    static uint8_t len = 3;
    static uint8_t dataSet[3] = {0, };

    dataSet[0] = accessID_H;
    dataSet[1] = accessID_L;
    dataSet[2] = (uint8_t)strtol(argv[4], &endptr, 16);

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::ROUTINE_CONTROL, cmd, cmd2, dataSet, len);

    return false;
}

bool diagSessionControl(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD SESSION Control \n");
    printMessage("    ex) cmd[0x10], cmd2[0x01/0x02/0x03/0x40/0x81/0x82/0x83/0x84/0xC0],payload[0x00] \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;

    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);
    uint8_t option = (uint8_t)strtol(argv[2], &endptr, 16);
//  uint8_t accessID_H = (uint8_t)strtol(argv[3], &endptr, 16);
//  uint8_t accessID_L = (uint8_t)strtol(argv[4], &endptr, 16);

    static uint8_t len = 1;
    static uint8_t dataSet[1] = {0, };
    dataSet[0] = option;
//  dataSet[1] = accessID_H;
//  dataSet[2] = accessID_L;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::SESSION_CONTROL, cmd, cmd2, dataSet, len);

    return false;
}

bool diagTesterPresent(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD TESTER PRESENT \n");
    printMessage("    ex) cmd[0x3E], cmd2[0x00/0x80],payload[0x00] \n");
    printMessage("-------------------------------------------------------\n");

    uint8_t cmd = 0;
    uint8_t cmd2 = 0;
    char *endptr;
    cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);
    uint8_t option = (uint8_t)strtol(argv[2], &endptr, 16);

    static uint8_t len = 1;
    static uint8_t dataSet[1] = {0, };
    dataSet[0] = option;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::TESTER_PRESENT, cmd, cmd2, dataSet, len);
    return false;
}

bool diagWriteData(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD WRITE BY ID \n");
    printMessage("    ex) cmd[0x2E], cmd2[0x00], payload[DID] \n");
    printMessage("-------------------------------------------------------\n");

    char *endptr;
    uint8_t cmd = (uint8_t)strtol(argv[0], &endptr, 16);
    uint8_t cmd2 = (uint8_t)strtol(argv[1], &endptr, 16);

    uint8_t accessID_H = (uint8_t)strtol(argv[2], &endptr, 16);
    uint8_t accessID_L = (uint8_t)strtol(argv[3], &endptr, 16);

    static uint8_t dataSet[6] = {0, };
    static uint8_t len = 6;
    dataSet[0] = accessID_H;
    dataSet[1] = accessID_L;

    if((dataSet[0]==0xD9)&&(dataSet[1]==0x07)){
        dataSet[2] = (uint8_t)strtol(argv[4], &endptr, 16);
        dataSet[3] = (uint8_t)strtol(argv[5], &endptr, 16);
        dataSet[4] = (uint8_t)strtol(argv[6], &endptr, 16);
        dataSet[5] = (uint8_t)strtol(argv[7], &endptr, 16);
        len=6;
    }

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::WRITE_DATA_BY_ID, cmd, cmd2, dataSet, len);

    return false;
}

bool diagTUstatus(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD TU STATUS \n");
    printMessage("-------------------------------------------------------\n");

    DiagSlddBridge::getInstance().slddCmdStatusInfo(TU_STATUS);

    return false;
}

bool diagBubstatus(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD BUB STATUS \n");
    printMessage("-------------------------------------------------------\n");

    DiagSlddBridge::getInstance().slddCmdStatusInfo(BUB_STATUS);
    return false;
}

bool diagSpeakerstatus(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD SPEAKER STATUS \n");
    printMessage("-------------------------------------------------------\n");

    DiagSlddBridge::getInstance().slddCmdStatusInfo(SPEAKER_STATUS);
    return false;
}

bool diagMicstatus(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD MIC STATUS \n");
    printMessage("-------------------------------------------------------\n");

    DiagSlddBridge::getInstance().slddCmdStatusInfo(MIC_STATUS);
    return false;
}

bool diagExtbattstatus(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD EXT BATTERY STATUS \n");
    printMessage("-------------------------------------------------------\n");

    DiagSlddBridge::getInstance().slddCmdStatusInfo(EXT_BAT_STATUS);
    return false;
}

void register_diag()
{
    registerCommands(MODULE_DIAG_MANAGER, NULL, diag_cmdTable);
}

bool diagCmdMcuWrite(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD CMD MCU WRITE \n");
    printMessage("    ex) cmd[0x01], cmd2[0xF1], payload[16byte] \n");
    printMessage("-------------------------------------------------------\n");

    char *endptr;
    uint8_t cmd = 0x01;
    uint8_t cmd2 = 0xF1;
    uint8_t dataSet[16+1] = {0, };
    uint8_t i = 0 ;
    uint8_t len = 16 ;

    for (i = 0 ; i < 16 ; i++)
    {
        if (argv[i] == NULL)
            break;
        dataSet[i] = (uint8_t)strtol(argv[i], &endptr, 16);
    }

    printMessage("data:[0x%X][0x%X][0x%X][0x%X][0x%X][0x%X][0x%X][0x%X][0x%X][0x%X]",
        dataSet[0], dataSet[1], dataSet[2], dataSet[3], dataSet[4],
        dataSet[5], dataSet[6], dataSet[7], dataSet[8], dataSet[9]);
    printMessage("data:[0x%X][0x%X][0x%X][0x%X][0x%X][0x%X]",
        dataSet[10], dataSet[11], dataSet[12], dataSet[13], dataSet[14], dataSet[15]);

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::SLDD_CMD_MCU, cmd, cmd2, dataSet, len);

    return false;
}
bool diagCmdMcuRead(int32_t argc, char **argv)
{
    printMessage("-------------------------------------------------------\n");
    printMessage("    DIAG SLDD CMD MCU WRITE \n");
    printMessage("    ex) cmd[0x01], cmd2[0xF0] \n");
    printMessage("-------------------------------------------------------\n");

    char *endptr;
    uint8_t cmd = 0x01;
    uint8_t cmd2 = 0xF0;
    uint8_t dataSet[1] = {0, };
    uint8_t i = 0 ;
    uint8_t len = 0 ;

    DiagSlddBridge::getInstance().slddCmdInnerServiceToLen(DiagService::SLDD_CMD_MCU, cmd, cmd2, dataSet, len);

    return false;
}

Diag_Event_TimeInfo sldddiagTimeInfo;

bool slddDiagSetBUBTestWaitTime(int32_t argc, char **argv)
{
    char *endptr;
    uint8_t hour = (uint8_t)strtol(argv[0], &endptr, 10);
    uint8_t minute = (uint8_t)strtol(argv[1], &endptr, 10);
    uint8_t seconds = (uint8_t)strtol(argv[2], &endptr, 10);

    static uint32_t setTime = 0;
    static uint32_t changedWiatTime = 0;
    static uint32_t currentTime=0;
    static uint32_t tempTime=0;
    printMessage("slddDiagSetBUBTestWaitTime()\n");

    DiagLoadTimeInfo();
    changedWiatTime = (hour*60*60) + (minute*60) + seconds;
    printMessage("-------------------------------------------------------\n");
    printMessage("    CMD SET BUBTEST WAITED TIME (%d/Hr, %d/Min, %d/sec => convert to seconds [%d].)\n",hour,minute,seconds,changedWiatTime);
    printMessage("    How long did you wait, after testing BUB ? => answer is [%d] seconds.)\n",changedWiatTime);
    printMessage("-------------------------------------------------------\n");

    // change BUB test start time.
    currentTime = getCurrentTime();
    tempTime = sldddiagTimeInfo.BUBTestTime;//backup BUB test start time
    setTime = currentTime - changedWiatTime;
    Diag_SetTimeInfo(BUB_TEST_TIME_INDEX,setTime);

    printMessage("Current time(sec) is [%d]\n",currentTime);
    printMessage("Changed start time of BUB test from [%d] to [%d]\n",tempTime,sldddiagTimeInfo.BUBTestTime);

    return true;

}


time_t getCurrentTime()
{
    time_t seconds  = time(NULL); /* get current time */
//    if (seconds < 0) {
//        seconds = 0;
//    }
    return seconds;
}

uint32_t compareTime(time_t refTime)
{
    uint32_t diffSeconds=0;
    time_t currentTime  = time(NULL); /* get current time */
    diffSeconds = difftime(currentTime, refTime);
    return diffSeconds;
}

error_t Diag_SetTimeInfo(uint8_t index, time_t timesec )
{

    FILE* Fp = fopen(sldd_TIME_INFO_PATH, "w");

    if (Fp == NULL) {
        LOGE("Can't save Simprofile, because File pointer is NULL");
        return E_ERROR;
    } else {
        if(index==DRIVING_CYCLE_COUNT_INDEX){
            if(timesec==NULL){
                sldddiagTimeInfo.drivingCycleCount = 0;
            }else if (sldddiagTimeInfo.drivingCycleCount<255)
                sldddiagTimeInfo.drivingCycleCount++;
            else
                sldddiagTimeInfo.drivingCycleCount=255;

        }else if(index==SERVICE_START_TIME_INDEX){
            sldddiagTimeInfo.serviceStartTime=timesec;
        }else if(index==LEDBUTTON_CHECK_TIME_INDEX){
            sldddiagTimeInfo.LEDButtonTestTime=timesec;
        }else if(index==BUB_TEST_TIME_INDEX){
            sldddiagTimeInfo.BUBTestTime=timesec;
        }else if(index==TCU3_RESET_TIME_INDEX){
            sldddiagTimeInfo.tcuResetTime=timesec;
//        }else if(index==CURRENT_WAKEUP_PTR_INDEX){
//            sldddiagTimeInfo.currentStoredPtr=timesec;
        }else if(index==WAKEUP_TIME_INDEX){
            sldddiagTimeInfo.wakeupTime[sldddiagTimeInfo.currentStoredPtr]=timesec;
            sldddiagTimeInfo.currentStoredPtr++;
            if(sldddiagTimeInfo.currentStoredPtr>100)
                sldddiagTimeInfo.currentStoredPtr = 0;
        }else {
            LOGV("Incorrect time index[%d]",index);
        }
        uint16_t size = sizeof(Diag_Event_TimeInfo);
        char save_ptr[DIAG_DTC_BUFFER_MAX/100]={0,};
        memcpy((char*) &save_ptr[0], (char*) (&sldddiagTimeInfo), size);
        fwrite((char*) &save_ptr[0], 1, size, Fp);
    }
    fclose(Fp);

    LOGV("Diag_SetTimeInfo complete.");

    //mPower.release();
    return E_OK;
}

error_t DiagLoadTimeInfo()
{
    error_t ret=E_OK;
    int32_t retVal = 0;

    FILE* Fp = fopen(sldd_TIME_INFO_PATH, "r");
    if (Fp == NULL) {
        LOGE("============ LoadTimeInfo : File is not exist. ============");
        //clear sldddiagTimeInfo buffer
        memset(&sldddiagTimeInfo,'\0',sizeof(Diag_Event_TimeInfo));
        ret = E_ERROR;
    } else {
        //mPower.acquire();
        uint16_t size = sizeof(Diag_Event_TimeInfo);
        char dtcData[1000] = { 0, };
        LOGI("============ LoadTimeInfo : start ============");
        retVal = fread((char*) &dtcData[0], 1, size, Fp);
        fclose(Fp);
        LOGI("============ LoadTimeInfo : end   ============");
        memcpy((char*) (&sldddiagTimeInfo), (char*) &dtcData[0], size);
        ret = E_OK;
    }
    return ret;
}


