/**
* \file    sldd.cpp
* \brief     Implementation of sldb
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.09.01
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <cstdio>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Condition.h>
#include <utils/Mutex.h>
#include <utils/Thread.h>


//#include "include/utils/SystemProperty.h"
#include "man.h"
#include "sldd_common.h"
#include "cansignal.h"
#include "AlarmManager.h"
#include "AppManager.h"
#include "AudioManager.h"
#include "CommunicationManager.h"
#include "ConfigurationManager.h"
#include "LocationManager.h"
#include "PowerManager.h"
#include "SystemManager.h"
#include "WatchdogManager.h"
#include "telephony/data.h"
#include "telephony/sms.h"
#include "telephony/telephony.h"
#include "systemprop.h"

#define ENABLE_STACK_TRACE
#if defined(ENABLE_STACK_TRACE)
#include <execinfo.h>
#include <signal.h>
#include <sys/types.h>
#include <cxxabi.h>
#define max_frames 256
#endif

#define LOG_TAG "sldd"
#define LOGH_TAG "sldd_cmd"

//#######################################################################################
//# Constant Define
//#######################################################################################

typedef enum {
    TYPE_ACCELOMETER = 0,
} cmd_type_t;

typedef enum {
    ACTION_SET_ENABLE = 0,
    ACTION_SET_DISABLE
} cmd_action_t;

//typedef struct {
//} action_table_t;

typedef void (*commandAction)(int32_t type, int32_t action);
typedef bool (*commandAction2)(int32_t argc, char **argv);

typedef struct {
    char* module;
    commandAction2 action;
} action2_table_t;

action2_table_t action2_table[] = {
    {MODULE_MAN, commandActionMAN},
    {MODULE_CAN, commandActionCAN},
    {MODULE_TELEPHONY, commandActionTelephony},
    {MODULE_SMS, commandActionSms},
    {MODULE_DATA, commandActionData},
    {MODULE_ALARM_MANAGER, commandActionAlarm},
    {MODULE_APP_MANAGER, commandActionApplication},
    {MODULE_AUDIO_MANAGER, commandActionAudio},
    {MODULE_COMMUNICATION_MANAGER, commandActionCommunication},
    {MODULE_CONFIGURATION_MANAGER, commandActionConfiguration},
    {MODULE_LOCATION_MANAGER, commandActionLocation},
    {MODULE_POWER_MANAGER, commandActionPower},
    {MODULE_SYSTEM_MANAGER, commandActionSystem},
    {MODULE_WATCHDOG_MANAGER, commandActionWatchdog},
    {MODULE_SYSTEM_PROP, commandActionPROP},
    {NULL, NULL}
};

#if defined(ENABLE_STACK_TRACE)
static void print_trace_demangling() {

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    uint32_t addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
    printf("empty back trace log");
    return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256UL;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char *p = symbollist[i]; *p; ++p)
    {
        if (*p == '(')
        begin_name = p;
        else if (*p == '+')
        begin_offset = p;
        else if (*p == ']' && begin_offset) {
        end_offset = p;
        break;
        }
        else
        {
            /// do nothing
        }
    }

    if (begin_name && begin_offset && end_offset
        && begin_name < begin_offset)
    {
        *begin_name++ = '\0';
        *begin_offset++ = '\0';
        *end_offset = '\0';

        // mangled name is now in [begin_name, begin_offset) and caller
        // offset in [begin_offset, end_offset). now apply
        // __cxa_demangle():

        int status;
        char* ret = abi::__cxa_demangle(begin_name,
                        funcname, &funcnamesize, &status);
        if (status == 0) {
        funcname = ret; // use possibly realloc()-ed string
        printf( "  %s : %s+%s]\n",
            symbollist[i], funcname, begin_offset);
        }
        else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        printf( "  %s : %s()+%s]\n",
            symbollist[i], begin_name, begin_offset);
        }
    }
    else
    {
        // couldn't parse the line? print the whole line.
        printf( "  %s\n", symbollist[i]);
    }
    }

    free(funcname);
    free(symbollist);
}
static void debugSigHandler(int n, siginfo_t* info, void* unused __attribute__((unused))) {
    print_trace_demangling();      /* printk call stack */

    signal(n, SIG_DFL);

    switch (n) {
        case SIGABRT:
        case SIGFPE:
        case SIGPIPE:
            //(void) tgkill(getpid(), gettid(), n);
            break;
        default:    // SIGILL, SIGBUS, SIGSEGV
            break;
    }
}

static void registerSigHandler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = debugSigHandler;
    sa.sa_flags = 0;

    sigaction(SIGILL, &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
    sigaction(SIGBUS, &sa, NULL);
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
}
#endif

void usage_man();

//#######################################################################################
//# Structure Define
//#######################################################################################

class CommanderClient : public RefBase
{
public:
    CommanderClient() {

    }
    virtual ~CommanderClient() {}

    bool init()
    {
        register_can();
        register_telephony();
        register_sms();
        register_data();
        register_alarm();
        register_application();
        register_Audio();
        register_location();
        register_communication();
        register_configuration();
        register_system();
        register_power();
        register_Watchdog();
        register_sysprop();
        return check_application();
    }

    void act(commandAction fn, cmd_type_t type, cmd_action_t action)
    {
        fn(type, action);
    }

    void act2(commandAction2 fn, int32_t argc, char **argv)
    {
        if(fn(argc, argv) == false)
        {

            if (argv[0] != NULL && !strcmp(argv[0], MODULE_MAN))
            {
                usage_man();
                return;
            }
            printMessage(
                "==========================================\n"
                "            FAILED                       \n"
                "==========================================\n"
            );
        }
    }
};

sp<CommanderClient> sClient;

#ifdef ENABLE_HMI_MGR
void commandActionHMI(int32_t type, int32_t action)
{
    sp<IHMIManagerService> m;
    m = interface_cast<IHMIManagerService>
        (defaultServiceManager()->getService(String16(HMI_SRV_NAME)));

    switch(type)
    {
        case TYPE_ACCELOMETER:
            if(action == ACTION_SET_ENABLE)
            {
                m->enableAccelerometer();
            }
            else if(action == ACTION_SET_DISABLE)
            {
                m->disableAccelerometer();
            }
            else
            {
            }
            break;

        default:
            break;
    }
}
#endif

void make_history(int32_t argc, char **argv)
{
    int32_t command_len = 0;
    for(int8_t i=0 ; i<argc ; i++)
    {
        command_len += strlen(argv[i])+1;
    }

    char command_string[256];// = new char[command_len];
    char* buf = command_string;
    strcpy(buf, argv[0]);
    strcat(buf, " ");
    for(int8_t i=1 ; i<argc ; i++)
    {
        buf += strlen(argv[i-1])+1;
        strcat(buf, argv[i]);
        strcat(buf, " ");
    }

    LOGH("%s",command_string);
}

void usage(char *module, char *cmd)
{
    char* header = "Service Layer Debug Deamon V0.1 \n\n";

    if (module == NULL) {
        fprintf(stderr,  "start from below command \n  sldd man \n");
    }
}

void usage_man()
{
    printMessage(" Supported module \n");
    for(int i=0 ; action2_table[i].module != NULL ; i++)
    {
        printMessage(" sldd %d \n", action2_table[i].module);
    }
    printMessage(" If you want to find command in module \n");
    printMessage(" You can do like belew \n");
    printMessage("   sldd man list <module> \n");
    printMessage("   sldd man find <module> <command> \n");
}

bool command(int argc, char **argv)
{
    bool enable = false;
    bool disable = false;

    while(argc > 0)
    {
        LOGI("command : argv:%s", argv[0]);
        if(!strcmp(argv[0],"-h") || !strcmp(argv[0],"--h"))
        {
            usage(argv[1], argv[2]);
            return true;
        } else if(!strcmp(argv[0],"-e"))
        {
            enable = true;
        } else if (!strcmp(argv[0], "-d"))
        {
            /* this is a special flag used only when the ADB client launches the ADB Server */
            disable = true;
#ifdef ENABLE_HMI_MGR
        } else if((enable == true) && (!strcmp(argv[0],"accel"))) {
            sClient->act(commandActionHMI, TYPE_ACCELOMETER, ACTION_SET_ENABLE);
            return true;
        } else if((disable == true) && (!strcmp(argv[0],"accel"))) {
            sClient->act(commandActionHMI, TYPE_ACCELOMETER, ACTION_SET_DISABLE);
            return true;
#endif
        } else {
            for(int i=0 ; action2_table[i].module != NULL ; i++)
            {
                if (argv[0]!= NULL && !strcmp(argv[0], action2_table[i].module))
                {
                    //make_history(argc, argv);
                    sClient->act2(action2_table[i].action, argc-1, argv+1);
                    make_history(argc, argv);
                    return true;
                }
            }

            usage(argv[1], argv[2]);
            return true;
        }

        argc--;
        argv++;
    }

    return false;
}

int main(int argc, char **argv)
{
#if defined(ENABLE_STACK_TRACE)
    registerSigHandler();
#endif

    android::ProcessState::self()->startThreadPool();

    sClient = new CommanderClient();
    if( sClient->init() == false )
    {
        printMessage(" can not use sldd now because TCU3 is not ready \n");
        return 0;
    }

    if(command(argc - 1, argv + 1) == true)
    {
        ::usleep(100000); // 0.1s
        sClient.clear();

        //exit
        return 0;
    }

    ::usleep(100000U); // 0.1s
    sClient.clear();

    return 1;
}

