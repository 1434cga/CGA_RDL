/**
* \file    AudioManager.h
* \brief     Implementation of AudioManager
*
* \details
*    This software is copyright protected and proprietary to
*    LG electronics. LGE grants to you only those rights as
*    set out in the license conditions. All other rights remain
*    with LG electronics.
* \author       kitaek.seok
* \date       2015.12.29
* \attention Copyright (c) 2015 by LG electronics co, Ltd. All rights reserved.
*/

#ifndef _SLDD_AUDIO_MANAGER_H_
#define _SLDD_AUDIO_MANAGER_H_

#define MODULE_AUDIO_MANAGER "audio"

#define AUDIO_INTER_LOOPBACK_ENABLE 0
#define AUDIO_NETWORK_LOOPBACK_ENABLE 1
#define AUDIO_INTER_LOOPBACK_DISABLE 2
#define AUDIO_NETWORK_LOOPBACK_DISABLE 3

#define DSP1          1     // main DSP
#define DSP2          2     // sub  DSP

bool commandActionAudio(int32_t argc, char **argv);
char* usage_Audio(char *cmd);

void register_Audio();

#endif ///_SLDD_AUDIO_MANAGER_H_