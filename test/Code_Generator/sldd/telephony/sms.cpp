
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>

#include "man.h"
#include "sldd_common.h"
#include "sms.h"
#include "telephony/telephony_common.h"

#define LOG_TAG "sldd_sms"

#if 0
#if defined (UZ_DBG)
#define UZ_LOG(...) RLOGD("UZ - "__VA_ARGS__)
#else
#define UZ_LOG(...) do {} while (0)
#endif
#endif

#define printMessage(...) (fprintf(stderr, __VA_ARGS__))

static int8_t sanityTestResult[20];
static int32_t curSanityTestCase = TC_NONE;

#define INIT_TEST_RESULT()              do { memset(sanityTestResult, 0xff, sizeof(sanityTestResult)/sizeof(int8_t));} while(0)
#define SET_TEST_RESULT(tc, result)     sanityTestResult[tc] = result
#define GET_TEST_RESULT(tc)             sanityTestResult[tc]
#define SET_CUR_TEST_RESULT(result)     sanityTestResult[curSanityTestCase] = result
#define GET_CUR_TEST_RESULT()           sanityTestResult[curSanityTestCase]

#define SET_NEXT_TC(tc)                 do { curSanityTestCase = tc;} while(0)
#define GET_CUR_TC()                    curSanityTestCase

static uint8_t binary_8bit_3seg[280] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, // 1 seg

    0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, // 2seg

    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, // 3 seg
};


class Waiter : public RefBase
{
public:
    Waiter() : mTimeout(180), mResult(0) {}
    virtual ~Waiter() {}

    inline void setTimeout(int32_t timeout) {
        mTimeout = timeout;
    }

    inline void setTimeout(int32_t timeout, int32_t result) {
        mTimeout = timeout;
        mResult = result;
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

class SlddSmsSendCallback : public BnSmsSendCallback
{
public:
    virtual ~SlddSmsSendCallback() {}

    virtual void onMessageSent(int32_t status, int32_t errorCode, int32_t msgId) {
        printMessage("Sending Completed\n");
        printMessage("Status : %d,     errorCode : %d     msgId : %d\n", status, errorCode, msgId);

        switch(GET_CUR_TC()) {
            case TC_MO_7BIT:        //FALL-THROUGH
            case TC_MO_8BIT:        //FALL-THROUGH
            case TC_MO_16BIT:       //FALL-THROUGH
            case TC_MO_CONCAT_8BIT: //FALL-THROUGH
            case TC_MO_CONCAT_16BIT://FALL-THROUGH
                sWaiter->setResult(status);
                sWaiter->notifyAll();
                break;
            default:
                break;
        }
    }
    virtual void onDeliveryReport(int32_t referenceNumber) {

    }

};

static sp<SlddSmsSendCallback> sSmsSendCallback = new SlddSmsSendCallback();

class SlddSmsReceiveCallback: public BnSmsReceiveCallback
{
public:
    explicit SlddSmsReceiveCallback(): mReceiverName(NULL) {}
    explicit SlddSmsReceiveCallback(const char* name) : mReceiverName(name) {}
    virtual ~SlddSmsReceiveCallback() {}

    virtual void onDataMessageReceived(const uint8_t* bytes, const size_t length, const char* origAddr) {
        printMessage("%s : Received Data Message\n", mReceiverName);

        sp<ByteArray> dataMsg = new ByteArray(bytes, length);
        sp<String> msgStr = dataMsg->toString();

        printMessage("data : %s\n", CSTR(msgStr));
        printMessage("originator : %s\n", origAddr);

        switch(GET_CUR_TC()) {
            case TC_MT_8BIT:
            {
                char* mmsNotiStr = "6d6d732e6e6174652e636f6d"; // mms.nate.com
                if (msgStr->contains(mmsNotiStr)) {
                    sWaiter->setResult(1);
                    sWaiter->notifyAll();
                }
            }
                break;
            default:
                break;
        }
    }

    virtual void onTextMessageReceived(const char* text, const char* origAddr) {
        printMessage("%s : Received Text Message\n", mReceiverName);
        printMessage("text : %s\n", text);
        printMessage("originator : %s\n", origAddr);

        bool bTestSuccess = false;

        switch(GET_CUR_TC()) {
            case TC_MT_7BIT:
                bTestSuccess = true;
                break;

            case TC_MT_16BIT:
                if (strlen(text) == 70UL && !strncmp(text, TEXT_16BIT_3SEG, 70UL))
                    bTestSuccess = true;
                break;

            case TC_MT_CONCAT_16BIT:
                if (!strcmp(text, TEXT_16BIT_3SEG))
                    bTestSuccess = true;
                break;

            default:
                break;
        }

        if (bTestSuccess) {
            sWaiter->setResult(1);
            sWaiter->notifyAll();
        }
    }

private:
    const char* mReceiverName;
};
static sp<SlddSmsReceiveCallback> sSmsReceiveCallback = new SlddSmsReceiveCallback("SmsReceiver1");
static sp<SlddSmsReceiveCallback> sSmsReceiveCallback2 = new SlddSmsReceiveCallback("SmsReceiver2");

sldd_cmd_table_t smsCmds[] = {
    /* -------------------- Open Command  ---------------------------- */
    {   NULL,               CMD_SMS_OPEN_START,         NULL,               NULL    },
    {   "sendSms",          CMD_SEND_SMS,               commandActionSms,
        "sendSms <number> <data> - send binary SMS\n"
    },
    {   "recvSms",          CMD_RECV_SMS,               commandActionSms,
        "recvSms - receive SMS\n"
    },
    {   "getSmsc",          CMD_GET_SMSC,               commandActionSms,
        "getSmsc - get SMS Center Address\n"
    },
    {   "setSmsc",          CMD_SET_SMSC,               commandActionSms,
        "setSmsc <number> - set SMS Center number\n"
    },
    {   NULL,               CMD_SMS_OPEN_END,           NULL,               NULL    },

    /* -------------------- Internel Command for test  -------------------- */
    {   NULL,               CMD_SMS_INTERNEL_START,     NULL,               NULL    },
    {   "echoTest",         CMD_SMS_ECHO_TEST,          commandActionSms,
        "echoTest - Echo Test\n"
    },
    {   "chkSmsSvc",        CMD_SMS_CHECK_SMS_SERVICE,  commandActionSms,
        "chkSmsSvc - Check SMS Service\n"
    },
    {   "sendSms7",         CMD_SEND_SMS7BIT,           commandActionSms,
        "sendSms7 - send 7bit SMS\n"
    },
    {   "sendSms16",        CMD_SEND_SMS16BIT,          commandActionSms,
        "sendSms16 - send 16bit SMS\n"
    },
    {   "setFakeMtMode",    CMD_SET_FAKE_MT_MODE,         commandActionSms,
        "setFakeMtMode <mode> <data> - Set Fake MT Mode\n"
    },
    {   "sanityTest",       CMD_DO_SANITY_TEST,         commandActionSms,
        "sanityTest - Do SMS Sanity Test\n"
    },
    {   NULL,               CMD_SMS_INTERNEL_END,       NULL,               NULL    }
};

static int32_t getCommandId(char* string);
static void usage_sms(int32_t id);
static void processCmdSendBinarySms(int32_t argc, char **argv);
static void processCmdReceiveSms(int32_t argc, char **argv);
static void processCmdGetSmsc(int32_t argc, char **argv);
static void processCmdSetSmsc(int32_t argc, char **argv);

static void processCmdEchoTest(int32_t argc, char **argv);
static void processCmdCheckSmsService(int32_t argc, char **argv);
static void processCmdSendTextSms(int32_t argc, char **argv);
static void processCmdSendTextSms16(int32_t argc, char **argv);
static void processCmdDoSanityTest(int32_t argc, char **argv);
static void processCmdSetFakeMtMode(int32_t argc, char **argv);


void register_sms() {
    registerCommands(MODULE_SMS, NULL, smsCmds);
}

bool commandActionSms(int32_t argc, char **argv) {

    if (argc < 1) {
        usage_sms(-1);
        return true;
    }

    int32_t id = getCommandId(argv[0]);

    switch(id) {
        case CMD_SEND_SMS:      processCmdSendBinarySms(argc, argv);    break;
        case CMD_RECV_SMS:      processCmdReceiveSms(argc, argv);       break;
        case CMD_GET_SMSC:      processCmdGetSmsc(argc, argv);          break;
        case CMD_SET_SMSC:      processCmdSetSmsc(argc, argv);          break;
        case CMD_SMS_ECHO_TEST:         processCmdEchoTest(argc, argv);         break;
        case CMD_SMS_CHECK_SMS_SERVICE: processCmdCheckSmsService(argc, argv);  break;
        case CMD_SEND_SMS7BIT:          processCmdSendTextSms(argc, argv);      break;
        case CMD_SEND_SMS16BIT:         processCmdSendTextSms16(argc, argv);    break;
        case CMD_SET_FAKE_MT_MODE:  processCmdSetFakeMtMode(argc, argv);    break;
        case CMD_DO_SANITY_TEST:        processCmdDoSanityTest(argc, argv);     break;

        default:
            printMessage("-------------------------------------------------\n");
            printMessage(" %s : Unknown command\n", argv[0]);
            printMessage("-------------------------------------------------\n");
            break;
    }

    return true;
}

static void usage_sms(int32_t id) {
    if (id < 0) {
        printMessage(" sldd sms [<command>] [parameters] \n");
        for (int32_t i = CMD_SMS_OPEN_START+1; i < CMD_SMS_OPEN_END; i++) {
            printMessage("  [command] %s \n", smsCmds[i].name );
            printMessage("    [usage]\n %s \n", smsCmds[i].simple_usage );
        }
    } else {
        printMessage("[usage]");
        printMessage(" sldd sms %s", smsCmds[id].simple_usage);
    }
}

static int32_t getCommandId(char* string) {
    int32_t ret = -1;

    if(string == NULL)
        return -1;

    sp<String> inCmd = new String(string);

    for (int32_t i=CMD_SMS_OPEN_START; i < CMD_SMS_INTERNEL_END; i++) {
        if(smsCmds[i].name !=NULL && inCmd->equalsIgnoreCase(smsCmds[i].name)) {
            ret = smsCmds[i].id;
            break;
        }
    }

    return ret;
}

static void processCmdSendBinarySms(int32_t argc, char **argv) {
    if (argc > 4) {
        usage_sms(CMD_SEND_SMS);
        return;
    }

    printMessage("-------------------------------------------------\n");
    printMessage("    Send binary SMS\n");
    printMessage("-------------------------------------------------\n");

    int32_t ret;

    ret = SmsManager::registerReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    //ret = SmsManager::registerReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);

    sWaiter->setTimeout(MAX_TIMEOUT_MO);

#if 0
    uint8_t tempData[140] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b
    };
#endif

    sp<String> sca = SmsManager::getServiceCenterAddress();

    if (argc == 1) {
        printMessage("send a 8bit test message to myself...\n");
        for (int32_t i=0; i<1; i++) {
            ret = SmsManager::sendSms(CSTR(TelephonyManager::getMsisdn()), CSTR(sca), 0, binary_8bit_3seg, sizeof(binary_8bit_3seg), sSmsSendCallback); // SKT TEST
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else if (argc == 2){
        printMessage("send a 8bit test message to %s\n", argv[1]);
        ret = SmsManager::sendSms(argv[1], CSTR(sca), 0, binary_8bit_3seg, sizeof(binary_8bit_3seg), sSmsSendCallback); // SKT TEST
        printMessage("ret sendSmsTest Message ID : %d\n", ret);
    } else if (argc == 3){
        // sldd sms sendSms +821049263797 4A4C52
        sp<String> byteChar = new String(argv[2]);
        sp<ByteArray> byteArr = IccUtils::hexStringToBytes(byteChar);
        printMessage("send a binary message to %s\n", argv[1]);
        ret = SmsManager::sendSms(argv[1], CSTR(sca), 0, byteArr->getBytes(), byteArr->size(), sSmsSendCallback);
        printMessage("ret sendSms Message ID : %d\n", ret);
    } else if (argc == 4){
        // sldd sms sendSms +821049263797 +82100099104926 4A4C52
        sp<String> byteChar = new String(argv[3]);
        sp<ByteArray> byteArr = IccUtils::hexStringToBytes(byteChar);
        ret = SmsManager::sendSms(argv[1], argv[2], 0, byteArr->getBytes(), byteArr->size(), sSmsSendCallback);
        printMessage("ret sendSmsTest Message ID : %d\n", ret);
    } else {
        printMessage("  %s\n", STR_INVALID_ARGUMENTS);
    }
    printMessage("wait for sending result...\n");
    sWaiter->wait();
    if (sWaiter->getResult() == 0) {
        printMessage(" Timed out\n");
    }

    ret = SmsManager::unregisterReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    //ret = SmsManager::unregisterReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);

}

static void processCmdReceiveSms(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Receiving SMS. Wait...\n");
    printMessage("-------------------------------------------------\n");

    int32_t ret;

    ret = SmsManager::registerReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    ret = SmsManager::registerReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_ERA_GLONASS);

    //SmsManager::recvSmsTest();

    sWaiter->setTimeout(MAX_TIMEOUT_MT);
    sWaiter->wait();
    if (sWaiter->getResult() == 0) {
        printMessage(" Timed out\n");
    }

    ret = SmsManager::unregisterReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    ret = SmsManager::unregisterReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);
}

static void processCmdGetSmsc(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Get SMS Center Address\n");
    printMessage("-------------------------------------------------\n");

    sp<String> ret = SmsManager::getServiceCenterAddress();
    printMessage("SMS center address : %s\n", ret->c_str());
}

static void processCmdSetSmsc(int32_t argc, char **argv) {
    if (argc != 2) {
        printMessage("  %s\n", STR_INVALID_ARGUMENTS);
        usage_sms(CMD_SET_SMSC);
        return;
    }

    printMessage("-------------------------------------------------\n");
    printMessage("    Set SMS Center Address\n");
    printMessage("-------------------------------------------------\n");

    bool ret = SmsManager::setServiceCenterAddress(argv[1]);
    printMessage("result : %s\n", ret ? STR_SUCCESS : STR_FAIL);
}

static void processCmdEchoTest(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    SMS Echo Test\n");
    printMessage(" -> message : %s\n", argv[1]);
    printMessage("-------------------------------------------------\n");
}

static void processCmdCheckSmsService(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Check SMS Service\n");
    printMessage("-------------------------------------------------\n");
    int32_t cmdParam = 0L;

    if (argc == 2 )
        cmdParam = sl::atoi(argv[1]);

    sp<String> msgSmsService = SmsManager::checkSmsService(cmdParam);
    printMessage(" -> message from SMS Service : %s\n", CSTR(msgSmsService));
}

static void processCmdSendTextSms(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Sending 7bit SMS Test\n");
    printMessage("-------------------------------------------------\n");

    int32_t nCount = 1;

    if (argc > 1) {
        nCount = sl::atoi(argv[1]);

        if (nCount > 100) {
            printMessage("Count is over !!\n");
            return;
        }
    }

    int32_t ret = SmsManager::registerReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    //ret = SmsManager::registerReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);
    sp<String> sca = SmsManager::getServiceCenterAddress();

    sWaiter->setTimeout(MAX_TIMEOUT_MO);

    if (argc <= 2) {
        // sldd sms sendSms7 [Count]
        // sldd sms sendSms7 1
        printMessage("send 7bit test messages to myself...\n");

        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(CSTR(TelephonyManager::getMsisdn()), CSTR(sca), "7bit dummy text",sSmsSendCallback, true); // SKT TEST
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else if (argc == 3){
        // sldd sms sendSms7 [Count][Dest]
        printMessage("send a 7bit test message to %s\n", argv[2]);
        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(argv[2], CSTR(sca), "7bit dummy text",sSmsSendCallback, true);
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else if (argc == 4){
        // sldd sms sendSms7 [Count][Dest][Sca]
        // sldd sms sendSms7 1 +821049263797 +82100099104926
        printMessage("send a 7bit test message to %s sca : %s\n", argv[2], argv[3]);
        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(argv[2], argv[3], "7bit_text", sSmsSendCallback, true);
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else {
        printMessage("Invalid parameters !!\n");
        goto EXIT;
    }

    printMessage("wait for sending result...\n");
    sWaiter->wait();
    if (sWaiter->getResult() == 0) {
        printMessage(" Timed out\n");
    }

EXIT:
    ret = SmsManager::unregisterReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    //ret = SmsManager::unregisterReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);
}

static void processCmdSendTextSms16(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Sending 16bit SMS Test\n");
    printMessage("-------------------------------------------------\n");

    int32_t nCount = 1;

    if (argc > 1) {
        nCount = sl::atoi(argv[1]);

        if (nCount > 100) {
            printMessage("Count is over !!\n");
            return;
        }
    }

    int32_t ret = SmsManager::registerReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    sp<String> sca = SmsManager::getServiceCenterAddress();
    // 5 segments
    //char sampleText[] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    // 3 segments
    //char sampleText[] = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    // 2 segments
    //char sampleText[] = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890ABCD";
    // 1 segments
    //char sampleText[70 + 1] = "012345678901234567890123456789012345678901234567890123456789ABCDEFGHIJ";
    char* sampleText = TEXT_16BIT_3SEG;


    sWaiter->setTimeout(MAX_TIMEOUT_MO);

    if (argc <= 2) {
        // sldd sms sendSms16 [Count]
        // sldd sms sendSms16 1
        printMessage("send 16bit test messages to myself...\n");

        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(CSTR(TelephonyManager::getMsisdn()), CSTR(sca), sampleText,sSmsSendCallback, false);
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else if (argc == 3){
        // sldd sms sendSms16 [Count][Dest]
        printMessage("send a 16bit test message to %s\n", argv[2]);
        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(argv[1], CSTR(sca), sampleText,sSmsSendCallback, false);
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else if (argc == 4){
        // sldd sms sendSms16 [Count][Dest][Text]
        // sldd sms sendSms16 1 +82100099104926 0123456789012345678901234567890123456789012345678901234567890123456789ABCDEFGHIJ
        printMessage("send a 16bit test message to %s\n", argv[2]);
        for (int32_t i=0; i<nCount; i++) {
            ret = SmsManager::sendSmsText(argv[2], CSTR(sca), argv[3], sSmsSendCallback, false);
            printMessage("ret sendSmsTest Message ID : %d\n", ret);
        }
    } else {
        printMessage("Invalid parameters !!\n");
        goto EXIT;
    }

    printMessage("wait for sending result...\n");
    sWaiter->wait();
    if (sWaiter->getResult() == 0) {
        printMessage(" Timed out\n");
    }

EXIT:
    ret = SmsManager::unregisterReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    //ret = SmsManager::unregisterReceiver(sSmsReceiveCallback2, ID_SMS_RECEIVER_DEFAULT);
}

static void processCmdSetFakeMtMode(int32_t argc, char **argv) {
    int32_t ret = -1;

    if (argc > 3) {
        printMessage("  %s\n", STR_INVALID_ARGUMENTS);
        usage_sms(CMD_SET_FAKE_MT_MODE);
        return;
    }

    printMessage("-------------------------------------------------\n");
    printMessage("    Set SMS MT Mode\n");
    printMessage("-------------------------------------------------\n");



    // sldd sms sendSms +821049263797 4A4C52
    bool mode = (sl::atoi(argv[1]) == 1) ? true : false;

    if (mode) {
        sp<String> byteChar = new String(argv[2]);
        sp<ByteArray> byteArr = NULL;
        byteArr = IccUtils::hexStringToBytes(byteChar);
        printMessage("set Fake Data %s\n", argv[2]);
        ret = SmsManager::setFakeMTSms(mode, byteArr->getBytes(), byteArr->size());
    } else {
        ret = SmsManager::setFakeMTSms(mode, NULL, 0UL);
    }


    printMessage("result : %s\n", ret ? STR_SUCCESS : STR_FAIL);
}

/*----------------------------------------------------------------------------*/
/*                  Sanity Test Functions                                                                                     */
/*----------------------------------------------------------------------------*/

static char* tcNameToString(int32_t tc) {
    switch(tc) {
        case TC_START:                  return "Sanity Test Start";
        case TC_CHECK_PRECONDITINON:    return "TC_CHECK_PRECONDITINON";
        case TC_MO_7BIT:                return "TC_MO_7BIT";
        case TC_MT_7BIT:                return "TC_MT_7BIT";
        case TC_MO_8BIT:                return "TC_MO_8BIT";
        case TC_MT_8BIT:                return "TC_MT_8BIT";
        case TC_MO_16BIT:               return "TC_MO_16BIT";
        case TC_MT_16BIT:               return "TC_MT_16BIT";
        case TC_MO_CONCAT_8BIT:         return "TC_MO_CONCAT_8BIT";
        case TC_MT_CONCAT_8BIT:         return "TC_MT_CONCAT_8BIT";
        case TC_MO_CONCAT_16BIT:        return "TC_MO_CONCAT_16BIT";
        case TC_MT_CONCAT_16BIT:        return "TC_MT_CONCAT_16BIT";
        case TC_MOMT_STRESS:            return "TC_MOMT_STRESS";
        case TC_SMSC_GET:               return "TC_SMSC_GET";
        case TC_SMSC_SET:               return "TC_SMSC_SET";
        case TC_END:                    return "Sanity Test End";
        default:                        return "TC_UNKNOWN";
    }
}

static void printSanityTestResult() {
    printMessage("\n-------------------------------------------------\n");
    printMessage("    < SMS Sanity Test Result >\n\n");

    int32_t idx = 1;

    for (int32_t i = TC_CHECK_PRECONDITINON; i < TC_END; i++) {
        char *strResult = NULL;
        if (sanityTestResult[i] == TC_PASS)
            strResult = "O";
        else if (sanityTestResult[i] == TC_FAIL)
            strResult = "X";
        else
            strResult = "Not tested";
        printMessage("%d. %s :   %s\n", idx++, tcNameToString(i), strResult);
    }
}

static bool isAvailableTestCondition(sp<String>& sca, sp<String>& msisdn) {
    bool isAvailable = true;
    int32_t ss = TelephonyManager::getServiceState();

    sp<String> simOperator = TelephonyManager::getSimOperator();

    if (!simOperator->startsWith("450")) {
        printMessage("Not Korean SIM card\n");
    }

    if (ss != ServiceState::STATE_IN_SERVICE) {
        printMessage("Radio is not available\n");
        isAvailable = false;
    }

    if (sca == NULL || sca->isEmpty()) {
        printMessage("Invalid service center address\n");
        isAvailable = false;
    }

    if (msisdn == NULL || msisdn->isEmpty()) {
        printMessage("Invalid msisdn\n");
        isAvailable = false;
    }

    SET_CUR_TEST_RESULT((isAvailable) ? TC_PASS : TC_FAIL);

    return isAvailable;
}

static bool isTcPassed(int32_t tc, int32_t result) {
    bool isPassed = false;

    switch(tc) {
        case TC_MO_7BIT:        //FALL-THROUGH
        case TC_MO_8BIT:        //FALL-THROUGH
        case TC_MO_16BIT:       //FALL-THROUGH
        case TC_MO_CONCAT_8BIT: //FALL-THROUGH
        case TC_MO_CONCAT_16BIT://FALL-THROUGH
            if (result == 0)
                 isPassed = true;
            break;

        case TC_MT_7BIT:        //FALL-THROUGH
        case TC_MT_8BIT:        //FALL-THROUGH
        case TC_MT_16BIT:       //FALL-THROUGH
        case TC_MT_CONCAT_8BIT: //FALL-THROUGH
        case TC_MT_CONCAT_16BIT://FALL-THROUGH
            if (result == 1)
                isPassed = true;
            break;

        default:
            break;
    }

    return isPassed;
}

static bool doMoTest(int32_t tc, sp<String>& sca, sp<String>& msisdn) {
    RLOGD("doMoTest : %s", tcNameToString(tc));

    bool isPassed = false;
    int32_t sentRet = -1;
    int32_t waitRet = -1;

    sleep(1);

    switch(tc) {
        case TC_MO_7BIT:
            sentRet = SmsManager::sendSmsText(CSTR(msisdn), CSTR(sca), "7bit dummy text",sSmsSendCallback, true); // SKT TEST
            break;

        case TC_MO_8BIT:
            sentRet = SmsManager::sendSms(CSTR(msisdn), CSTR(sca), 0, binary_8bit_3seg, 140, sSmsSendCallback);
            break;

        case TC_MO_16BIT:
        {
            char sampleText16[70+1] = {0};
            strncpy(sampleText16, TEXT_16BIT_3SEG, 70);
            sentRet = SmsManager::sendSmsText(CSTR(TelephonyManager::getMsisdn()), CSTR(sca), sampleText16, sSmsSendCallback, false);
        }
            break;

        case TC_MO_CONCAT_8BIT:
            sentRet = SmsManager::sendSms(CSTR(msisdn), CSTR(sca), 0, binary_8bit_3seg, sizeof(binary_8bit_3seg), sSmsSendCallback);
            break;

        case TC_MO_CONCAT_16BIT:
            sentRet = SmsManager::sendSmsText(CSTR(TelephonyManager::getMsisdn()), CSTR(sca), TEXT_16BIT_3SEG, sSmsSendCallback, false);
            break;

        case TC_MOMT_STRESS:
            break;

        default:
            RLOGD("UNKNOWN TC");
            return false;
    }

    if (sentRet > 0) {
        sWaiter->setTimeout(MAX_TIMEOUT_MO, RESULT_TIMEOUT);
        sWaiter->wait();
        waitRet = sWaiter->getResult();
        isPassed = isTcPassed(tc, waitRet);
    }

    SET_CUR_TEST_RESULT((isPassed) ? TC_PASS : TC_FAIL);

    return isPassed;
}

static bool doMtTest(int32_t tc) {
    RLOGD("doMtTest : %s", tcNameToString(tc));

    bool isPassed = false;
    int32_t waitRet = -1;

    if (tc == TC_MT_CONCAT_8BIT) {
        int8_t ret8Bit = GET_TEST_RESULT(TC_MT_8BIT);
        isPassed = (ret8Bit == TC_PASS) ? true : false;
        SET_CUR_TEST_RESULT(ret8Bit);
    } else {
        sWaiter->setTimeout(MAX_TIMEOUT_MT, RESULT_TIMEOUT);
        sWaiter->wait();
        waitRet = sWaiter->getResult();
        isPassed = isTcPassed(tc, waitRet);
        SET_CUR_TEST_RESULT((isPassed) ? TC_PASS : TC_FAIL);
    }

    return isPassed;
}

static bool doSmscTest(int32_t tc, sp<String>& sca) {
    bool isPassed = false;

    if (tc == TC_SMSC_GET) {
        isPassed = true;
    } else if (tc == TC_SMSC_SET) {
        bool ret = SmsManager::setServiceCenterAddress("+8210123456789");
        if (ret) {
            sp<String> newSca = SmsManager::getServiceCenterAddress();
            if (newSca->equals("+8210123456789")) {
                ret = SmsManager::setServiceCenterAddress(sca->c_str());
                sp<String> oldSca = SmsManager::getServiceCenterAddress();
                if (oldSca->equals(sca))
                    isPassed = true;
            }
        }
    } else {
        isPassed = false;
    }

    SET_CUR_TEST_RESULT((isPassed) ? TC_PASS : TC_FAIL);

    return isPassed;
}

static void moveNextTc(bool success) {
    int32_t curTc = GET_CUR_TC();

    switch(curTc) {
        case TC_START:
            SET_NEXT_TC(TC_CHECK_PRECONDITINON);
            break;

        case TC_CHECK_PRECONDITINON:
            if (success)
                SET_NEXT_TC(TC_MO_7BIT);
            else
                SET_NEXT_TC(TC_END);
            break;

        case TC_MO_7BIT:
            if (success)
                SET_NEXT_TC(TC_MT_7BIT);
            else
                SET_NEXT_TC(TC_MO_8BIT);
            break;

        case TC_MT_7BIT:
            SET_NEXT_TC(TC_MO_8BIT);
            break;

        case TC_MO_8BIT:
            if (success) {
                sleep(10);
            }
            SET_NEXT_TC(TC_MT_8BIT);
            break;

        case TC_MT_8BIT:
            SET_NEXT_TC(TC_MO_16BIT);
            break;

        case TC_MO_16BIT:
            if (success)
                SET_NEXT_TC(TC_MT_16BIT);
            else
                SET_NEXT_TC(TC_MO_CONCAT_8BIT);
            break;

        case TC_MT_16BIT:
            SET_NEXT_TC(TC_MO_CONCAT_8BIT);
            break;

        case TC_MO_CONCAT_8BIT:
            if (success) {
                sleep(20U);
            }
            SET_NEXT_TC(TC_MT_CONCAT_8BIT);
            break;

        case TC_MT_CONCAT_8BIT:
            SET_NEXT_TC(TC_MO_CONCAT_16BIT);
            break;

        case TC_MO_CONCAT_16BIT:
            SET_NEXT_TC(TC_MT_CONCAT_16BIT);
            break;

        case TC_MT_CONCAT_16BIT:
            SET_NEXT_TC(TC_SMSC_GET);
            break;

        case TC_MOMT_STRESS:
            break;

        case TC_SMSC_GET:
            SET_NEXT_TC(TC_SMSC_SET);
            break;

        case TC_SMSC_SET:
            SET_NEXT_TC(TC_END);
            break;

        case TC_END:
            break;

        default:
            break;
    }

}

static void processCmdDoSanityTest(int32_t argc, char **argv) {
    printMessage("-------------------------------------------------\n");
    printMessage("    Do SMS Sanity Test\n");
    printMessage("-------------------------------------------------\n");

    bool bFinished = false;
    int32_t sentRet = -1;
    int32_t waitRet = -1;
    int32_t curTc = TC_NONE;
    int32_t ret = SmsManager::registerReceiver(sSmsReceiveCallback, ID_SMS_RECEIVER_DEFAULT);
    sp<String> sca = SmsManager::getServiceCenterAddress();
    sp<String> msisdn = TelephonyManager::getMsisdn();

    //scanf("%d", &input);
    //printMessage("input value : %d\n", input);

    SET_NEXT_TC(TC_START);

    while(!bFinished) {
        curTc = GET_CUR_TC();
        printMessage("\n======= %s Testing...\n", tcNameToString(curTc));

        switch(curTc) {
            case TC_START:
                INIT_TEST_RESULT();
                moveNextTc(true);
                break;

            case TC_CHECK_PRECONDITINON:
                moveNextTc(isAvailableTestCondition(sca, msisdn));
                break;

            case TC_MO_7BIT:        //FALL-THROUGH
            case TC_MO_8BIT:        //FALL-THROUGH
            case TC_MO_16BIT:       //FALL-THROUGH
            case TC_MO_CONCAT_8BIT: //FALL-THROUGH
            case TC_MO_CONCAT_16BIT://FALL-THROUGH
                moveNextTc(doMoTest(curTc, sca, msisdn));
                break;

            case TC_MT_7BIT:        //FALL-THROUGH
            case TC_MT_16BIT:       //FALL-THROUGH
            case TC_MT_CONCAT_8BIT: //FALL-THROUGH
            case TC_MT_CONCAT_16BIT://FALL-THROUGH
                moveNextTc(doMtTest(curTc));
                break;

            case TC_MT_8BIT:
            {
                printMessage("\n<8bit MT Test>\n");
                printMessage("1 : Run test (press 1 and please send a MMS to %s)\n", CSTR(msisdn));
                printMessage("2 : Skip test \n");
                printMessage("select & enter : ");

                int32_t input = 0;
                bool bRetTest = false;
                scanf("%d", &input);

                if (input == 1) {
                    bRetTest = doMtTest(curTc);
                }
                moveNextTc(bRetTest);
            }
                break;

            case TC_MOMT_STRESS:
                break;

            case TC_SMSC_GET:
                doSmscTest(curTc, sca);
                SET_NEXT_TC(TC_SMSC_SET);
                break;

            case TC_SMSC_SET:
                doSmscTest(curTc, sca);
                SET_NEXT_TC(TC_END);
                break;

            case TC_END:
                bFinished = true;
                break;

            default:
                break;
        }
    }

    printSanityTestResult();
    printMessage("\n=== SMS Sanity Test Finish ===\n");

}
