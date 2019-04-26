/**
 * \file    systemprop.h
 * \brief   Declare of http
 *
 * \details
 *    This software is copyright protected and proprietary to
 *    LG electronics. LGE grants to you only those rights as
 *    set out in the license conditions. All other rights remain
 *    with LG electronics.
 * \author       dy616.lee
 * \date         2018.02.05
 * \attention Copyright (c) 2017 by LG electronics co, Ltd. All rights reserved.
 */

#ifndef SLDD_SYSPROP_H
#define SLDD_SYSPROP_H

#define MODULE_SYSTEM_PROP "sysprop"

extern bool commandActionPROP(int32_t argc, char **argv);
extern char* usage_sysprop(char* cmd);
extern void register_sysprop(void);

#endif

