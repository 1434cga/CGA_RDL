/**
* \file    DiagManager.h
* \brief     Declare of DiagManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       sungwoo.oh
* \date       2015.10.23 UK time
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_DIAG_MANAGER_H_
#define _SLDD_DIAG_MANAGER_H_

#define MODULE_DIAG_MANAGER "diag"

bool commandActionDiag(int32_t argc, char **argv);
char* usage_diag(char *cmd);

void register_diag();

#endif ///_SLDD_DIAG_MANAGER_H_