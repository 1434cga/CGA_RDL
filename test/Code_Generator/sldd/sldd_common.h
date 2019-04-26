/**
* \file    sldd_common.h
* \brief     Declare of sldd_common
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.09.31
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_COMMON_DEFINE_H_
#define _SLDD_COMMON_DEFINE_H_

#include <strings.h>

#include <Log.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/atoi.h>

using namespace android;

#define SLDD_OUTPUT_STDOUT

#undef SLDD_OUTPUT_ANDROID_LOG

#if defined(SLDD_OUTPUT_STDOUT)
#define printMessage(...) (fprintf(stderr, __VA_ARGS__))
#elif defined(SLDD_OUTPUT_ANDROID_LOG)
#define printMessage(...) (LOGI(__VA_ARGS__))
#else
#error There is no target output
#endif ///Log output

#define NOT_IMPLEMENTED        ("Under construction\n")

typedef bool (*cmd_handler)(int32_t argc, char **argv);

typedef struct SLDD_CMD_TABLE {
    char* name;
    int32_t id;
    cmd_handler handler;
    char* simple_usage;
} sldd_cmd_table_t;

/**
*    DO_HANDLER
*
*    perform Handler operation in table.
*    If handler returns true, it will success without any message.
*    If handler returns false, it will display usage.
*
*    This will return false if cmd can not find suitable handler in the table.
*/
#define DO_HANDLER(table, cmd, argc, argv) \
    do { \
        for (uint32_t i=0; i<(sizeof(table) / sizeof(sldd_cmd_table_t))-1; i++) { \
            if (!strcasecmp(table[i].name, cmd)) { \
                bool ret = table[i].handler(argc, argv); \
                if(ret == false) \
                    printMessage(table[i].simple_usage); \
                return true; \
            } \
        } \
        return false; \
    } while(0);

/**
*    DO_HANDLER_WITHOUT_RETURN
*
*    perform Handler operation in table.
*    If handler returns true, it will success without any message.
*    If handler returns false, it will display usage.
*
*    This will not make any return value even though cmd can not find suitable handler in the table.
*/
#define DO_HANDLER_WITHOUT_RETURN(table, cmd, argc, argv) \
    do { \
        for (uint32_t i=0; i<(sizeof(table) / sizeof(sldd_cmd_table_t))-1; i++) { \
            if (!strcasecmp(table[i].name, cmd)) { \
                bool ret = table[i].handler(argc, argv); \
                if(ret == false) \
                    printMessage(table[i].simple_usage); \
                return true; \
            } \
        } \
    } while(0);

/**
*    DO_HANDLER_UNTIL_FIND
*
*    find handler until handler returns true. true means it can be hander of cmd.
*
*    This will return false if cmd can not find suitable handler in the table.
*/
#define DO_HANDLER_UNTIL_FIND(table, argc, argv) \
    do { \
        for (uint32_t i=0; i<(sizeof(table) / sizeof(sldd_cmd_table_t))-1; i++) { \
            if(table[i].handler(argc, argv) == true) { \
                return true; \
            } else { \
                printMessage(table[i].simple_usage); \
            } \
        } \
        return false; \
    } while(0);

#define SL_INTERFACE_DECLARE(service_name) \
    static sp<I##service_name> s##__FILE__##service_name = NULL; \
    static sp<I##service_name> __FILE__##service_name##_Interface() \
    { \
        if( s##__FILE__##service_name == NULL) \
        { \
            s##__FILE__##service_name = interface_cast<I##service_name> \
                (defaultServiceManager()->getService(String16(service_name::getServiceName()))); \
        } \
        return s##__FILE__##service_name; \
    } \

#define SL_INTERFACE_DECLARE2(service_name, name) \
    static sp<I##service_name> s##__FILE__##service_name = NULL; \
    static sp<I##service_name> __FILE__##service_name##_Interface() \
    { \
        if( s##__FILE__##service_name == NULL) \
        { \
            s##__FILE__##service_name = interface_cast<I##service_name> \
                (defaultServiceManager()->getService(String16(name))); \
        } \
        return s##__FILE__##service_name; \
    } \

#define SL_INTERFACE_GET(service_name) \
    __FILE__##service_name##_Interface()

#define PRINT_USAGE(tbl) \
    do { \
        for (uint32_t i=0; i<(sizeof(tbl) / sizeof(sldd_cmd_table_t))-1; i++) { \
            printMessage("  [command] %s \n", tbl[i].name ); \
            printMessage("    [usage]\n %s \n", tbl[i].simple_usage ); \
        } \
    } while(0);


#endif ///_SLDD_COMMON_DEFINE_H_
