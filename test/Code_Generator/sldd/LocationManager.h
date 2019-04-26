/**
* \file    PowerManager.h
* \brief     Declare of PowerManager
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

#ifndef _SLDD_LOCATION_MANAGER_H_
#define _SLDD_LOCATION_MANAGER_H_

#define MODULE_LOCATION_MANAGER "loc"

bool commandActionLocation(int32_t argc, char **argv);
char* usage_location(char *cmd);

void register_location();

#endif ///_SLDD_POWER_MANAGER_H_