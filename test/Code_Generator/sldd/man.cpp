/**
* \file    man.cpp
* \brief     Implementation of man
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.10.20
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/


#include <cstdlib>
#include <Log.h>
#include <utils/KeyedVector.h>

#include <services/ApplicationManagerService/IApplicationManagerService.h>
#include <services/ApplicationManagerService/IApplicationManagerServiceType.h>

#include "man.h"
#include "sldd_common.h"

#define LOG_TAG "man"

using android::KeyedVector;

enum man_commands {
    MAN_CMD_LIST =1,
    MAN_CMD_FIND,
};

static bool handler_man_list(int32_t argc, char **argv);
static bool handler_man_find(int32_t argc, char **argv);
static bool handler_man_command(int32_t argc, char **argv);

struct module_cmd_set_t {
    char* sub_module_name;
    sldd_cmd_table_t* cmd_tbl;
    module_cmd_set_t* next;
};

static KeyedVector<char*, struct module_cmd_set_t* > sModule;
/**
*    Declare binder interface
*
*/

SL_INTERFACE_DECLARE2(ApplicationManagerService, APPLICATION_SRV_NAME);



sldd_cmd_table_t man_cmdTable[] = {
    {"list", MAN_CMD_LIST, handler_man_list,
     "  sldd man list [all|module|<module name>]  - display commands and shot usage \n"
     "                  all<by default> -  display all of command \n"
     "                  module          -  display sub module which has commands \n"
     "                  <module name>   -  display commands supported by <module name> \n"},
    {"find", MAN_CMD_FIND, handler_man_find, NOT_IMPLEMENTED},
    {NULL, NULL, NULL, NULL}
};

bool commandActionMAN(int32_t argc, char **argv)
{

    if (argc == 0) {
        usage_man(NULL);
        return true;
    }

    DO_HANDLER_WITHOUT_RETURN(man_cmdTable, argv[0], argc-1, argv+1);

    return handler_man_command(argc, argv);
}


char* usage_man(char *cmd)
{
    printMessage(" sldd man [<command>]     - provide usage for debug command \n");
    printMessage("    <command> \n");
    PRINT_USAGE(man_cmdTable);
    return NULL;
}

void display_table(struct module_cmd_set_t* module_set, char* module, bool display_module)
{
    LOGV("display_table module :%s ", module);
    printMessage("==========================================\n");
    printMessage("|%10s | sldd %s [command]\n", module, module);

    if(module_set == NULL)
        return;

    if(!display_module)
    {
        printMessage("|\n");
        do {
            if(module_set->sub_module_name != NULL)
            {
                printMessage("| sub module - %s\n", module_set->sub_module_name);
            }

            LOGV(" module:%s tbl size:%d tbl name:%s", module, sizeof(sldd_cmd_table_t), module_set->cmd_tbl[0].name);
            //for (int32_t i=0; i<(sizeof(tbl) / sizeof(sldd_cmd_table_t)); i++) {
            for (int32_t i=0; module_set->cmd_tbl[i].name != NULL; i++) {
                printMessage("    [command]  %s\n", module_set->cmd_tbl[i].name);
                printMessage("      [usage]\n%s \n", module_set->cmd_tbl[i].simple_usage);
            }
            module_set = module_set->next;
        } while(module_set != NULL);
    }
}

bool handler_man_list(int32_t argc, char **argv)
{
    LOGV("handler_man_list");
    bool display_all = false;
    bool display_module = false;
    if(argc == 0 ||
       (!strcmp("all", argv[0])))
    {
        display_all = true;
    }

    if(argc == 1 && !strcmp("module", argv[0]))
    {
        display_module = true;
    }

    int8_t module_cnt = sModule.size();
    if(module_cnt == 0)
    {
        printMessage("    There is no module to display.\n");
        return false;
    }

    if(display_all == false && display_module == false)
    {
        LOGV("display module");
        ///Find and display commands for suitable module for argv[0]
        for(int8_t i=0 ; i < module_cnt ; i++)
        {
            char* module_name = sModule.keyAt(i);
            LOGV("display module :%s [%s]", module_name, argv[0]);
            if(module_name != NULL && !strcmp(module_name, argv[0]))
            {
                struct module_cmd_set_t* module_set = sModule[i];
                display_table(module_set, argv[0], true);
                return true;
            }
        }

        printMessage("   Invalid module name - %s.\n", argv[0]);
        return true;
    }

    if(display_all == true || display_module == true)
    {
        LOGV("display all of modules");
        for(uint32_t i=0 ; i < module_cnt ; i++)
        {
            char* module_name = sModule.keyAt(i);
            struct module_cmd_set_t* module_set = sModule[i];

            if(module_name == NULL || module_set == NULL)
            {
                printMessage("   Unexpected table output - [%s]%d.\n", module_name, i);
                continue;
            }

            display_table(module_set, module_name, display_module);
        }
        return true;
    }

    usage_man(NULL);
    return true;
}

bool handler_man_find(int32_t argc, char **argv)
{
    printMessage(" preparing to provide \n");
    return false;
}

static void display_file(const char *file_name)
{
    size_t nr;
    size_t nw;
    char buf[2048];
    size_t buf_size = 2048;
    FILE *f = fopen(file_name, "r");      // open the specified file
    if (f != NULL)
    {
        while ((nr = fread(buf, buf_size, 1U, f)) > 0)
        if ((nw = fwrite(buf, 1U, nr, stdout)) < 0)
        {
            break;
        }

        fclose(f);
    }
}

bool handler_man_command(int32_t argc, char **argv)
{
    if(argv[0] == NULL)
    {
        printMessage(" sldd man <command> \n");
        printMessage("     - insert command to display usage \n");
        return true;
    }

    char man_path[512] = {0, };
    snprintf(man_path, 512UL, "/usr/data/sldd/man/%s.man", argv[0]);
    if( access( man_path, F_OK ) == 0 )
    {
        //snprintf(man_path, 512, "/usr/sldd/man/%s", strlwr(argv[0]));
        //if( access( man_path, F_OK ) != 0 )
        //{
            display_file(man_path);
            return true;
        //}
    }
    printMessage(" no usage for %s \n", argv[0]);

    return false;
}

void registerCommands(char* module, char* sub_module, sldd_cmd_table_t* tbl)
{
    module_cmd_set_t* new_set;
    ssize_t idx = sModule.indexOfKey(module);
    if (idx >= 0)
    {
        if(sub_module != NULL)
        {
            ///find submodule;
            struct module_cmd_set_t* set = sModule.editValueAt(idx);
            if(strcmp(sub_module, set->sub_module_name))
            {
                new_set = new module_cmd_set_t;
                new_set->sub_module_name = sub_module;
                new_set->cmd_tbl = tbl;
                while(set->next != NULL)
                {
                    set = set->next;
                }
                set->next = new_set;
                return;
            }
        }
        LOGE("registerCommands : already exist %s", module);
        return;
    }
    new_set = new module_cmd_set_t;
    new_set->sub_module_name = sub_module;
    new_set->cmd_tbl = tbl;
    new_set->next = NULL;
    sModule.add(module, new_set);
}

