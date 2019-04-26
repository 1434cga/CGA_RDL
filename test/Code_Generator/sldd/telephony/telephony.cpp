
#include <sys/stat.h>
#include <utils/SLLooper.h>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "services/TelephonyManagerService/opt/Call.h"
#include "man.h"
#include "sldd_common.h"
#include "telephony.h"
#include "telephony/telephony_common.h"

#define LOG_TAG "sldd_telephony"

using namespace RILConstants;
using namespace std;
using mindroid::String;
using mindroid::StringWrapper;

enum Commands {
    CMD_GET_IMEI = 1,
    CMD_GET_IMEI_SV,
    CMD_GET_SW_VERSION,
    CMD_GET_NAD_PART_NUMBER,
    CMD_GET_HW_COUNTRY_ID,
    CMD_GET_SW_COUNTRY_ID,
    CMD_DIAL,
    CMD_END_CALL,
    CMD_ANSWER_RINGING_CALL,
    CMD_REJECT_CALL,
    CMD_SEND_DTMF,
    CMD_START_DTMF,
    CMD_STOP_DTMF,
    CMD_GET_CALL_STATE,
    CMD_IS_RADIO_ON,
    CMD_SET_RADIO,
    CMD_SET_RADIO_POWER,
    CMD_GET_NETWORK_OPERATOR_NAME,
    CMD_GET_NETWORK_OPERATOR,
    CMD_IS_NETWORK_ROAMING,
    CMD_GET_NETWORK_COUNTRY_ISO,
    CMD_GET_NETWORK_TYPE,
    CMD_GET_DATA_NETWORK_TYPE,
    CMD_GET_VOICE_NETWORK_TYPE,
    CMD_GET_NETWORK_CLASS,
    CMD_GET_NETWORK_TYPE_NAME,
    CMD_GET_PREFERRED_NETWORK_TYPE,
    CMD_SET_PREFERRED_NETWORK_TYPE,
    CMD_GET_AVAILABLE_NETWORKS,
    CMD_SET_NETWORK_SELECTION_MODE_AUTOMATIC,
    CMD_SELECT_NETWORK_MANUALLY,
    CMD_GET_NETWORK_SELECTION_MODE,
    CMD_GET_NETWORK_REG_STATE,
    CMD_GET_MCC,
    CMD_GET_MNC,
    CMD_GET_LAC,
    CMD_GET_CID,
    CMD_GET_RX_LEVEL,
    CMD_GET_SIGNAL_STRENGTH,
    CMD_GET_SNR,
    CMD_SET_BAND_MODE,
    CMD_GET_BAND_MODE,
    CMD_GET_SERVICE_STATE,
    CMD_HAS_SIM_CARD,
    CMD_IS_SIM_PIN_ENABLED,
    CMD_CHANGE_SIM_LOCK_STATE,
    CMD_CHANGE_PIN,
    CMD_SUPPLY_PIN,
    CMD_SUPPLY_PUK,
    CMD_SUPPLY_PIN_REPORT_RESULT,
    CMD_SUPPLY_PUK_REPORT_RESULT,
    CMD_GET_SIM_REMAINING_COUNT,
    CMD_GET_SIM_OPERATOR,
    //CMD_GET_SIM_OPERATOR_NAME,
    CMD_GET_SIM_COUNRTY_ISO,
    CMD_GET_ICCID,
    CMD_GET_EID,
    CMD_GET_IMSI,
    CMD_GET_MSISDN,
    CMD_GET_SIM_STATE,
    CMD_GET_SIM_PROFILE_COUNT,
    CMD_GET_SIM_PROFILE_ID,
    CMD_GET_SIM_PROFILE_TYPE,
    CMD_GET_ACTIVE_SIM_PROFILE,
    CMD_SET_ACTIVE_SIM_PROFILE,    
    CMD_OPEN_SIM_LOGICAL_CHANNEL,
    CMD_CLOSE_SIM_LOGICAL_CHANNEL,
    CMD_TRANSMIT_APDU_LOGICAL_CHANNEL,
    CMD_TRANSMIT_APDU_BASIC_CHANNEL,
    CMD_GET_TCU_POWER_MODE,
    CMD_SET_TCU_POWER_MODE,
    CMD_GET_TCU_OPERATION_MODE,
    CMD_SET_TCU_OPERATION_MODE,
    CMD_GET_TCU_SPY_MODE,
    CMD_SET_TCU_SPY_MODE,
    CMD_CHECK_RF_CALIBRATION,
    CMD_SET_ACTIVE_SIM,
    CMD_GET_ACTIVE_SIM,
    CMD_GET_PSIM_STATE,
    CMD_GET_SIM_SWITCHING_CAUSE,
    CMD_GET_PSIM_BLOCK_STATE,
    CMD_CLEAR_PSIM_BLOCK_STATE,
    CMD_ACQUIRE_PSIM_LOCK,
    CMD_RELEASE_PSIM_LOCK,
    CMD_GET_PSIM_LOCK_STATE,
    CMD_ACQUIRE_TSIM_LOCK,
    CMD_RELEASE_TSIM_LOCK,
    CMD_GET_TSIM_LOCK_STATE,
    CMD_SET_TOPS_STATUS,
    CMD_GET_TOPS_STATUS,
    CMD_GET_TOPS_NUMBER,
    CMD_ECALL_INDICATION_REGI,
    CMD_ECALL_START,
    CMD_ECALL_SET_MSD,
    CMD_ECALL_SET_CONFIG,
    CMD_ECALL_STOP,
    CMD_ECALL_ANSWER,
    CMD_SET_CALLBACK_STATE,
    CMD_GET_JAMMING_DETECTION_STATUS,
    CMD_SET_DIVERSITY_ANTENNA,
    CMD_GET_TEMPERATURE,
    CMD_GET_IMSA_SERVICE_STATUS,
    CMD_GET_IMSA_REGISTRATION_STATUS,
    CMD_GET_IMSA_PDP_STATUS,
    //--------------------------------
    // hidden commands
    CMD_DUMP,
    CMD_GET_FRAT_TIMER,
    CMD_SET_FRAT_TIMER,
    CMD_LISTEN,
    CMD_FACTORY_GET_RFCAL_CHECK,
    CMD_FACTORY_GET_DATA_CHECK,
    CMD_FACTORY_SET_DATA_CHECK,
    CMD_FACTORY_GET_IMEI,
    CMD_FACTORY_SET_IMEI,
    CMD_FACTORY_READ_QFUSING,
    CMD_FACTORY_WRITE_QFUSING,
    CMD_FACTORY_GET_FID,
    CMD_FACTORY_SET_FID,
    CMD_FACTORY_SET_SW_COUNTRY_ID,
    CMD_FACTORY_SET_EFS_RECOVERY,
    CMD_GET_CALL_REJECT_CLEARED,
    CMD_SET_CALL_REJECT_CLEARED,
    CMD_GET_AUDIO_ENABLED,
    CMD_SET_AUDIO_ENABLED,
};

sldd_cmd_table_t telephonyCommands[] = {
    { "getImei", 0, commandActionTelephony, "getImei - Get IMEI\n" },
    { "getImeiSv", 0, commandActionTelephony, "getImeiSv - Get IMEI/SV\n" },
    { "getSwVer", 0, commandActionTelephony, "getSwVer - Get nad sw version\n" },
    { "getNadPartNumber", 0, commandActionTelephony, "getNadPartNumber - Get nad part number\n" },
    { "getHwCountryId", 0, commandActionTelephony, "getRegison - Get HW country id\n" },
    { "getSwCountryId", 0, commandActionTelephony, "getSwCountryId - Get SW country id\n" },

    { "dial", 0, commandActionTelephony, "dial <number> - Dial\n" },
    { "endCall", 0, commandActionTelephony, "endCall - End call\n" },
    { "answerCall", 0, commandActionTelephony, "answerCall - Answer ringing call\n" },
    { "rejectCall", 0, commandActionTelephony, "rejectCall - Reject call\n" },
    { "sendDtmf", 0, commandActionTelephony, "sendDtmf <digit> - Send DTMF\n" },
    { "startDtmf", 0, commandActionTelephony, "startDtmf <digit> - Start DTMF\n" },
    { "stopDtmf", 0, commandActionTelephony, "stopDtmf - Stop DTMF\n" },
    { "getCallState", 0, commandActionTelephony, "getCallState - Get call state\n" },

    { "isRadioOn", 0, commandActionTelephony, "isRadioOn - Return true if radio state is on\n" },
    { "setRadio", 0, commandActionTelephony, "setRadio (on | off) - Set the radio to on or off\n" },
    { "setRadioPower", 0, commandActionTelephony, "setRadioPower (on | off) - Set the radio to on or off unconditionally\n" },
    { "getNetOperName", 0, commandActionTelephony, "getNetOperName - Get the alphabetic name of current registered operator\n" },
    { "getNetOper", 0, commandActionTelephony, "getNetOper - Get the numeric name (MCC+MNC) of current registered operator\n" },
    { "isNetRoaming", 0, commandActionTelephony, "isNetRoaming - Return true if the device is considered roaming on the current network\n" },
    { "getNetCountryIso", 0, commandActionTelephony, "getNetCountryIso - Get network country iso\n" },
    { "getNetType", 0, commandActionTelephony, "getNetType - Get network type\n" },
    { "getDataNetType", 0, commandActionTelephony, "getDataNetType - Get network type for data \n" },
    { "getVoiceNetType", 0, commandActionTelephony, "getVoiceNetType - Get network type for voice \n" },
    { "getNetClass", 0, commandActionTelephony, "getNetClass - Get network class\n" },
    { "getNetTypeName", 0, commandActionTelephony, "getNetTypeName - Get network type name\n" },
    { "getPrefNetType", 0, commandActionTelephony, "getPrefNetType - Get preferred network type \n" },
    { "setPrefNetType", 0, commandActionTelephony, "setPrefNetType <type> - Set preferred network type \n" },
    { "getAvailNets", 0, commandActionTelephony, "getAvailNets [timeout] - Get available networks (Default timeout: 180 seconds)\n" },
    { "setNetSelModeAuto", 0, commandActionTelephony, "setNetSelModeAuto - Switch network selection mode to automatic\n" },
    { "selNetManual", 0, commandActionTelephony, "selNetManual <MccMnc> - Manually select a network\n" },
    { "getNetSelMode", 0, commandActionTelephony, "getNetSelMode - Get network selection mode\n" },
    { "getNetRegState", 0, commandActionTelephony, "getNetRegState - Get network registration state (0 ~ 6)\n" },
    { "getMcc", 0, commandActionTelephony, "getMcc - Get mobile country code (0 ~ 999)\n" },
    { "getMnc", 0, commandActionTelephony, "getMnc - Get mobile network code (0 ~ 999)\n" },
    { "getLac", 0, commandActionTelephony, "getLac - Get location area code (0 ~ 65535)\n" },
    { "getCid", 0, commandActionTelephony, "getCid - Get cell identity (GSM:0 ~ 65535, LTE:0 ~ 268435455)\n" },
    { "getRxLevel", 0, commandActionTelephony, "getRxLevel - Get the signal receive level (0~31, 99-Unknown)\n" },
    { "getSignalStrength", 0, commandActionTelephony, "getSignalStrength - Get the signal strength as dBm\n" },
    { "getSnr", 0, commandActionTelephony, "getSnr - Get the signal to noise ratio\n" },
    { "setBandMode", 0, commandActionTelephony, "setBandMode - Set band mode <0|1>\n" },
    { "getBandMode", 0, commandActionTelephony, "getBandMode - Get band mode\n" },
    { "getServiceState", 0, commandActionTelephony, "getServiceState [type] - Get service state of voice or data registration\n" },

    { "hasSimCard", 0, commandActionTelephony, "hasSimCard - Return true if the sim card is present\n" },
    { "isSimPinEnabled", 0, commandActionTelephony, "isSimPinEnabled - Return true if sim pin lock is enabled\n" },
    { "changeSimLockState", 0, commandActionTelephony, "changeSimLockState (on|off) <Pin> - Change sim lock\n" },
    { "changePin", 0, commandActionTelephony, "changePin <OldPin> <NewPin> - Change sim pin code\n" },
    { "supplyPin", 0, commandActionTelephony, "supplyPin <Pin> - Supply a pin to unlock the sim\n" },
    { "supplyPuk", 0, commandActionTelephony, "supplyPuk <Puk> <NewPin> - Supply puk to unlock the sim and set sim pin to new pin\n" },
    { "supplyPinRP", 0, commandActionTelephony, "supplyPinRP <Pin> - Supply a pin to unlock the sim\n" },
    { "supplyPukRP", 0, commandActionTelephony, "supplyPukRP <Puk> <Pin> - Supply puk to unlock the sim and set sim pin to new pin\n" },

    { "getSimRemainingCount", 0, commandActionTelephony, "getSimRemainingCount <type> - Get remaining count of SIM\n" },

    { "getSimOper", 0, commandActionTelephony, "getSimOper - Get sim operator numeric (MCC/MNC)\n" },
    //{ "getSimOperName", 0, commandActionTelephony, "getSimOperName - Get sim operator name\n" },
    { "getSimCountryIso", 0, commandActionTelephony, "getSimCountryIso - Get sim country ISO\n" },
    { "getIccid", 0, commandActionTelephony, "getIccid - Get sim serial number (ICCID)\n" },
    { "getEid", 0, commandActionTelephony, "getEid - Get EUICC serial number (EID)\n" },
    { "getImsi", 0, commandActionTelephony, "getImsi - Get subscriber id (IMSI)\n" },
    { "getMsisdn", 0, commandActionTelephony, "getMsisdn - Get MSISDN\n" },
    { "getSimState", 0, commandActionTelephony, "getSimState - Get sim state\n" },

    { "getSimProfCount", 0, commandActionTelephony, "getSimProfCount - Get sim profile count\n" },
    { "getSimProfId", 0, commandActionTelephony, "getSimProfId <ProfileType> - Get sim profile id\n" },
    { "getSimProfType", 0, commandActionTelephony, "getSimProfType <ProfileId> - Get sim profile type\n" },
    { "getActSimProf", 0, commandActionTelephony, "getActSimProf - Get active sim profile\n" },
    { "setActSimProf", 0, commandActionTelephony, "setActSimProf <ProfileId> - Set active sim profile\n" },
    { "openSimLCh", 0, commandActionTelephony, "openSimLCh - Open sim logical channel <aid>\n" },
    { "closeSimLCh", 0, commandActionTelephony, "closeSimLCh - Close sim logical channel <channelId>\n" },
    { "transApduLogical", 0, commandActionTelephony, "transApduLogical - TRANSMIT APDU LOGICAL CHANNEL <channel cla command p1 p2 p3 data>\n" },
    { "transApduBasic", 0, commandActionTelephony, "transApduBasic - TRANSMIT APDU BASIC CHANNEL <cla command p1 p2 p3 data>\n" },

    { "getPowerMode", 0, commandActionTelephony, "getPowerMode - Get TCU power mode\n" },
    { "setPowerMode", 0, commandActionTelephony, "setPowerMode <mode> - Set TCU power mode\n" },
    { "getOperMode", 0, commandActionTelephony, "getOperMode - Get TCU operation mode\n" },
    { "setOperMode", 0, commandActionTelephony, "setOperMode <mode> - Set TCU operation mode\n" },
    { "getSpyMode", 0, commandActionTelephony, "getSpyMode - Get TCU spy mode\n" },
    { "setSpyMode", 0, commandActionTelephony, "setSpyMode <mode> - Set TCU spy mode\n" },

    { "checkRfCal", 0, commandActionTelephony, "checkRfCal - Check RF calibration\n" },

    { "setActSim", 0, commandActionTelephony, "setActSim <sim> <priority> <cause> - Set active sim\n" },
    { "getActSim", 0, commandActionTelephony, "getActSim - Get active sim\n" },
    { "getPSimState", 0, commandActionTelephony, "getPSimState - Get PSIM state\n" },

    { "getSimSwitchingCause", 0, commandActionTelephony, "getSimSwitchingCause - Get SIM switching cause\n" },
    { "getPsimBlockState", 0, commandActionTelephony, "getPsimBlockState - Get PSIM block state\n" },
    { "clearPsimBlockState", 0, commandActionTelephony, "clearPsimBlockState - Clear PSIM block state\n" },

    { "acquirePsimLock", 0, commandActionTelephony, "acquirePsimLock - Acquire PSIM lock\n" },
    { "releasePsimLock", 0, commandActionTelephony, "releasePsimLock - Release PSIM lock\n" },
    { "getPsimLockState", 0, commandActionTelephony, "getPsimLockState - Get PSIM lock state\n" },

    { "acquireTsimLock", 0, commandActionTelephony, "acquireTsimLock - Acquire TSIM lock\n" },
    { "releaseTsimLock", 0, commandActionTelephony, "releaseTsimLock - Release TSIM lock\n" },
    { "getTsimLockState", 0, commandActionTelephony, "getTsimLockState - Get TSIM lock state\n" },

    { "setTopsStatus", 0, commandActionTelephony, "setTopsStatus <status> - Set TOPS status\n" },
    { "getTopsStatus", 0, commandActionTelephony, "getTopsStatus - Get TOPS status\n" },
    { "getTopsNumber", 0, commandActionTelephony, "getTopsNumber <type> - Get TOPS count of type\n" },

    { "eCallRegi", 0, commandActionTelephony, "eCallRegi <hexstring> - eCall indication regi\n" },
    { "eCallStart", 0, commandActionTelephony, "eCallStart <hexstring> - eCall start\n" },
    { "eCallSetMsd", 0, commandActionTelephony, "eCallSetMsd <hexstring> - eCall set MSD\n" },
    { "eCallSetConfig", 0, commandActionTelephony, "eCallSetConfig <hexstring> - eCall set config\n" },
    { "eCallStop", 0, commandActionTelephony, "eCallStop <hexstring> - eCall stop\n" },
    { "eCallAnswer", 0, commandActionTelephony, "eCallAnswer <hexstring> - eCall answer\n" },
    { "setCallbackState", 0, commandActionTelephony, "setCallbackState <state> - Set callback stat\n" },

    { "getJammingDetectionStatus", 0, commandActionTelephony, "getJammingDetectionStatus - Get jamming detection status\n" },
    { "setDiversityAntenna", 0, commandActionTelephony, "setDiversityAntenna <divAnt> - Set diversity antenna\n" },

    { "getTemperature", 0, commandActionTelephony, "getTemperature - Get temperature\n" },

    { "getIMSAServiceStatus", 0, commandActionTelephony, "getIMSAServiceStatus - Get IMS sevice status\n" },
    { "getIMSARegistrationStatus", 0, commandActionTelephony, "getIMSARegistrationStatus - Get IMS registration status\n" },
    { "getIMSAPDPStatus", 0, commandActionTelephony, "getIMSAPDPStatus - Get IMS pdp status\n" },

    { NULL, NULL, NULL, NULL}
};

//-----------------------------------------------------------------------------

static void usage_telephony(int32_t id);
static int32_t getCommandId(char* string);
static int32_t getHiddenCommandId(char* string);

static const char getDtmfDigit(char* string);

static const char* getCallStateString(int32_t state);
static const char* getNetworkTypeString(int32_t type);
static const char* getNetworkClassString(int32_t networkClass);
static const char* getPreferredNetworkTypeString(int32_t type);
static const char* getNetRegStateString(int32_t state);
static const char* getServiceStateString(int32_t state);
static const char* getSimStateString(int32_t state);
static const char* getSimProfileIdString(int32_t id);
static const char* getSimProfileTypeString(int32_t type);

static const char* getPowerModeString(int32_t mode);
static const char* getOperationModeString(int32_t mode);
static const char* getSpyModeString(int32_t mode);

static const char* getPinReturnResultString(int32_t result);
static const char* getSimRemainingCountTypeString(int32_t type);

static const char* getSetActiveSimResultString(int32_t result);
static const char* getGetActiveSimResultString(int32_t activeSim);
static const char* getPsimStateString(int32_t state);
static const char* getSimSwitchingCauseString(int32_t cause);

static const char* getDataConnectionStateString(int32_t state);
static const char* getDataActivityString(int32_t state);

static const char* getHwCountryIdString(int32_t hwId);
static const char* getSwCountryIdString(int32_t swId);

static const char* getJammingDetectionStatusString(int32_t status);
static const char* getTemperatureResultString(int32_t result);


static int32_t calcImeiChecksum(const char* imei);

//-----------------------------------------------------------------------------

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

class MyNetworkQueryServiceCallback : public BnNetworkQueryServiceCallback
{
public:
    MyNetworkQueryServiceCallback(sp<Waiter> waiter) : mWaiter(waiter) {
    }

    virtual void onQueryComplete(Vector< sp<OperatorInfo> > networkInfoArray, int32_t status) {
        if (status != 0) {
            printMessage(" Failed (%d)\n", status);
            mWaiter->setResult(1);
            mWaiter->notifyAll();
            return;
        }

        printMessage(" Completed\n");
        printMessage("-------------------------------------------------------\n");
        printMessage(" -> Available networks (%d)\n", networkInfoArray.size());

        if (networkInfoArray.size() > 0UL) {
            int32_t nWidth = 0;
            int32_t sWidth = 0;
            int32_t lWidth = 0;

            Vector< sp<OperatorInfo> >::iterator itr = networkInfoArray.begin();
            while (itr != networkInfoArray.end()) {
                uint32_t nLen = ::strlen(CSTR((*itr)->getOperatorNumeric()));
                uint32_t lLen = ::strlen(CSTR((*itr)->getOperatorAlphaLong()));
                uint32_t sLen = ::strlen(CSTR((*itr)->getOperatorAlphaShort()));

                nWidth = (nWidth > nLen ? nWidth : nLen);
                sWidth = (sWidth > sLen ? sWidth : sLen);
                lWidth = (lWidth > lLen ? lWidth : lLen);

                itr++;
            }

            itr = networkInfoArray.begin();
            for (int32_t i = 1; itr != networkInfoArray.end(); i++) {
                printMessage("    %2d.", i);
                printMessage(" %-*s /", nWidth, CSTR((*itr)->getOperatorNumeric()));
                printMessage(" %-*s /", lWidth, CSTR((*itr)->getOperatorAlphaLong()));
                printMessage(" %-*s /", sWidth, CSTR((*itr)->getOperatorAlphaShort()));
                printMessage(" %-9s\n", CSTR((*itr)->getStateString()));
                itr++;
            }
        }

        mWaiter->setResult(1);
        mWaiter->notifyAll();
    }

private:
    sp<Waiter> mWaiter;
};

class MyPhoneStateListener : public PhoneStateListener
{
public:
    MyPhoneStateListener(sp<sl::SLLooper>& looper) : PhoneStateListener(looper) {
    }

    virtual void onServiceStatesChanged(const sp<ServiceState>& serviceState) {
        printMessage("### [%s] onServiceStatesChanged: serviceState = %s\n", CSTR(getCurrentTimeString()), STRING_CSTR(serviceState));
    }

    virtual void onServiceStateChanged(int32_t state) {
        printMessage("### [%s] onServiceStateChanged: state = %s\n", CSTR(getCurrentTimeString()), getServiceStateString(state));
    }

    virtual void onDataServiceStateChanged(int32_t state) {
        printMessage("### [%s] onDataServiceStateChanged: state = %s\n", CSTR(getCurrentTimeString()), getServiceStateString(state));
    }

    virtual void onCallStateChanged(int32_t state, const char* incomingNumber) {
        printMessage("### [%s] onCallStateChanged: state = %d, incomingNumber = %s\n", CSTR(getCurrentTimeString()), state, (incomingNumber != NULL ? incomingNumber : ""));
    }

    virtual void onSignalStrengthsChanged(const sp<SignalStrength>& signalStrength) {
        printMessage("### [%s] onSignalStrengthsChanged: signalStrength = %s\n", CSTR(getCurrentTimeString()), STRING_CSTR(signalStrength));
    }

    virtual void onPreciseCallStateChanged(int32_t state) {
        printMessage("### [%s] onPreciseCallStateChanged: state = %s\n", CSTR(getCurrentTimeString()), getCallStateString(state));
    }

    virtual void onDisconnect(int32_t cause) {
        printMessage("### [%s] onDisconnect: cause = %d\n", CSTR(getCurrentTimeString()), cause);
    }

    virtual void onPSimStateChanged(int32_t simAct, int32_t event, int32_t slot, int32_t errorCause) {
        printMessage("### [%s] onPSimStateChanged: simAct = %d, event = %d, slot = %d, errorCause = %d\n", CSTR(getCurrentTimeString()), simAct, event, slot, errorCause);
    }

    virtual void onActiveSimStateChanged(int32_t activeSim) {
        printMessage("### [%s] onActiveSimStateChanged: activeSim = %d\n", CSTR(getCurrentTimeString()), activeSim);
    }

    virtual void onInternalSimStateChanged(int32_t simState) {
        printMessage("### [%s] onInternalSimStateChanged: simState = %d\n", CSTR(getCurrentTimeString()), simState);
    }

    virtual void onPsimLockStateChanged(bool locked) {
        printMessage("### [%s] onPsimLockStateChanged: locked = %s\n", CSTR(getCurrentTimeString()), BOOL_STR(locked));
    }

    virtual void onTsimLockStateChanged(bool locked) {
        printMessage("### [%s] onTsimLockStateChanged: locked = %s\n", CSTR(getCurrentTimeString()), BOOL_STR(locked));
    }

    virtual void onSimSessionClosed(int32_t channel) {
        printMessage("### [%s] onSimSessionClosed: channel = %d\n", CSTR(getCurrentTimeString()), channel);
    }

    virtual void onECallStateChanged(int32_t state) {
        printMessage("### [%s] onECallStateChanged: state = %d\n", CSTR(getCurrentTimeString()), state);
    }

    virtual void onECallEraGlonassMsdTxFail(const sp<ByteArray>& bytes) {
        printMessage("### [%s] onECallEraGlonassMsdTxFail: bytes = %s\n", CSTR(getCurrentTimeString()), STRING_CSTR(bytes));
    }

    virtual void onECallMsdRequested(int32_t msdReq) {
        printMessage("### [%s] onECallMsdRequested: msdReq = %d\n", CSTR(getCurrentTimeString()), msdReq);
    }

    virtual void onECallRedialType(int32_t redialType) {
        printMessage("### [%s] onECallRedialType: redialType = %d\n", CSTR(getCurrentTimeString()), redialType);
    }

    virtual void onECallSentMsdReported(const sp<ByteArray>& bytes) {
        printMessage("### [%s] onECallSentMsdReported: bytes = %s\n", CSTR(getCurrentTimeString()), STRING_CSTR(bytes));
    }

    virtual void onECallIncallStatus(int32_t callId, int32_t callType, int32_t callStatus) {
        printMessage("### [%s] onECallIncallStatus: callId = %d, callType = %d, callStatus = %d\n", CSTR(getCurrentTimeString()), callId, callType, callStatus);
    }

    virtual void onTachyonAntennaStatus(bool result) {
        printMessage("### [%s] onTachyonAntennaStatus: result = %s\n", CSTR(getCurrentTimeString()), BOOL_STR(result));
    }

    virtual void onRequestListenMode(bool request) {
        printMessage("### [%s] onRequestListenMode: request = %s\n", CSTR(getCurrentTimeString()), BOOL_STR(request));
    }

    virtual void onJammingDetectionStatus(int32_t status) {
        printMessage("### [%s] onJammingDetectionStatus: status = %d\n", CSTR(getCurrentTimeString()), status);
    }

    virtual void onWakeupAntennaAdc(int32_t adc) {
        printMessage("### [%s] onWakeupAntennaAdc: adc = %d\n", CSTR(getCurrentTimeString()), adc);
    }

    virtual void onDataLimit(int32_t dataLimitPercnet) {
        printMessage("### [%s] onDataLimit: dataLimitPercnet = %d \n", CSTR(getCurrentTimeString()), dataLimitPercnet);
    }

    virtual void onDataActivity(int32_t direction) {
        printMessage("### [%s] onDataActivity: direction = %d [%s] \n", CSTR(getCurrentTimeString()), direction, getDataActivityString(direction));
    }

    virtual void onDataConnectionFailed(const char* reason, const char* apnType) {
        printMessage("### [%s] onDataConnectionFailed: reason = %s, apnType = %s \n", CSTR(getCurrentTimeString()), reason, apnType);
    }

    virtual void onDataConnectionStateChanged(int32_t state, int32_t networkType) {
        printMessage("### [%s] onDataConnectionStateChanged: state = %d [%s] \n", CSTR(getCurrentTimeString()), state, getDataConnectionStateString(state));
    }

private:
    sp<String> getCurrentTimeString() {
        StringWrapper s;

        std::time_t currTime = std::time(NULL);
        std::tm time = *std::gmtime(&currTime);

        s.appendFormatted("%02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);

        return new String(s.c_str());
    }
};

static sp<TelephonyManager::TsimLock> sTsimLock = new TelephonyManager::TsimLock();
static sp<TelephonyManager::PsimLock> sPsimLock = new TelephonyManager::PsimLock();


//---------------------------------------------------------------------------------

void register_telephony() {
    registerCommands(MODULE_TELEPHONY, NULL, telephonyCommands);
}

bool commandActionTelephony(int32_t argc, char **argv) {
    if (argc < 1) {
        usage_telephony(0);
        return true;
    }

    int32_t id = getHiddenCommandId(argv[0]);
    if (id == -1) {
        id = getCommandId(argv[0]);
    }

    if (id == -1) {
        printMessage("-------------------------------------------------------\n");
        printMessage(" %s : Unknown command\n", argv[0]);
        printMessage("-------------------------------------------------------\n");
        return true;
    }

    switch(id) {
        case CMD_GET_IMEI: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get IMEI\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> imei = TelephonyManager::getImei();
            printMessage(" -> IMEI : %s\n", CSTR(imei));
        } break;

        case CMD_GET_IMEI_SV: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get IMEI/SV\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> imeiSv = TelephonyManager::getImeiSv();
            printMessage(" -> IMEI/SV : %s\n", CSTR(imeiSv));
        } break;

        case CMD_GET_SW_VERSION: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get software version\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> version = TelephonyManager::getSoftwareVersion();
            printMessage(" -> Version : %s\n", CSTR(version));
        } break;

        case CMD_GET_NAD_PART_NUMBER: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get nad part number\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> partNumber = TelephonyManager::getNadPartNumber();
            printMessage(" -> Part number : %s\n", CSTR(partNumber));
        } break;

        case CMD_GET_HW_COUNTRY_ID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get HW country id\n");
            printMessage("-------------------------------------------------------\n");
            int32_t hwId = TelephonyManager::getHwCountryId();
            printMessage(" -> HW country id : %d (%s)\n", hwId, getHwCountryIdString(hwId));
        } break;

        case CMD_GET_SW_COUNTRY_ID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get SW country id\n");
            printMessage("-------------------------------------------------------\n");
            int32_t swId = TelephonyManager::getSwCountryId();
            printMessage(" -> SW country id : %d (%s)\n", swId, getSwCountryIdString(swId));
        } break;

        case CMD_DIAL: {
            if (argc < 2) {
                usage_telephony(CMD_DIAL);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Dial (number: %s)\n", argv[1]);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::dial(argv[1]);
            ::usleep(1000000);
            int32_t state = TelephonyManager::getCallState();
            printMessage(" -> Call state : %d (%s)\n", state, getCallStateString(state));
        } break;

        case CMD_END_CALL: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    End call\n");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::endCall();
            ::usleep(1000000);
            int32_t state = TelephonyManager::getCallState();
            printMessage(" -> Call state : %d (%s)\n", state, getCallStateString(state));
        } break;

        case CMD_ANSWER_RINGING_CALL: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Answer ringing call\n");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::answerRingingCall();
            ::usleep(1000000);
            int32_t state = TelephonyManager::getCallState();
            printMessage(" -> Call state : %d (%s)\n", state, getCallStateString(state));
        } break;

        case CMD_REJECT_CALL: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Reject call\n");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::rejectCall();
            ::usleep(1000000);
            int32_t state = TelephonyManager::getCallState();
            printMessage(" -> Call state : %d (%s)\n", state, getCallStateString(state));
        } break;

        case CMD_SEND_DTMF: {
            if (argc < 2) {
                usage_telephony(CMD_SEND_DTMF);
                break;
            }
            char digit = getDtmfDigit(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Send DTMF : %c\n", digit);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::sendDtmf(digit);
        } break;

        case CMD_START_DTMF: {
            if (argc < 2) {
                usage_telephony(CMD_START_DTMF);
                break;
            }
            char digit = getDtmfDigit(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Start DTMF : %c\n", digit);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::startDtmf(digit);
        } break;

        case CMD_STOP_DTMF: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Stop DTMF\n");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::stopDtmf();
        } break;

        case CMD_GET_CALL_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get call state\n");
            printMessage("-------------------------------------------------------\n");
            int32_t state = TelephonyManager::getCallState();
            printMessage(" -> Call state : %d (%s)\n", state, getCallStateString(state));
        } break;

        case CMD_IS_RADIO_ON: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Is radio on ?\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::isRadioOn();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_SET_RADIO: {
            if (argc < 2) {
                usage_telephony(CMD_SET_RADIO);
                break;
            }

            bool turnOn = true;
            if (!strcmp(argv[1], "off")) {
                turnOn = false;
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set radio : %s\n", turnOn ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setRadio(turnOn);
        } break;

        case CMD_SET_RADIO_POWER: {
            if (argc < 2) {
                usage_telephony(CMD_SET_RADIO_POWER);
                break;
            }

            bool turnOn = true;
            if (!strcmp(argv[1], "off")) {
                turnOn = false;
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set radio power : %s\n", turnOn ? "on" : "off");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setRadioPower(turnOn);
        } break;

        case CMD_GET_NETWORK_OPERATOR_NAME: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network operator name\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> networkOperatorName = TelephonyManager::getNetworkOperatorName();
            printMessage(" -> Network operator name : %s\n", CSTR(networkOperatorName));
        } break;

        case CMD_GET_NETWORK_OPERATOR: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network operator\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> networkOperator = TelephonyManager::getNetworkOperator();
            printMessage(" -> Network operator : %s\n", CSTR(networkOperator));
        } break;

        case CMD_IS_NETWORK_ROAMING: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Is network roaming ?\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::isNetworkRoaming();
            printMessage(" -> Roaming : %s\n", BOOL_STR(result));
        } break;

        case CMD_GET_NETWORK_COUNTRY_ISO: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network country ISO\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> networkCountryIso = TelephonyManager::getNetworkCountryIso();
            printMessage(" -> Network country ISO : %s\n", CSTR(networkCountryIso));
        } break;

        case CMD_GET_NETWORK_TYPE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network type\n");
            printMessage("-------------------------------------------------------\n");
            int32_t networkType = TelephonyManager::getNetworkType();
            printMessage(" -> Network type : %d (%s)\n", networkType, getNetworkTypeString(networkType));
        } break;

        case CMD_GET_DATA_NETWORK_TYPE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get data network type\n");
            printMessage("-------------------------------------------------------\n");
            int32_t networkType = TelephonyManager::getDataNetworkType();
            printMessage(" -> Network type : %d (%s)\n", networkType, getNetworkTypeString(networkType));
        } break;

        case CMD_GET_VOICE_NETWORK_TYPE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get voice network type\n");
            printMessage("-------------------------------------------------------\n");
            int32_t networkType = TelephonyManager::getVoiceNetworkType();
            printMessage(" -> Network type : %d (%s)\n", networkType, getNetworkTypeString(networkType));
        } break;

        case CMD_GET_NETWORK_CLASS: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network class\n");
            printMessage("-------------------------------------------------------\n");
            int32_t networkClass = TelephonyManager::getNetworkClass();
            printMessage(" -> Network class : %d (%s)\n", networkClass, getNetworkClassString(networkClass));
        } break;

        case CMD_GET_NETWORK_TYPE_NAME: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network type name\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> networkTypeName = TelephonyManager::getNetworkTypeName();
            printMessage(" -> Network type name : %s\n", CSTR(networkTypeName));
        } break;

        case CMD_GET_PREFERRED_NETWORK_TYPE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get preferred network type\n");
            printMessage("-------------------------------------------------------\n");
            int32_t networkType = TelephonyManager::getPreferredNetworkType();
            printMessage(" -> Network type : %d (%s)\n", networkType, getPreferredNetworkTypeString(networkType));
        } break;

        case CMD_SET_PREFERRED_NETWORK_TYPE: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                                      " sldd telephony setPrefNetType <type> \n"
                                      "-------------------------------------------------------\n"
                                      "   <type> \n"
                                      "     0   - WCDMA pref \n"
                                      "     1   - GSM only \n"
                                      "     2   - WCDMA only \n"
                                      "     3   - GSM & UMTS \n"
                                      "     4   - CDMA \n"
                                      "     5   - CDMA & no EVDO \n"
                                      "     6   - EVDO & no CDMA \n"
                                      "     7   - GLOBAL \n"
                                      "     8   - LTE, CDMA & EVDO \n"
                                      "     9   - LTE, GSM & WCDMA \n"
                                      "     10  - LTE, CDMA, EVDO, GSM & WCDMA \n"
                                      "     11  - LTE only \n"
                                      "     12  - LTE & WCDMA \n"
                                      "     13  - TD-SDCAM only \n"
                                      "     14  - TD-SDCAM & WCDMA \n"
                                      "     15  - TD-SDCAM & LTE \n"
                                      "     16  - TD-SDCAM & GSM \n"
                                      "     17  - TD-SDCAM, GSM & LTE \n"
                                      "     18  - TD-SDCAM, GSM, & WCDMA \n"
                                      "     19  - TD-SDCAM, WCDMA & LTE \n"
                                      "     20  - TD-SDCAM, GSM, WCDMA & LTE \n"
                                      "-------------------------------------------------------\n");
                break;
            }
            int32_t networkType = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set preferred network type : %d (%s)\n", networkType, getPreferredNetworkTypeString(networkType));
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setPreferredNetworkType(networkType);
        } break;

        case CMD_GET_AVAILABLE_NETWORKS: {
            sp<INetworkQueryService> networkQueryService = interface_cast<INetworkQueryService>
                        (defaultServiceManager()->getService(String16("service_layer.NetworkQueryService")));

            if (networkQueryService == NULL) {
                printMessage("    Telephony is not avaialbe, try later\n");
                break;
            }

            static sp<Waiter> waiter = new Waiter();
            static sp<MyNetworkQueryServiceCallback> networkQueryCallback = new MyNetworkQueryServiceCallback(waiter);

            if (argc == 2) {
                int32_t timeout = sl::atoi(argv[1]);
                if (timeout < 0 || timeout > 180) {
                    timeout = 180;
                }
                waiter->setTimeout(timeout);
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get avilable networks\n");
            printMessage("-------------------------------------------------------\n");

            networkQueryService->startNetworkQuery(networkQueryCallback);
            printMessage("     Searching...");
            waiter->wait();
            if (waiter->getResult() == 0) {
                printMessage(" Timed out\n");
            }
            networkQueryService->stopNetworkQuery(networkQueryCallback);
        } break;

        case CMD_SET_NETWORK_SELECTION_MODE_AUTOMATIC: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Select network automatic\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setNetworkSelectionModeAutomatic();
            printMessage(" -> Result : %s\n", result ? "Succeeded" : "Failed");
        } break;

        case CMD_SELECT_NETWORK_MANUALLY: {
            if (argc < 2) {
                usage_telephony(CMD_SELECT_NETWORK_MANUALLY);
                break;
            }
            const char* operatorNumeric = argv[1];
            printMessage("-------------------------------------------------------\n");
            printMessage("    Select network manually : %s\n", operatorNumeric);
            printMessage("-------------------------------------------------------\n");
            printMessage(" *** You have to execute \"getAvailNets\" to get the available network list ***\n");
            printMessage("-------------------------------------------------------\n");
            if (strlen(operatorNumeric) >= 5) {
                printMessage("      Registering on %s ...", operatorNumeric);
                bool result = TelephonyManager::selectNetworkManually(operatorNumeric);
                printMessage(" %s\n", result ? "Succeeded" : "Failed");
            }
        } break;

        case CMD_GET_NETWORK_SELECTION_MODE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get network selection mode \n");
            printMessage("-------------------------------------------------------\n");
            int32_t mode = TelephonyManager::getNetworkSelectionMode();
            printMessage(" -> Mode : %d (%s)\n", mode, mode==0 ? "Automatic" : "Manual");
        } break;

        case CMD_GET_NETWORK_REG_STATE: {
            int32_t type = 0xFF;
            if (argc == 2) {
                type = sl::atoi(argv[1]);
            }
            if (type == 0xFF) {
                printMessage("-------------------------------------------------------\n");
                printMessage("    Get network registration state \n");
                printMessage("-------------------------------------------------------\n");
                int32_t voiceState = TelephonyManager::getNetworkRegState(0);
                int32_t dataState  = TelephonyManager::getNetworkRegState(1);
                printMessage(" -> Network Reg State (Voice) : %d (%s) \n", voiceState, getNetRegStateString(voiceState));
                printMessage("    Network Reg State (Data)  : %d (%s) \n", dataState, getNetRegStateString(dataState));

            } else {
                printMessage("-------------------------------------------------------\n");
                printMessage("    Get network registration state (%s)\n", type==0 ? "Voice" : "Data");
                printMessage("-------------------------------------------------------\n");
                int32_t state = TelephonyManager::getNetworkRegState(type);
                printMessage(" -> Network Reg State (%s) : %d (%s)\n", type==0 ? "Voice" : "Data", state, getNetRegStateString(state));
            }
        } break;

        case CMD_GET_MCC: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get MCC (0~999)\n");
            printMessage("-------------------------------------------------------\n");
            int32_t mcc = TelephonyManager::getMcc();
            printMessage(" -> MCC : %d\n", mcc);
        } break;

        case CMD_GET_MNC: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get MNC (0~999)\n");
            printMessage("-------------------------------------------------------\n");
            int32_t mnc = TelephonyManager::getMnc();
            printMessage(" -> MNC : %d\n", mnc);
        } break;

        case CMD_GET_LAC: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get LAC (0 ~ 65535)\n");
            printMessage("-------------------------------------------------------\n");
            int32_t lac = TelephonyManager::getLac();
            printMessage(" -> LAC : %d\n", lac);
        } break;

        case CMD_GET_CID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get CID (GSM:0 ~ 65535, LTE:0 ~ 268435455)\n");
            printMessage("-------------------------------------------------------\n");
            int32_t cid = TelephonyManager::getCid();
            printMessage(" -> CID : %d\n", cid);
        } break;

        case CMD_GET_RX_LEVEL: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get the signal receive level (0 ~ 31, 99-Unknown)\n");
            printMessage("-------------------------------------------------------\n");
            int32_t rxLevel = TelephonyManager::getRxLevel();
            int32_t networkClass = TelephonyManager::getNetworkClass();
            printMessage(" -> Rx Level : %d [RAT:%s]\n", rxLevel, getNetworkClassString(networkClass));
        } break;

        case CMD_GET_SIGNAL_STRENGTH: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get the signal strength as dBm\n");
            printMessage("-------------------------------------------------------\n");
            int32_t rxLevel = TelephonyManager::getSignalStrength();
            int32_t networkClass = TelephonyManager::getNetworkClass();
            printMessage(" -> Signal strength : %d dBm [RAT:%s]\n", rxLevel, getNetworkClassString(networkClass));
        } break;

        case CMD_GET_SNR: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get the signal to noise ratio, only valid in LTE\n");
            printMessage("-------------------------------------------------------\n");
            if (TelephonyManager::NETWORK_CLASS_4_G == TelephonyManager::getNetworkClass()) {
                int32_t snr = TelephonyManager::getSnr();
                if (SignalStrength::INVALID == snr) {
                    printMessage(" -> SNR : Invalid\n");
                } else {
                    printMessage(" -> SNR : %.1f dB\n", ((float)snr/10));
                }
            } else {
                printMessage(" -> SNR is only valid in LTE.\n");
            }
        } break;

        case CMD_SET_BAND_MODE: {
            if (argc < 2) {
                usage_telephony(CMD_SET_BAND_MODE);
                break;
            }

            int32_t bandMode = sl::atoi(argv[1]);
            if (bandMode != 0) {
                bandMode = 1;
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set band mode : %d\n", bandMode);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setBandMode(bandMode);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_GET_BAND_MODE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get band mode \n");
            printMessage("-------------------------------------------------------\n");
            int32_t bandMode = TelephonyManager::getBandMode();
            printMessage(" -> Band mode : %d\n", bandMode);
        } break;

        case CMD_GET_SERVICE_STATE: {
            int32_t type = 0xFF;
            if (argc == 2) {
                type = sl::atoi(argv[1]);
            }

            if (type == 0xFF) {
                printMessage("-------------------------------------------------------\n");
                printMessage("    Get service state \n");
                printMessage("-------------------------------------------------------\n");
                int32_t voiceState = TelephonyManager::getServiceState(0);
                int32_t dataState  = TelephonyManager::getServiceState(1);
                printMessage(" -> Service state (Voice) : %d (%s) \n", voiceState, getServiceStateString(voiceState));
                printMessage("    Service state (Data)  : %d (%s) \n", dataState, getServiceStateString(dataState));

            } else {
                printMessage("-------------------------------------------------------\n");
                printMessage("    Get service state (%s)\n", type==0 ? "Voice" : "Data");
                printMessage("-------------------------------------------------------\n");
                int32_t state = TelephonyManager::getServiceState(type);
                printMessage(" -> Service state (%s) : %d (%s)\n", type==0 ? "Voice":"Data", state, getServiceStateString(state));
            }
        } break;

        case CMD_HAS_SIM_CARD: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Has sim card ?\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::hasSimCard();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_IS_SIM_PIN_ENABLED: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Is sim pin enabled ?\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::isSimPinEnabled();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_CHANGE_SIM_LOCK_STATE: {
            if (argc < 3) {
                usage_telephony(CMD_CHANGE_SIM_LOCK_STATE);
                break;
            }

            bool enable = true;
            if (!strcmp(argv[1], "off")) {
                enable = false;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Change sim lock state : %s %s\n", enable ? "on" : "off", argv[2]);
            printMessage("-------------------------------------------------------\n");

            int32_t result = TelephonyManager::changeSimLockState(enable, argv[2]);
            int32_t returnValue = result & 0x000000FF;
            int32_t remainingCount = (result & 0x0000FF00) >> 8;

            StringWrapper s;
            s.appendFormatted(" -> Result : %d (%s)", returnValue, getPinReturnResultString(returnValue));
            if (returnValue != PhoneConstants::PIN_RESULT_SUCCESS) {
                s.appendFormatted(", remaining count : %d", remainingCount);
            }
            s.append("\n");
            printMessage(s.c_str());
        } break;

        case CMD_CHANGE_PIN: {
            if (argc < 3) {
                usage_telephony(CMD_CHANGE_PIN);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Change pin : %s %s\n", argv[1], argv[2]);
            printMessage("-------------------------------------------------------\n");

            int32_t result = TelephonyManager::changePin(argv[1], argv[2]);
            int32_t returnValue = result & 0x000000FF;
            int32_t remainingCount = (result & 0x0000FF00) >> 8;

            StringWrapper s;
            s.appendFormatted(" -> Result : %d (%s)", returnValue, getPinReturnResultString(returnValue));
            if (returnValue != PhoneConstants::PIN_RESULT_SUCCESS) {
                s.appendFormatted(", remaining count : %d", remainingCount);
            }
            s.append("\n");
            printMessage(s.c_str());
        } break;

        case CMD_SUPPLY_PIN: {
            if (argc < 2) {
                usage_telephony(CMD_SUPPLY_PIN);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Supply pin : %s\n", argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::supplyPin(argv[1]);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_SUPPLY_PUK: {
            if (argc < 3) {
                usage_telephony(CMD_SUPPLY_PUK);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Supply puk : %s %s\n", argv[1], argv[2]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::supplyPuk(argv[1], argv[2]);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_SUPPLY_PIN_REPORT_RESULT: {
            if (argc < 2) {
                usage_telephony(CMD_SUPPLY_PIN_REPORT_RESULT);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Supply pin report result : %s\n", argv[1]);
            printMessage("-------------------------------------------------------\n");

            int32_t result = TelephonyManager::supplyPinReportResult(argv[1]);
            int32_t returnValue = result & 0x000000FF;
            int32_t remainingCount = (result & 0x0000FF00) >> 8;


            StringWrapper s;
            s.appendFormatted(" -> Result : %d (%s)", returnValue, getPinReturnResultString(returnValue));
            if (returnValue != PhoneConstants::PIN_RESULT_SUCCESS) {
                s.appendFormatted(", remaining count : %d", remainingCount);
            }
            s.append("\n");
            printMessage(s.c_str());
        } break;

        case CMD_SUPPLY_PUK_REPORT_RESULT: {
            if (argc < 3) {
                usage_telephony(CMD_SUPPLY_PUK_REPORT_RESULT);
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Supply puk report result : %s %s\n", argv[1], argv[2]);
            printMessage("-------------------------------------------------------\n");

            int32_t result = TelephonyManager::supplyPukReportResult(argv[1], argv[2]);
            int32_t returnValue = result & 0x000000FF;
            int32_t remainingCount = (result & 0x0000FF00) >> 8;

            StringWrapper s;
            s.appendFormatted(" -> Result : %d (%s)", returnValue, getPinReturnResultString(returnValue));
            if (returnValue != PhoneConstants::PIN_RESULT_SUCCESS) {
                s.appendFormatted(", remaining count : %d", remainingCount);
            }
            s.append("\n");
            printMessage(s.c_str());
        } break;

        case CMD_GET_SIM_REMAINING_COUNT: {
            if (argc < 2) {
                usage_telephony(CMD_GET_SIM_REMAINING_COUNT);
                break;
            }
            int32_t type = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim remaining count (%s)\n", getSimRemainingCountTypeString(type));
            printMessage("-------------------------------------------------------\n");
            int32_t count = TelephonyManager::getSimRemainingCount(type);
            printMessage(" -> SIM remaining count (%s) : %d\n", getSimRemainingCountTypeString(type), count);
        } break;

        case CMD_GET_SIM_OPERATOR: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim operator numeric\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> simOperator = TelephonyManager::getSimOperator();
            printMessage(" -> SIM MCC/MNC : %s\n", CSTR(simOperator));
        } break;

/* TODO : port SPN
        case CMD_GET_SIM_OPERATOR_NAME: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim operator name\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> simOperatorName = TelephonyManager::getSimOperatorName();
            printMessage(" -> SPN : %s\n", CSTR(simOperatorName));
        } break;
*/
        case CMD_GET_SIM_COUNRTY_ISO: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim country ISO\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> simCountryIso = TelephonyManager::getSimCountryIso();
            printMessage(" -> SIM ISO country code : %s\n", CSTR(simCountryIso));
        } break;

        case CMD_GET_ICCID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get ICCID\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> iccid = TelephonyManager::getIccid();
            printMessage(" -> ICCID : %s\n", CSTR(iccid));
        } break;
        case CMD_GET_EID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get EID\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> eid = TelephonyManager::getEid();
            printMessage(" -> EID : %s\n", CSTR(eid));
        } break;

        case CMD_GET_IMSI: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get IMSI\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> imsi = TelephonyManager::getImsi();
            printMessage(" -> IMSI : %s\n", CSTR(imsi));
        } break;

        case CMD_GET_MSISDN: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get MSISDN\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> msisdn = TelephonyManager::getMsisdn();
            printMessage(" -> MSISDN : %s\n", CSTR(msisdn));
        } break;

        case CMD_GET_SIM_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim state\n");
            printMessage("-------------------------------------------------------\n");
            int32_t state = TelephonyManager::getSimState();
            printMessage(" -> SIM state : %d (%s)\n", state, getSimStateString(state));
        } break;

        case CMD_GET_SIM_PROFILE_COUNT: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim profile count\n");
            printMessage("-------------------------------------------------------\n");
            int32_t profileCount = TelephonyManager::getSimProfileCount();
            printMessage(" -> Profile count : %d\n", profileCount);
        } break;

        case CMD_GET_SIM_PROFILE_ID: {
            if (argc < 2) {
                usage_telephony(CMD_GET_SIM_PROFILE_ID);
                break;
            }

            int32_t profileType = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim profile id : %d (%s)\n", profileType, getSimProfileTypeString(profileType));
            printMessage("-------------------------------------------------------\n");
            int32_t profileId = TelephonyManager::getSimProfileId(profileType);
            if (0 == strlen(getSimProfileIdString(profileId))) {
                printMessage(" -> Profile id : %d\n", profileId);
            } else {
                printMessage(" -> Profile id : %d (%s)\n", profileId, getSimProfileIdString(profileId));
            }
        } break;

        case CMD_GET_SIM_PROFILE_TYPE: {
            if (argc < 2) {
                usage_telephony(CMD_GET_SIM_PROFILE_TYPE);
                break;
            }

            int32_t profileId = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get sim profile type : %d\n", profileId);
            printMessage("-------------------------------------------------------\n");
            int32_t profileType = TelephonyManager::getSimProfileType(profileId);
            printMessage(" -> Profile type : %d (%s)\n", profileType, getSimProfileTypeString(profileType));
        } break;

        case CMD_GET_ACTIVE_SIM_PROFILE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get active sim profile id\n");
            printMessage("-------------------------------------------------------\n");
            int32_t profileId = TelephonyManager::getActiveSimProfile();
            if (0 == strlen(getSimProfileIdString(profileId))) {
                printMessage(" -> Profile id : %d\n", profileId);
            } else {
                printMessage(" -> Profile id : %d (%s)\n", profileId, getSimProfileIdString(profileId));
            }
        } break;

        case CMD_SET_ACTIVE_SIM_PROFILE: {
            if (argc < 2) {
                usage_telephony(CMD_SET_ACTIVE_SIM_PROFILE);
                break;
            }

            int32_t profileId = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set active sim profile id : %d\n", profileId);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setActiveSimProfile(profileId);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_OPEN_SIM_LOGICAL_CHANNEL: {
            if (argc < 2) {
                usage_telephony(CMD_OPEN_SIM_LOGICAL_CHANNEL);
                break;
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Open sim logical channel : %s\n", argv[1]);
            printMessage("-------------------------------------------------------\n");
            int32_t channel = TelephonyManager::openSimLogicalChannel(argv[1]);
            printMessage(" -> Channel id : %d\n", channel);
        } break;

        case CMD_CLOSE_SIM_LOGICAL_CHANNEL: {
            if (argc < 2) {
                usage_telephony(CMD_CLOSE_SIM_LOGICAL_CHANNEL);
                break;
            }
            int32_t channel = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Close sim logical channel : %d\n", channel);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::closeSimLogicalChannel(channel);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case  CMD_TRANSMIT_APDU_LOGICAL_CHANNEL: {
            if (argc < 7) {
                usage_telephony(CMD_TRANSMIT_APDU_LOGICAL_CHANNEL);
                break;
            }
            int32_t channel = sl::atoi(argv[1]);
            int32_t cla = sl::atoi(argv[2]);
            int32_t command = sl::atoi(argv[3]);
            int32_t p1 = sl::atoi(argv[4]);
            int32_t p2 = sl::atoi(argv[5]);
            int32_t p3 = sl::atoi(argv[6]);
			const char* stringdata = argv[7];
            printMessage("-------------------------------------------------------\n");
            printMessage("    TRANSMIT APDU LOGICAL CHANNEL : %d %d %d %d %d %d %s\n", channel, cla, command, p1, p2, p3, stringdata);
            printMessage("-------------------------------------------------------\n");
            sp<String> result = TelephonyManager::iccTransmitApduLogicalChannel(channel, cla, command, p1, p2, p3, stringdata);
            printMessage(" -> Result : %s\n", CSTR(result));
        } break;

		case CMD_TRANSMIT_APDU_BASIC_CHANNEL: {
            if (argc < 6) {
                usage_telephony(CMD_TRANSMIT_APDU_BASIC_CHANNEL);
                break;
            }
            int32_t cla = sl::atoi(argv[1]);
            int32_t command = sl::atoi(argv[2]);
            int32_t p1 = sl::atoi(argv[3]);
            int32_t p2 = sl::atoi(argv[4]);
            int32_t p3 = sl::atoi(argv[5]);
			const char* stringdata = argv[6];
            printMessage("-------------------------------------------------------\n");
            printMessage("    TRANSMIT APDU BASIC CHANNEL : %d %d %d %d %d %s\n", cla, command, p1, p2, p3, stringdata);
            printMessage("-------------------------------------------------------\n");
            sp<String> result = TelephonyManager::iccTransmitApduBasicChannel(cla, command, p1, p2, p3, stringdata);
            printMessage(" -> Result : %s\n", CSTR(result));
        } break;

        case CMD_GET_TCU_POWER_MODE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get TCU power mode\n");
            printMessage("-------------------------------------------------------\n");
            int32_t mode = TelephonyManager::getTcuPowerMode();
            printMessage(" -> mode : %d (%s) \n", mode, getPowerModeString(mode));
        } break;

        case CMD_SET_TCU_POWER_MODE: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setPowerMode <mode> \n"
                               "-------------------------------------------------------\n"
                               "   <mode> \n"
                               "     0   - Unpowered mode \n"
                               "     1   - Sleep mode \n"
                               "     2   - Polling mode 1 \n"
                               "     3   - Polling mode 2 \n"
                               "     4   - Listen mode 1 \n"
                               "     5   - Listen mode 2 \n"
                               "     6   - Normal mode \n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t mode = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set TCU power mode (mode: %d)\n", mode);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setTcuPowerMode(mode);
            printMessage(" -> result : %s\n", BOOL_STR(result));
        } break;

        case CMD_GET_TCU_OPERATION_MODE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get TCU operation mode\n");
            printMessage("-------------------------------------------------------\n");
            int32_t mode = TelephonyManager::getTcuOperationMode();
            printMessage(" -> mode : %d (%s)\n", mode, getOperationModeString(mode));
        } break;

        case CMD_SET_TCU_OPERATION_MODE: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setOperMode <mode> \n"
                               "-------------------------------------------------------\n"
                               "   <mode> \n"
                               "     0   - Deactivated mode \n"
                               "     1   - Preactivated mode \n"
                               "     2   - Activated Unprovisioned mode \n"
                               "     3   - Activated Provisioned mode \n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t mode = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set TCU operation mode (mode: %d)\n", mode);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setTcuOperationMode(mode);
            printMessage(" -> result : %s\n", BOOL_STR(result));
        } break;

        case CMD_GET_TCU_SPY_MODE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get TCU spy mode\n");
            printMessage("-------------------------------------------------------\n");
            int32_t mode = TelephonyManager::getTcuSpyMode();
            printMessage(" -> mode : %d (%s)\n", mode, getSpyModeString(mode));
        } break;

        case CMD_SET_TCU_SPY_MODE: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setSpyMode <mode> \n"
                               "-------------------------------------------------------\n"
                               "   <mode> \n"
                               "     0   - Disable \n"
                               "     1   - Enable \n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t mode = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set TCU spy mode (mode: %d)\n", mode);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::setTcuSpyMode(mode);
            printMessage(" -> result : %s\n", BOOL_STR(result));
        } break;

        case CMD_CHECK_RF_CALIBRATION: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Check RF calibration\n");
            printMessage("-------------------------------------------------------\n");
            bool result  = TelephonyManager::checkRfCalibration();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_SET_ACTIVE_SIM: {
            bool bFailed = false;
            while(1) {
                int32_t sim = TelephonyManager::REMOTE_SIM_TSIM;
                int32_t priority = TelephonyManager::REMOTE_SIM_PRIORITY_NORMAL;
                int32_t cause = TelephonyManager::REMOTE_SIM_CAUSE_NLI;

                if (argc < 4) {
                    bFailed = true;
                    break;
                }

                if (argc >= 2) {
                    StringWrapper simString(argv[1]);
                    if (simString.equalsIgnoreCase("TSIM")) {
                        sim = TelephonyManager::REMOTE_SIM_TSIM;
                    } else if (simString.equalsIgnoreCase("PSIM")) {
                        sim = TelephonyManager::REMOTE_SIM_PSIM;
                    } else {
                        bFailed = true;
                        break;
                    }
                }


                if (argc >= 3) {
                    StringWrapper priorirtyString(argv[2]);
                    if (priorirtyString.equalsIgnoreCase("NORMAL")) {
                        priority = TelephonyManager::REMOTE_SIM_PRIORITY_NORMAL;
                    } else if (priorirtyString.equalsIgnoreCase("HIGH")) {
                        priority = TelephonyManager::REMOTE_SIM_PRIORITY_HIGH;
                    } else {
                        bFailed = true;
                        break;
                    }
                }

                if (argc >= 4) {
                    StringWrapper causeString(argv[3]);
                    if (causeString.equalsIgnoreCase("NGI")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_NGI;
                    } else if (causeString.equalsIgnoreCase("NLI")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_NLI;
                    } else if (causeString.equalsIgnoreCase("NGTP")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_NGTP;
                    } else if (causeString.equalsIgnoreCase("ECALL")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_ECALL;
                    } else if (causeString.equalsIgnoreCase("BCALL")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_BCALL;
                    } else if (causeString.equalsIgnoreCase("SVT")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_SVT;
                    } else if (causeString.equalsIgnoreCase("REMOTE")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_REMOTESERVICE;
                    } else if (causeString.equalsIgnoreCase("PROVISIONING")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_PROVISIONING;
                    } else if (causeString.equalsIgnoreCase("EJECT")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_PSIM_EJECT;
                    } else if (causeString.equalsIgnoreCase("NAD")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_NAD;
                    } else if (causeString.equalsIgnoreCase("RETRY_STRATEGY")) {
                        cause = TelephonyManager::REMOTE_SIM_CAUSE_RETRY_STRATEGY;
                    } else {
                        bFailed = true;
                        break;
                    }
                }

                printMessage("-------------------------------------------------------\n");
                printMessage("    Set active sim \n");
                printMessage("-------------------------------------------------------\n");
                int32_t result = TelephonyManager::setActiveSim(sim, priority, cause);
                printMessage(" -> result : %s\n", getSetActiveSimResultString(result));
                break;
            }

            if (bFailed) {
                printMessage( "-------------------------------------------------------\n"
                              " sldd telephony setActSim <sim> <priority> <cause> \n"
                              "-------------------------------------------------------\n"
                              "   sim      : TSIM or PSIM \n"
                              "   priority : NORMAL or HIGH \n"
                              "   cause   : NGI, NLI, NGTP, ECALL, BCALL, SVT, REMOTE, PROVISIONING, EJECT, LISTEN, RETRY_STRATEGY \n"
                              "-------------------------------------------------------\n");
            }
        } break;

        case CMD_GET_ACTIVE_SIM: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get active sim\n");
            printMessage("-------------------------------------------------------\n");
            int32_t activeSim = TelephonyManager::getActiveSim();
            printMessage(" -> Active SIM : %d (%s)\n", activeSim, getGetActiveSimResultString(activeSim));
        } break;

        case CMD_GET_PSIM_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get psim state\n");
            printMessage("-------------------------------------------------------\n");
            int32_t state = TelephonyManager::getPSimState();
            printMessage(" -> State : %d (%s)\n", state, getPsimStateString(state));
        } break;

        case CMD_GET_SIM_SWITCHING_CAUSE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get SIM switching cause\n");
            printMessage("-------------------------------------------------------\n");
            int32_t cause = TelephonyManager::getSimSwitchingCause();
            printMessage(" -> Cause : %d (%s)\n", cause, getSimSwitchingCauseString(cause));
        } break;

        case CMD_GET_PSIM_BLOCK_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get PSIM block state\n");
            printMessage("-------------------------------------------------------\n");
            int32_t maxNoSimSwitch = 0;
            int32_t noSimSwitch = 0;
            bool timerInOperation = false;
            TelephonyManager::getPsimBlockState(maxNoSimSwitch, noSimSwitch, timerInOperation);
            printMessage(" -> Max # of SIM switch : %d, # of SIM switch : %d, Timer in operation: %s\n", maxNoSimSwitch, noSimSwitch, BOOL_STR(timerInOperation));
        } break;

        case CMD_CLEAR_PSIM_BLOCK_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Clear PSIM block state\n");
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::clearPsimBlockState();
        } break;

        case CMD_ACQUIRE_PSIM_LOCK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Acquire PSIM lock\n");
            printMessage("-------------------------------------------------------\n");
            bool result = sPsimLock->acquire();
            printMessage(" -> Result : %s\n", result ? "Succeeded" : "Failed");
            printMessage("\n");
            printMessage("*** PSIM lock is acquired and will be released in 1 minute\n");
            ::sleep(60);
            sPsimLock->release();
            printMessage("\n");
            printMessage("*** PSIM lock is released\n");
        } break;

        case CMD_RELEASE_PSIM_LOCK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Release PSIM lock\n");
            printMessage("-------------------------------------------------------\n");
            bool result = sPsimLock->release();
            printMessage(" -> Result : %s\n", result ? "Succeeded" : "Failed");
        } break;

        case CMD_GET_PSIM_LOCK_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get PSIM lock state\n");
            printMessage("-------------------------------------------------------\n");
            bool locked = TelephonyManager::getPsimLockState();
            printMessage(" -> Result : %s\n", locked ? "on" : "off");
        } break;

        case CMD_ACQUIRE_TSIM_LOCK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Acquire TSIM lock\n");
            printMessage("-------------------------------------------------------\n");
            bool result = sTsimLock->acquire();
            printMessage(" -> Result : %s\n", result ? "Succeeded" : "Failed");
            printMessage("\n");
            printMessage("*** TSIM lock is acquired and will be released in 1 minute\n");
            ::sleep(60UL);
            sTsimLock->release();
            printMessage("\n");
            printMessage("*** TSIM lock is released\n");
        } break;

        case CMD_RELEASE_TSIM_LOCK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Release TSIM lock\n");
            printMessage("-------------------------------------------------------\n");
            bool result = sTsimLock->release();
            printMessage(" -> Result : %s\n", result ? "Succeeded" : "Failed");
        } break;

        case CMD_GET_TSIM_LOCK_STATE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get TSIM lock state\n");
            printMessage("-------------------------------------------------------\n");
            bool locked = TelephonyManager::getTsimLockState();
            printMessage(" -> Result : %s\n", locked ? "on" : "off");
        } break;

        case CMD_SET_TOPS_STATUS: {
            if (argc == 2) {
                int32_t status = sl::atoi(argv[1]);
                if (status == TelephonyManager::TOPS_STATUS_ACTIVE_NOT_CONNECTED ||
                    status == TelephonyManager::TOPS_STATUS_ACTIVE_CONNECTED) {
                    printMessage("-------------------------------------------------------\n");
                    printMessage("    Set TOPS status: %s\n", TopsStatusAsString(status));
                    printMessage("-------------------------------------------------------\n");
                    TelephonyManager::setTopsStatus(status);
                    break;
                }
            }
            printMessage( "-------------------------------------------------------\n"
                          " sldd telephony setTopsStatus <status>\n"
                          "-------------------------------------------------------\n"
                          "   <status> \n"
                          "     2   - Active, not connected \n"
                          "     3   - Active, connected \n"
                          "-------------------------------------------------------\n");
        } break;

        case CMD_GET_TOPS_STATUS: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get TOPS status\n");
            printMessage("-------------------------------------------------------\n");
            int32_t status = TelephonyManager::getTopsStatus();
            printMessage(" -> Status : %s\n", TopsStatusAsString(status));
        } break;

        case CMD_GET_TOPS_NUMBER: {
            if (argc == 2) {
                int32_t type = sl::atoi(argv[1]);
                if (type >= TelephonyManager::TOPS_NUMBER_FALLBACKS_TOTAL && type <= TelephonyManager::TOPS_NUMBER_CONNECTED_SINCE) {
                    printMessage("-------------------------------------------------------\n");
                    printMessage("    Get TOPS %s\n", TopsNumberAsString(type));
                    printMessage("-------------------------------------------------------\n");
                    int32_t number = TelephonyManager::getTopsNumber(type);
                    printMessage(" -> Result : %d\n", number);
                    break;
                }
            }
            printMessage( "-------------------------------------------------------\n"
                          " sldd telephony getTopsNumber <type>\n"
                          "-------------------------------------------------------\n"
                          "   <type> \n"
                          "     0   - number of fallbacks total \n"
                          "     1   - number of fallbacks total since last successful connection \n"
                          "     2   - number of times connectivity has been disabled due to too many fallbacks \n"
                          "     3   - number of times connected total \n"
                          "     4   - number of times connected since (last) start of connectivity system \n"
                          "-------------------------------------------------------\n");
        } break;

        case CMD_ECALL_INDICATION_REGI: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Indication Regi (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::eCallIndicationRegi(bytes->getBytes(), bytes->size());
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_ECALL_START: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Start (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::eCallStart(bytes->getBytes(), bytes->size());
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_ECALL_SET_MSD: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Set MSD (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::eCallSetMsd(bytes->getBytes(), bytes->size());
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_ECALL_SET_CONFIG: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Set Config (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::eCallSetConfig(bytes->getBytes(), bytes->size());
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_ECALL_STOP: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Stop (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            int32_t result = TelephonyManager::eCallStop(bytes->getBytes(), bytes->size());
            printMessage(" -> Result : %d\n", result);
        } break;

        case CMD_ECALL_ANSWER: {
            if (argc < 2) {
                break;
            }
            sp<ByteArray> bytes = IccUtils::hexStringToBytes(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    eCall Answer (%d: %s)\n", bytes->size(), argv[1]);
            printMessage("-------------------------------------------------------\n");
            int32_t callId = TelephonyManager::eCallAnswer(bytes->getBytes(), bytes->size());
            printMessage(" -> Call ID : %d\n", callId);
        } break;

        case CMD_SET_CALLBACK_STATE: {
            if (argc < 2) {
                break;
            }
            int32_t state = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set callback state (%d)\n", state);
            printMessage("-------------------------------------------------------\n");
            int32_t result = TelephonyManager::setCallbackState(state);
            printMessage(" -> Result : %d\n", result);
        } break;

        case CMD_GET_JAMMING_DETECTION_STATUS: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get jamming detection status\n");
            printMessage("-------------------------------------------------------\n");
            int32_t status  = TelephonyManager::getJammingDetectionStatus();
            printMessage(" -> Status : %d (%s)\n", status, getJammingDetectionStatusString(status));
        } break;

        case CMD_SET_DIVERSITY_ANTENNA: {
            if (argc < 2) {
                usage_telephony(CMD_SET_DIVERSITY_ANTENNA);
                break;
            }
            int32_t divAnt = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set diversity antenna: %d\n", divAnt);
            printMessage("-------------------------------------------------------\n");
            int32_t result  = TelephonyManager::setDiversityAntenna(divAnt);
            printMessage(" -> Result : %s\n", result == 1 ? "Succeeded" : "Failed");
        } break;

        case CMD_GET_TEMPERATURE: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get temperature\n");
            printMessage("-------------------------------------------------------\n");
            int32_t temperature = 0;
            int32_t result  = TelephonyManager::getTemperature(temperature);

            if (result != TelephonyManager::TEMPERATURE_RESULT_SUCCESS) {
                printMessage(" -> Result : %d (%s)\n", result, getTemperatureResultString(result));
            } else {
                printMessage(" -> Result : %d (%s), Temperature : %.3f 'C\n",
                            result, getTemperatureResultString(result), ((float)temperature/1000));
            }
        } break;

        case CMD_DUMP: {
            if (argc < 2) {
                break;
            }
            int32_t module = sl::atoi(argv[1]);
            if (module == TelephonyManager::MODULE_CALL_TRACKER
                    || module == TelephonyManager::MODULE_SETRVICE_STATE_TRACKER
                    || module == TelephonyManager::MODULE_UICC_CONTROLLER) {
                printMessage("-------------------------------------------------------\n");
                printMessage("    dump (module: %d)\n", module);
                printMessage("-------------------------------------------------------\n");
                TelephonyManager::dumpModule(module);
            }
        } break;

        case CMD_GET_FRAT_TIMER: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get FRAT timer\n");
            printMessage("-------------------------------------------------------\n");
            int32_t result = TelephonyManager::getFratTimer();
            int32_t slowTimer = result & 0x000000FF;
            int32_t fastTimer = (result & 0x0000FF00) >> 8;
            printMessage(" -> Slow timer: %d, Fast timer: %d\n", slowTimer, fastTimer);
        } break;

        case CMD_SET_FRAT_TIMER: {
            if (argc < 3) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setFratTimer <SlowTimer> <FastTimer>\n"
                               "-------------------------------------------------------\n");
                break;
            }
            uint8_t slowTimer = (uint8_t)sl::atoi(argv[1]);
            uint8_t fastTimer = (uint8_t)sl::atoi(argv[2]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set FRAT timer: %d %d\n", slowTimer, fastTimer);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setFratTimer(slowTimer, fastTimer);
        } break;

        case CMD_LISTEN: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony listen <events> <time=600sec>\n"
                               "-------------------------------------------------------\n"
                               "   <events> \n"
                               "     0   - Listen all \n"
                               "     1   - Listen service state \n"
                               "     2   - Listen data service state \n"
                               "     3   - Listen service states \n"
                               "     4   - Listen call state \n"
                               "     5   - Listen signal strength \n"
                               "     6   - Listen precise call state \n"
                               "     7   - Listen disconnect \n"
                               "     8   - Listen psim state \n"
                               "     9   - Listen active sim state \n"
                               "     a   - Listen internal sim state \n"
                               "     b   - Listen PSIM lock state \n"
                               "     c   - Listen TSIM lock state \n"
                               "     d   - Listen sim session closed \n"
                               "     e   - Listen ecall state \n"
                               "     f   - Listen ecall era glonass msd tx fail \n"
                               "     g   - Listen ecall msd request \n"
                               "     h   - Listen ecall redial type \n"
                               "     i   - Listen ecall sent msd reported \n"
                               "     j   - Listen ecall incall status \n"
                               "     k   - Listen tachyon antenna status (only support factory version) \n"
                               "     l   - Listen request for listen mode (only support factory version)\n"
                               "     m   - Listen jamming detection status \n"
                               "     n   - Listen wakeup antenna adc \n"
                               "     w   - Listen reached Data Limit \n"
                               "     x   - Listen changes to the direction of data traffic on the data connection\n"
                               "     y   - Listen data connection failed \n"
                               "     z   - Listen data connection state \n"
                               "-------------------------------------------------------\n");
                break;
            }

            uint32_t timeout = 600;
            if (argc == 3) {
                timeout = sl::atoi(argv[2]);
            }

            int32_t events = 0;
            StringWrapper s(argv[1]);
            for (uint32_t i=0; i<s.size(); i++) {
                if (s[i] == '0') { // all
                    events = PhoneStateListener::LISTEN_SERVICE_STATE |
                             PhoneStateListener::LISTEN_DATA_SERVICE_STATE |
                             PhoneStateListener::LISTEN_SERVICE_STATES |
                             PhoneStateListener::LISTEN_CALL_STATE |
                             PhoneStateListener::LISTEN_SIGNAL_STRENGTHS |
                             PhoneStateListener::LISTEN_PRECISE_CALL_STATE |
                             PhoneStateListener::LISTEN_DISCONNECT |
                             PhoneStateListener::LISTEN_PSIM_STATE |
                             PhoneStateListener::LISTEN_ACTIVE_SIM_STATE |
                             PhoneStateListener::LISTEN_INTERNAL_SIM_STATE |
                             PhoneStateListener::LISTEN_PSIM_LOCK_STATE |
                             PhoneStateListener::LISTEN_TSIM_LOCK_STATE |
                             PhoneStateListener::LISTEN_SIM_SESSION_CLOSED |
                             PhoneStateListener::LISTEN_ECALL_STATE |
                             PhoneStateListener::LISTEN_ECALL_ERA_GLONASS_MSD_TX_FAIL |
                             PhoneStateListener::LISTEN_ECALL_MSD_REQUESTED |
                             PhoneStateListener::LISTEN_ECALL_REDIAL_TYPE |
                             PhoneStateListener::LISTEN_ECALL_SENT_MSD_REPORTED |
                             PhoneStateListener::LISTEN_ECALL_INCALL_STATUS |
                             PhoneStateListener::LISTEN_TACHYON_ANTENNA_STATUS |
                             PhoneStateListener::LISTEN_REQUEST_LISTEN_MODE |
                             PhoneStateListener::LISTEN_JAMMING_DETECTION_STATUS |
                             PhoneStateListener::LISTEN_WAKEUP_ANTENNA_ADC |
                             PhoneStateListener::LISTEN_DATA_LIMIT |
                             PhoneStateListener::LISTEN_DATA_ACTIVITY |
                             PhoneStateListener::LISTEN_DATA_CONNECTION_FAILED |
                             PhoneStateListener::LISTEN_DATA_CONNECTION_STATE;
                    break;
                } else if (s[i] == '1') {
                    events |= PhoneStateListener::LISTEN_SERVICE_STATE;
                } else if (s[i] == '2') {
                    events |= PhoneStateListener::LISTEN_DATA_SERVICE_STATE;
                } else if (s[i] == '3') {
                    events |= PhoneStateListener::LISTEN_SERVICE_STATES;
                } else if (s[i] == '4') {
                    events |= PhoneStateListener::LISTEN_CALL_STATE;
                } else if (s[i] == '5') {
                    events |= PhoneStateListener::LISTEN_SIGNAL_STRENGTHS;
                } else if (s[i] == '6') {
                    events |= PhoneStateListener::LISTEN_PRECISE_CALL_STATE;
                } else if (s[i] == '7') {
                    events |= PhoneStateListener::LISTEN_DISCONNECT;
                } else if (s[i] == '8') {
                    events |= PhoneStateListener::LISTEN_PSIM_STATE;
                } else if (s[i] == '9') {
                    events |= PhoneStateListener::LISTEN_ACTIVE_SIM_STATE;
                } else if (s[i] == 'a') {
                    events |= PhoneStateListener::LISTEN_INTERNAL_SIM_STATE;
                } else if (s[i] == 'b') {
                    events |= PhoneStateListener::LISTEN_PSIM_LOCK_STATE;
                } else if (s[i] == 'c') {
                    events |= PhoneStateListener::LISTEN_TSIM_LOCK_STATE;
                } else if (s[i] == 'd') {
                    events |= PhoneStateListener::LISTEN_SIM_SESSION_CLOSED;
                } else if (s[i] == 'e') {
                    events |= PhoneStateListener::LISTEN_ECALL_STATE;
                } else if (s[i] == 'f') {
                    events |= PhoneStateListener::LISTEN_ECALL_ERA_GLONASS_MSD_TX_FAIL;
                } else if (s[i] == 'g') {
                    events |= PhoneStateListener::LISTEN_ECALL_MSD_REQUESTED;
                } else if (s[i] == 'h') {
                    events |= PhoneStateListener::LISTEN_ECALL_REDIAL_TYPE;
                } else if (s[i] == 'i') {
                    events |= PhoneStateListener::LISTEN_ECALL_SENT_MSD_REPORTED;
                } else if (s[i] == 'j') {
                    events |= PhoneStateListener::LISTEN_ECALL_INCALL_STATUS;
                } else if (s[i] == 'k') {
                    events |= PhoneStateListener::LISTEN_TACHYON_ANTENNA_STATUS;
                } else if (s[i] == 'l') {
                    events |= PhoneStateListener::LISTEN_REQUEST_LISTEN_MODE;
                } else if (s[i] == 'm') {
                    events |= PhoneStateListener::LISTEN_JAMMING_DETECTION_STATUS;
                } else if (s[i] == 'n') {
                    events |= PhoneStateListener::LISTEN_WAKEUP_ANTENNA_ADC;
                } else if (s[i] == 'w') {
                    events |= PhoneStateListener::LISTEN_DATA_LIMIT;
                } else if (s[i] == 'x') {
                    events |= PhoneStateListener::LISTEN_DATA_ACTIVITY;
                } else if (s[i] == 'y') {
                    events |= PhoneStateListener::LISTEN_DATA_CONNECTION_FAILED;
                } else if (s[i] == 'z') {
                    events |= PhoneStateListener::LISTEN_DATA_CONNECTION_STATE;
                } else {
                    // not supported option
                }
            }

            if (events != 0) {

                printMessage("-------------------------------------------------------\n");
                printMessage("    Listening... (for %d seconds)\n", timeout);
                printMessage("-------------------------------------------------------\n");

                // make Looper object
                sp<sl::SLLooper> looper = new sl::SLLooper();
                looper->prepare();
                looper->start(false);

                // make PhoneStateListener object
                sp<PhoneStateListener> phoneStateListener = new MyPhoneStateListener(looper);

                TelephonyManager::listen(phoneStateListener, events, true);

                ::sleep(timeout);

                TelephonyManager::listen(phoneStateListener, PhoneStateListener::LISTEN_NONE);

                looper->stop();
            }
        } break;

        case CMD_FACTORY_GET_RFCAL_CHECK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Get RFCAL Check\n");
            printMessage("-------------------------------------------------------\n");
            bool result  = TelephonyManager::factoryGetRfcalCheck();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_GET_DATA_CHECK: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Get Data Check\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factoryGetDataCheck();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_SET_DATA_CHECK: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony factorySetDataCheck <factoryData>\n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t factoryData = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Set Data Check: %d\n", factoryData);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factorySetDataCheck(factoryData);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_GET_IMEI: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Get IMEI\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> imei = TelephonyManager::factoryGetImei();
            printMessage(" -> IMEI : %s\n", CSTR(imei));
        } break;

        case CMD_FACTORY_SET_IMEI: {
            if (argc < 2 || (::strlen(argv[1]) != 14 && ::strlen(argv[1]) != 15)) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony factorySetImei <14 or 15 digit>\n"
                               "-------------------------------------------------------\n");
                break;
            }

            // calculate checksum
            char imei[16] = {0,};
            strncpy(imei, argv[1], 14);
            imei[14] = calcImeiChecksum(imei);

            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Set IMEI: %s\n", imei);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factorySetImei(imei);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_READ_QFUSING: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Read Qfusing\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factoryReadQfusing();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_WRITE_QFUSING: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Write Qfusing\n");
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factoryWriteQfusing();
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_GET_FID: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Get FID\n");
            printMessage("-------------------------------------------------------\n");
            sp<String> fid = TelephonyManager::factoryGetFid();
            printMessage(" -> FID : %s\n", CSTR(fid));
        } break;

        case CMD_FACTORY_SET_FID: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony factorySetFid <fid>\n"
                               "-------------------------------------------------------\n");
                break;
            }
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Set FID: %s\n", argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factorySetFid(argv[1]);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_SET_SW_COUNTRY_ID: {
            int32_t swId = 0;
            if (argc >= 2) {
                sp<String> co = new String(argv[1]);
                if (co->equalsIgnoreCase("NA")) {
                    swId = TelephonyManager::SW_COUNTRY_NA;
                } else if (co->equalsIgnoreCase("EU")) {
                    swId = TelephonyManager::SW_COUNTRY_EU;
                } else if (co->equalsIgnoreCase("CN")) {
                    swId = TelephonyManager::SW_COUNTRY_CN;
                } else if (co->equalsIgnoreCase("BR")) {
                    swId = TelephonyManager::SW_COUNTRY_BR;
                } else if (co->equalsIgnoreCase("RU")) {
                    swId = TelephonyManager::SW_COUNTRY_RU;
                } else {
                    // error
                }
            }

            if (swId == 0) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony factorySetCountryId (NA|EU|CN|BR|RU)\n"
                               "-------------------------------------------------------\n");
                break;
            }

            printMessage("-------------------------------------------------------\n");
            printMessage("    Set SW country id : %s\n", argv[1]);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factorySetSwCountryId(swId);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_FACTORY_SET_EFS_RECOVERY: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony factorySetEfsRecovery <backup>\n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t backup = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Factory Set EFS Recovery: %d\n", backup);
            printMessage("-------------------------------------------------------\n");
            bool result = TelephonyManager::factorySetEfsRecovery(backup);
            printMessage(" -> Result : %s\n", BOOL_STR(result));
        } break;

        case CMD_GET_CALL_REJECT_CLEARED: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get call reject cleared\n");
            printMessage("-------------------------------------------------------\n");
            bool clear = TelephonyManager::getCallRejectCleared();
            printMessage(" -> Call reject cleared : %d\n", clear ? 1 : 0);
        } break;

        case CMD_SET_CALL_REJECT_CLEARED: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setCallRejectClared (0|1)\n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t clear = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set call reject cleared: %d\n", clear);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setCallRejectCleared(0 != clear);
        } break;

        case CMD_GET_AUDIO_ENABLED: {
            printMessage("-------------------------------------------------------\n");
            printMessage("    Get audio enabled\n");
            printMessage("-------------------------------------------------------\n");
            bool enable = TelephonyManager::getAudioEnabled();
            printMessage(" -> Audio enabled : %d\n", enable ? 1 : 0);
        } break;

        case CMD_SET_AUDIO_ENABLED: {
            if (argc < 2) {
                printMessage( "-------------------------------------------------------\n"
                               " sldd telephony setAudioEnabled (0|1)\n"
                               "-------------------------------------------------------\n");
                break;
            }
            int32_t enable = sl::atoi(argv[1]);
            printMessage("-------------------------------------------------------\n");
            printMessage("    Set audio enabled: %d\n", enable);
            printMessage("-------------------------------------------------------\n");
            TelephonyManager::setAudioEnabled(1 == enable);
        } break;


        case CMD_GET_IMSA_SERVICE_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("	  Get IMS Service Status\n");
            printMessage("-------------------------------------------------\n");
            int32_t result = TelephonyManager::getIMSAServiceStatus();
            printMessage(" -> result : %d \n", result);
        } break;

        case CMD_GET_IMSA_REGISTRATION_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("	  Get IMS Registration Status\n");
            printMessage("-------------------------------------------------\n");
            int32_t result = TelephonyManager::getIMSARegistrationStatus();
            printMessage(" -> result : %d \n", result);
        } break;

        case CMD_GET_IMSA_PDP_STATUS: {
            printMessage("-------------------------------------------------\n");
            printMessage("	  Get IMS PDP Status\n");
            printMessage("-------------------------------------------------\n");
            int32_t result = TelephonyManager::getIMSAPDPStatus();
            printMessage(" -> result : %d \n", result);
        } break;

        default:
            printMessage("-------------------------------------------------------\n");
            printMessage(" %s : Unknown command\n", argv[0]);
            printMessage("-------------------------------------------------------\n");
            break;
    }

    return true;
}

static void usage_telephony(int32_t id) {
    if (id == 0) {
        printMessage(" sldd telephony [<command>] [parameters] \n");
        printMessage("    <command> \n");
        PRINT_USAGE(telephonyCommands);
    } else {
        printMessage( " sldd telephony %s", telephonyCommands[id-1].simple_usage);
    }
}

static int32_t getCommandId(char* string) {
    for (int32_t i=0; telephonyCommands[i].name != NULL; i++) {
        sp<String> name = new String(telephonyCommands[i].name);
        if (name->equalsIgnoreCase(string)) {
            return i+1;
        }
    }
    return -1;
}

static int32_t getHiddenCommandId(char* string) {
    const char* hiddenCommands[] = {
        "dump",

        "getFratTimer",
        "setFratTimer",
        "listen",

        "factoryGetRfcalCheck",
        "factoryGetDataCheck",
        "factorySetDataCheck",
        "factoryGetImei",
        "factorySetImei",
        "factoryReadQfusing",
        "factoryWriteQfusing",
        "factoryGetFid",
        "factorySetFid",
        "factorySetSwCountryId",
        "factorySetEfsRecovery",

        "getCallRejectCleared",
        "setCallRejectCleared",

        "getAudioEnabled",
        "setAudioEnabled",

        NULL
    };

    for (int32_t i=0; hiddenCommands[i] != NULL; i++) {
        sp<String> name = new String(hiddenCommands[i]);
        if (name->equalsIgnoreCase(string)) {
            return i+CMD_DUMP;
        }
    }

    return -1;
}

static const char getDtmfDigit(char* string) {
    char ch = string[0];
    if ((ch >= '0' && ch <= '9') || ch == '*' || ch == '#') {
        return ch;
    } else {
        return '0';
    }
}

static const char* getCallStateString(int32_t state) {
    switch (state) {
        case Call::IDLE: return "IDLE";
        case Call::ACTIVE: return "ACTIVE";
        case Call::HOLDING: return "HOLDING";
        case Call::DIALING: return "DIALING";
        case Call::ALERTING: return "ALERTING";
        case Call::INCOMING: return "INCOMING";
        case Call::WAITING: return "WAITING";
        case Call::DISCONNECTED: return "DISCONNECTED";
        case Call::DISCONNECTING: return "DISCONNECTING";
        default: break;
    }
    return "Unknown";
}

static const char* getNetworkTypeString(int32_t type) {
    switch (type) {
        case TelephonyManager::NETWORK_TYPE_GPRS: return "GPRS";
        case TelephonyManager::NETWORK_TYPE_EDGE: return "EDGE";
        case TelephonyManager::NETWORK_TYPE_UMTS: return "UMTS";
        case TelephonyManager::NETWORK_TYPE_CDMA: return "CDMA";
        case TelephonyManager::NETWORK_TYPE_EVDO_0: return "EVDO Rev.0";
        case TelephonyManager::NETWORK_TYPE_EVDO_A: return "EVDO Rev.A";
        case TelephonyManager::NETWORK_TYPE_1xRTT: return "1xRTT";
        case TelephonyManager::NETWORK_TYPE_HSDPA: return "HSDPA";
        case TelephonyManager::NETWORK_TYPE_HSUPA: return "HSUPA";
        case TelephonyManager::NETWORK_TYPE_HSPA: return "HSPA";
        case TelephonyManager::NETWORK_TYPE_IDEN: return "iDen";
        case TelephonyManager::NETWORK_TYPE_EVDO_B: return "EVDO Rev.B";
        case TelephonyManager::NETWORK_TYPE_LTE: return "LTE";
        case TelephonyManager::NETWORK_TYPE_EHRPD: return "eHRPD";
        case TelephonyManager::NETWORK_TYPE_HSPAP: return "HSPA+";
        default: return "UNKNOWN";
    }
}

static const char* getNetworkClassString(int32_t networkClass) {
    switch (networkClass) {
        case TelephonyManager::NETWORK_CLASS_2_G: return "2G";
        case TelephonyManager::NETWORK_CLASS_3_G: return "3G";
        case TelephonyManager::NETWORK_CLASS_4_G: return "4G";
        default: return "UNKNOWN";
    }
}

static const char* getPreferredNetworkTypeString(int32_t type) {
    switch (type) {
        case RILConstants::NETWORK_MODE_WCDMA_PREF: return "WCDMA pref";
        case RILConstants::NETWORK_MODE_GSM_ONLY: return "GSM only";
        case RILConstants::NETWORK_MODE_WCDMA_ONLY: return "WCDMA only";
        case RILConstants::NETWORK_MODE_GSM_UMTS: return "GSM & UMTS";
        case RILConstants::NETWORK_MODE_CDMA: return "CDMA";
        case RILConstants::NETWORK_MODE_CDMA_NO_EVDO: return "CDMA & no EVDO";
        case RILConstants::NETWORK_MODE_EVDO_NO_CDMA: return "EVDO & no CDMA";
        case RILConstants::NETWORK_MODE_GLOBAL: return "GLOBAL";
        case RILConstants::NETWORK_MODE_LTE_CDMA_EVDO: return "LTE, CDMA & EVDO";
        case RILConstants::NETWORK_MODE_LTE_GSM_WCDMA: return "LTE, GSM & WCDMA";
        case RILConstants::NETWORK_MODE_LTE_CMDA_EVDO_GSM_WCDMA: return "LTE, CDMA, EVDO, GSM & WCDMA";
        case RILConstants::NETWORK_MODE_LTE_ONLY: return "LTE only";
        case RILConstants::NETWORK_MODE_LTE_WCDMA: return "LTE & WCDMA";
        case RILConstants::NETWORK_MODE_TD_SCDMA_ONLY: return "TD-SDCAM only";
        case RILConstants::NETWORK_MODE_TD_SCDMA_WCDMA: return "TD-SDCAM & WCDMA";
        case RILConstants::NETWORK_MODE_TD_SCDMA_LTE: return "TD-SDCAM & LTE";
        case RILConstants::NETWORK_MODE_TD_SCDMA_GSM: return "TD-SDCAM & GSM";
        case RILConstants::NETWORK_MODE_TD_SCDMA_GSM_LTE: return "TD-SDCAM, GSM & LTE";
        case RILConstants::NETWORK_MODE_TD_SCDMA_GSM_WCDMA: return "TD-SDCAM, GSM, & WCDMA";
        case RILConstants::NETWORK_MODE_TD_SCDMA_WCDMA_LTE: return "TD-SDCAM, WCDMA & LTE";
        case RILConstants::NETWORK_MODE_TD_SCDMA_GSM_WCDMA_LTE: return "TD-SDCAM, GSM, WCDMA & LTE";
        default: break;
    }
    return "Unknown";
}

static const char* getNetRegStateString(int32_t state) {
    switch (state) {
        case ServiceState::RIL_REG_STATE_NOT_REG: return "NOT REGISTERED";
        case ServiceState::RIL_REG_STATE_HOME: return "REGISTERED, HOME";
        case ServiceState::RIL_REG_STATE_SEARCHING: return "NOT REGISTERED, SEARCHING";
        case ServiceState::RIL_REG_STATE_DENIED: return "REGISTRATION DENIED";
        case ServiceState::RIL_REG_STATE_UNKNOWN: return "UNKNOWN";
        case ServiceState::RIL_REG_STATE_ROAMING: return "REGISTERED, ROAMING";
        default: return "NOT AVAILABLE";
    }
}

static const char* getServiceStateString(int32_t state) {
    switch (state) {
        case ServiceState::STATE_IN_SERVICE: return "IN SERVICE";
        case ServiceState::STATE_OUT_OF_SERVICE: return "OUT OF SERVICE";
        case ServiceState::STATE_EMERGENCY_ONLY: return "EMERGENCY ONLY";
        case ServiceState::STATE_POWER_OFF: return "POWER OFF";
        default: break;
    }
    return "Unknown";
}

static const char* getSimStateString(int32_t state) {
    switch (state) {
        case TelephonyManager::SIM_STATE_UNKNOWN: return "UNKNOWN";
        case TelephonyManager::SIM_STATE_ABSENT: return "ABSENT";
        case TelephonyManager::SIM_STATE_PIN_REQUIRED: return "PIN_REQUIRED";
        case TelephonyManager::SIM_STATE_PUK_REQUIRED: return "PUK_REQUIRED";
        case TelephonyManager::SIM_STATE_NETWORK_LOCKED: return "NETWORK_LOCKED";
        case TelephonyManager::SIM_STATE_READY: return "READY";
        case TelephonyManager::SIM_STATE_NOT_READY: return "NOT_READY";
        case TelephonyManager::SIM_STATE_PERM_DISABLED: return "PERM_DISABLED";
        case TelephonyManager::SIM_STATE_CARD_IO_ERROR: return "CARD_IO_ERROR";
        default: break;
    }
    return "Unknown";
}

static const char* getSimProfileIdString(int32_t id) {
    if (id < TelephonyManager::SIM_PROFILE_ID_MIN || id > TelephonyManager::SIM_PROFILE_ID_MAX) {
        return "Invalid";
    }
    return "";
}

static const char* getSimProfileTypeString(int32_t type) {
    switch (type) {
        case TelephonyManager::SIM_PROFILE_TYPE_REGULAR: return "Regular";
        case TelephonyManager::SIM_PROFILE_TYPE_EMERGENCY: return "Emergency";
        default: break;
    }
    return "Invalid";
}

static const char* getPowerModeString(int32_t mode) {
    switch (mode) {
        case 0: return "Unpowered mode";
        case 1: return "Sleep mode";
        case 2: return "Polling mode 1";
        case 3: return "Polling mode 2";
        case 4: return "Listen mode 1";
        case 5: return "Listen mode 2";
        case 6: return "Normal mode";
        default: break;
    }
    return "Unknown";
}

static const char* getOperationModeString(int32_t mode) {
    switch (mode) {
        case 0: return "Deactivated mode";
        case 1: return "Preactivated mode";
        case 2: return "Activated Unprovisioned mode";
        case 3: return "Activated Provisioned mode";
        default: break;
    }
    return "Unknown";
}

static const char* getSpyModeString(int32_t mode) {
    switch (mode) {
        case 0: return "Disable";
        case 1: return "Enable";
        default: break;
    }
    return "Unknown";
}

static const char* getPinReturnResultString(int32_t result) {
    switch (result) {
        case 0: return "Succeeded";
        case 1: return "Password incorrect";
        case 2: return "Generic failure";
        case 3: return "Invalid length";
        default: break;
    }
    return "Unknown";
}

static const char* getSimRemainingCountTypeString(int32_t type) {
    switch (type) {
        case TelephonyManager::SIM_PIN1: return "PIN1";
        case TelephonyManager::SIM_PUK1: return "PUK1";
        case TelephonyManager::SIM_PIN2: return "PIN2";
        case TelephonyManager::SIM_PUK2: return "PUK2";
        default: break;
    }
    return "Unknown";
}

static const char* getSetActiveSimResultString(int32_t result) {
    switch (result) {
        case TelephonyManager::REMOTE_SIM_NO_ERROR: return "Succeeded";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_NOT_DETECTED: return "PSIM not detected";
        case TelephonyManager::REMOTE_SIM_ERROR_SWITCHING_IN_PROGRESS: return "Switching in progress";
        case TelephonyManager::REMOTE_SIM_ERROR_ALREADY_SET: return "Already set";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_NOT_EXIST: return "PSIM not exist";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_BLOCKED: return "PSIM blocked";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_LOCKED: return "PSIM locked";
        case TelephonyManager::REMOTE_SIM_ERROR_TSIM_LOCKED: return "TSIM locked";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_DISABLED: return "PSIM disabled";
        case TelephonyManager::REMOTE_SIM_ERROR_PSIM_ERROR: return "PSIM error";
        default: break;
    }
    return "Unknown";
}

static const char* getGetActiveSimResultString(int32_t activeSim) {
    switch (activeSim) {
        case TelephonyManager::REMOTE_SIM_TSIM: return "TSIM";
        case TelephonyManager::REMOTE_SIM_PSIM: return "PSIM";
        case TelephonyManager::REMOTE_SIM_ERROR_SWITCHING_IN_PROGRESS: return "In progress";
        default: break;
    }
    return "Unknown";
}

static const char* getPsimStateString(int32_t state) {
    switch (state) {
        case TelephonyManager::PSIM_STATE_DISABLED : return "Disabled";
        case TelephonyManager::PSIM_STATE_NOT_DETECTED: return "Not detected";
        case TelephonyManager::PSIM_STATE_INSERTED: return "Inserted";
        case TelephonyManager::PSIM_STATE_EJECTED: return "Ejected";
        case TelephonyManager::PSIM_STATE_ERROR: return "Error";
        default: break;
    }
    return "Unknown";
}

static const char* getSimSwitchingCauseString(int32_t cause) {
    switch (cause) {
        case TelephonyManager::REMOTE_SIM_CAUSE_UNKNOWN: return "Unknown";
        case TelephonyManager::REMOTE_SIM_CAUSE_NGI: return "NGI";
        case TelephonyManager::REMOTE_SIM_CAUSE_NLI: return "NLI";
        case TelephonyManager::REMOTE_SIM_CAUSE_NGTP: return "NGTP";
        case TelephonyManager::REMOTE_SIM_CAUSE_ECALL: return "ECALL";
        case TelephonyManager::REMOTE_SIM_CAUSE_BCALL: return "BCALL";
        case TelephonyManager::REMOTE_SIM_CAUSE_SVT: return "SVT";
        case TelephonyManager::REMOTE_SIM_CAUSE_REMOTESERVICE: return "REMOTESERVICE";
        case TelephonyManager::REMOTE_SIM_CAUSE_PROVISIONING: return "PROVISIONING";
        case TelephonyManager::REMOTE_SIM_CAUSE_HIGH_PRIORITY_APPS_END: return "HIGH_PRIORITY_APPS_END";
        case TelephonyManager::REMOTE_SIM_CAUSE_PSIM_EJECT: return "PSIM_EJECT";
        case TelephonyManager::REMOTE_SIM_CAUSE_NAD: return "NAD";
        case TelephonyManager::REMOTE_SIM_CAUSE_RETRY_STRATEGY: return "RETRY_STRATEGY";
        case TelephonyManager::REMOTE_SIM_CAUSE_DEBUG_MANAGER: return "DEBUG_MANAGER";
        case TelephonyManager::REMOTE_SIM_CAUSE_DIAG_MANAGER: return "DIAG_MANAGER";
        default: break;
    }
    return "Unknown";
}

static const char* getDataConnectionStateString(int32_t state) {
    switch (state) {
        case TelephonyManager::DATA_UNKNOWN: return "Unknown";
        case TelephonyManager::DATA_DISCONNECTED: return "Disconnected";
        case TelephonyManager::DATA_CONNECTING: return "Connecting";
        case TelephonyManager::DATA_CONNECTED: return "Connected";
        case TelephonyManager::DATA_SUSPENDED: return "Suspended";
        default: break;
    }
    return "Unknown";
}

static const char* getDataActivityString(int32_t state) {
    switch (state) {
        case TelephonyManager::DATA_ACTIVITY_NONE: return "NONE: No traffic";
        case TelephonyManager::DATA_ACTIVITY_IN: return "DATAIN: Receiving IP ppp traffic";
        case TelephonyManager::DATA_ACTIVITY_OUT: return "DATAOUT: Sending IP ppp traffic";
        case TelephonyManager::DATA_ACTIVITY_INOUT: return "DATAINANDOUT: Both receiving and sending IP ppp traffic";
        case TelephonyManager::DATA_ACTIVITY_DORMANT: return "DORMANT: The data connection is still active, but physical link is down";
        default: break;
    }
    return "Unknown";
}

static const char* getHwCountryIdString(int32_t hwId) {
    switch (hwId) {
        case TelephonyManager::HW_COUNTRY_NA: return "NA";
        case TelephonyManager::HW_COUNTRY_EU: return "EU";
        case TelephonyManager::HW_COUNTRY_CN: return "CN";
        default: break;
    }
    return "Unknown";
}

static const char* getSwCountryIdString(int32_t swId) {
    switch (swId) {
        case TelephonyManager::SW_COUNTRY_NA: return "NA";
        case TelephonyManager::SW_COUNTRY_EU: return "EU";
        case TelephonyManager::SW_COUNTRY_CN: return "CN";
        case TelephonyManager::SW_COUNTRY_BR: return "BR";
        case TelephonyManager::SW_COUNTRY_RU: return "RU";
        default: break;
    }
    return "Unknown";
}

static const char* getJammingDetectionStatusString(int32_t status) {
    switch (status) {
        case TelephonyManager::JAMMING_STATUS_NORMAL: return "Normal";
        case TelephonyManager::JAMMING_STATUS_HIGH:   return "High";
        default: break;
    }
    return "Unknown";
}

static const char* getTemperatureResultString(int32_t result) {
    switch (result) {
        case TelephonyManager::TEMPERATURE_RESULT_FAIL:     return "Fail";
        case TelephonyManager::TEMPERATURE_RESULT_SUCCESS:  return "Success";
        case TelephonyManager::TEMPERATURE_RESULT_ABNORMAL: return "Abnormal";
        default: break;
    }
    return "Unknown";
}

static int32_t calcImeiChecksum(const char* imei) {
    uint32_t len = ::strlen(imei);

    int32_t sum = 0;
    for(int32_t i=0; i<len; i++) {
        int32_t num = imei[i] - 0x30;
        if (i%2 == 0) {
            sum += num;
        } else {
            int32_t mul = 2 * num;
            sum += ((mul / 10) + (mul % 10));
        }
    }

    int32_t remainder = sum % 10;
    if (remainder != 0) {
        remainder = 10 - remainder;
    }

    return (remainder + 0x30);
}
