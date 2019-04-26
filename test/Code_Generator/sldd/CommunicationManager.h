/**
* \file    ComManager.h
* \brief     Declare of ComManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2016.01.25
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_COMMUNICATION_MANAGER_H_
#define _SLDD_COMMUNICATION_MANAGER_H_

#define MODULE_COMMUNICATION_MANAGER "comm"

bool commandActionCommunication(int32_t argc, char **argv);
char* usage_communication(char *cmd);
void register_communication();
 
#endif //_SLDD_COMMUNICATION_MANAGER_H_