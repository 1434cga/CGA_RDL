/**
* \file    SystemManager.h
* \brief     Declare of SystemManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       cheolhwan.bae
* \date       2015.12.29
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_SYSTEM_MANAGER_H_
#define _SLDD_SYSTEM_MANAGER_H_

#define MODULE_SYSTEM_MANAGER "sys"

bool commandActionSystem(int32_t argc, char **argv);
char* usage_system(char *cmd);
void register_system();

#endif //_SLDD_SYSTEM_MANAGER_H_