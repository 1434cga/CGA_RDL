/**
* \file    man.h
* \brief     Declare of man
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

#ifndef _SLDD_MAN_H_
#define _SLDD_MAN_H_

#include "sldd_common.h"

#define MODULE_MAN "man"

bool commandActionMAN(int32_t argc, char **argv);
char* usage_man(char *cmd);

void registerCommands(char* module, char* sub_module, sldd_cmd_table_t* tbl);
#endif //_SLDD_MAN_H_