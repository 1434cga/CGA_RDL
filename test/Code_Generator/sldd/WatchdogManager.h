/**
* \file    WatchdogManager.h
* \brief     Declare of WatchdogManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2016.03.04
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_WATCHDOG_MANAGER_H_
#define _SLDD_WATCHDOG_MANAGER_H_

#define MODULE_WATCHDOG_MANAGER "watchdog"

bool commandActionWatchdog(int32_t argc, char **argv);
char* usage_Watchdog(char *cmd);
void register_Watchdog();

#endif //_SLDD_WATCHDOG_MANAGER_H_