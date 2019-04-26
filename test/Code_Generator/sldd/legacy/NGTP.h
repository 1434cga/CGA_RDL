/**
* \file    AppManager.h
* \brief     Declare of AppManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.09.14
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_NGTP_MANAGER_H_
#define _SLDD_NGTP_MANAGER_H_

#define MODULE_NGTP_MANAGER "ngtp"

bool commandActionNGTP(int32_t argc, char **argv);
char* usage_ngtp(char *cmd);

void register_ngtp();

#endif ///_SLDD_APP_MANAGER_H_
