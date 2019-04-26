
#ifndef SLDD_SMS_H
#define SLDD_SMS_H

#define MODULE_SMS "sms"

enum Commands {
    CMD_SMS_OPEN_START,         // Open Command
    CMD_SEND_SMS,
    CMD_RECV_SMS,
    CMD_GET_SMSC,
    CMD_SET_SMSC,
    CMD_SMS_OPEN_END,
    CMD_SMS_INTERNEL_START,     // Internel Command for test
    CMD_SMS_ECHO_TEST,
    CMD_SMS_CHECK_SMS_SERVICE,
    CMD_SEND_SMS7BIT,
    CMD_SEND_SMS16BIT,
    CMD_SET_FAKE_MT_MODE,
    CMD_DO_SANITY_TEST,
    CMD_SMS_INTERNEL_END
};

#define STR_SUCCESS                 "SUCCESS"
#define STR_FAIL                    "FAIL"
#define STR_INVALID_ARGUMENTS       "INVALID ARGUMENTS"


#define MAX_TIMEOUT_MO      (30)
#define MAX_TIMEOUT_MT      (60)
#define RESULT_TIMEOUT      (-9999)

enum {
    TC_NONE        = -1,
    TC_START       = 0,
    TC_CHECK_PRECONDITINON,
    TC_MO_7BIT,
    TC_MT_7BIT,
    TC_MO_8BIT,
    TC_MT_8BIT,
    TC_MO_16BIT,
    TC_MT_16BIT,
    TC_MO_CONCAT_8BIT,
    TC_MT_CONCAT_8BIT,
    TC_MO_CONCAT_16BIT,
    TC_MT_CONCAT_16BIT,
    TC_MOMT_STRESS,
    TC_SMSC_GET,
    TC_SMSC_SET,
    TC_END
};

#define TC_PASS     (1)
#define TC_FAIL     (0)
#define TEXT_16BIT_3SEG     ((const char*)"0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901")

extern void register_sms();
extern bool commandActionSms(int32_t argc, char **argv);

#endif /* SLDD_SMS_H */
