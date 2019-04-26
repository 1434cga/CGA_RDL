/**
* \file    AlarmManager.h
* \brief     Declare of AlarmManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2016.04.04
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_ALARM_MANAGER_H_
#define _SLDD_ALARM_MANAGER_H_

#define MODULE_ALARM_MANAGER "alarm"

bool commandActionAlarm(int32_t argc, char **argv);
char* usage_alarm(char *cmd);

void register_alarm();

#endif //_SLDD_ALARM_MANAGER_H_