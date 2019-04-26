/**
* \file    AudioManager.cpp
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


#include <Log.h>
#include <utils/atoi.h>
#include <AudioManagerService/IAudioManagerService.h>
#include <AudioManagerService/IAudioManagerServiceType.h>

#include "man.h"
#include "sldd_common.h"
#include "AudioManager.h"

#define LOG_TAG "sldd_audio"

using namespace android;

enum Audio_commands {
    AUDIO_CMD_DTMF_PLAY=1,
    AUDIO_CMD_SET_VOLUME,
    AUDIO_CMD_SET_LOOPBACK_VOLUME,
    AUDIO_CMD_SET_AMP_GAIN,
    AUDIO_CMD_SET_AUDIO_PATH,
    AUDIO_CMD_SET_MUTE_PIN,
    AUDIO_CMD_DTC_AUDIO,
    AUDIO_CMD_LOOPBACK_ENABLE,
    AUDIO_CMD_LOOPBACK_DISABLE,
    AUDIO_CMD_DSP_PARM_READ,
    AUDIO_CMD_DSP_PARM_WRITE,
    AUDIO_CMD_DSP_PARM_STORE,
    AUDIO_CMD_DSP_IMAGE_LOADING,
    AUDIO_CMD_SET_DSP_NUMBER,
    AUDIO_CMD_GET_DSP_NUMBER,
    AUDIO_CMD_FILE_PLAY,
    AUDIO_CMD_FILE_STOP,
    AUDIO_CMD_RINGTONE_PLAY,
    AUDIO_CMD_RINGTONE_STOP,
    AUDIO_CMD_PRM_AUTO_SEARCHING,
    AUDIO_CMD_GET_PRM_VERSION,
    AUDIO_CMD_REQ_BUB_VI,
    AUDIO_CMD_GET_CALL_CONNECT_STATE,
    AUDIO_CMD_GET_FILE_PLAY_STATE,
    AUDIO_CMD_GET_STATUS_PIN_STATE
};

static bool handler_dtmfPlay(int32_t argc, char **argv);
static bool handler_SetVolume(int32_t argc, char **argv);
static bool handler_SetLoopbackVolume(int32_t argc, char **argv);
static bool handler_SetAmpGain(int32_t argc, char **argv);
static bool handler_SetAudioPath(int32_t argc, char **argv);
static bool handler_SetMutePin(int32_t argc, char **argv);
static bool handler_DTCAudio(int32_t argc, char **argv);
static bool handler_LoopbackEnable(int32_t argc, char **argv);
static bool handler_LoopbackDisable(int32_t argc, char **argv);
static bool handler_DspParmRead(int32_t argc, char **argv);
static bool handler_DspParmWrite(int32_t argc, char **argv);
static bool handler_DspParmStore(int32_t argc, char **argv);
static bool handler_DspImageLoading(int32_t argc, char **argv);
static bool handler_AudioSetDspNum(int32_t argc, char **argv);
static bool handler_AudioGetDspNum(int32_t argc, char **argv);
static bool handler_AudioFilePlay(int32_t argc, char **argv);
static bool handler_AudioFileStop(int32_t argc, char **argv);
static bool handler_AudioRingTonePlay(int32_t argc, char **argv);
static bool handler_AudioRingToneStop(int32_t argc, char **argv);
static bool handler_PrmAutoSearching(int32_t argc, char **argv);
static bool handler_GetPrmVersion(int32_t argc, char **argv);
static bool handler_ReqBubRi(int32_t argc, char **argv);
static bool handler_AudioGetCallConnectState(int32_t argc, char **argv);
static bool handler_AudioGetFilePlayState(int32_t argc, char **argv);
static bool handler_AudioGetStatusPinState(int32_t argc, char **argv);


sldd_cmd_table_t audio_cmds[] = {
    {"dtmfPlay", AUDIO_CMD_DTMF_PLAY, handler_dtmfPlay},
    {"SetVolume", AUDIO_CMD_SET_VOLUME, handler_SetVolume},
    {"loopbackVolume", AUDIO_CMD_SET_LOOPBACK_VOLUME, handler_SetLoopbackVolume},
    {"SetAmpGain", AUDIO_CMD_SET_AMP_GAIN, handler_SetAmpGain},
    {"SetAudioPath", AUDIO_CMD_SET_AUDIO_PATH, handler_SetAudioPath},
    {"SetMutePin", AUDIO_CMD_SET_MUTE_PIN, handler_SetMutePin},
    {"DTCAudio", AUDIO_CMD_DTC_AUDIO, handler_DTCAudio},
    {"LoopbackEnable", AUDIO_CMD_LOOPBACK_ENABLE, handler_LoopbackEnable},
    {"LoopbackDisable", AUDIO_CMD_LOOPBACK_DISABLE, handler_LoopbackDisable},
    {"DspParmRead", AUDIO_CMD_DSP_PARM_READ, handler_DspParmRead},
    {"DspParmWrite", AUDIO_CMD_DSP_PARM_WRITE, handler_DspParmWrite},
    {"DspParmStore", AUDIO_CMD_DSP_PARM_STORE, handler_DspParmStore},
    {"DspImageLoading", AUDIO_CMD_DSP_PARM_STORE, handler_DspImageLoading},
    {"SetDspNum", AUDIO_CMD_SET_DSP_NUMBER, handler_AudioSetDspNum},
    {"GetDspNum", AUDIO_CMD_GET_DSP_NUMBER, handler_AudioGetDspNum},
    {"FilePlay", AUDIO_CMD_FILE_PLAY, handler_AudioFilePlay},
    {"FileStop", AUDIO_CMD_FILE_STOP, handler_AudioFileStop},
    {"RingTonePlay", AUDIO_CMD_RINGTONE_PLAY, handler_AudioRingTonePlay},
    {"RingToneStop", AUDIO_CMD_RINGTONE_STOP, handler_AudioRingToneStop},
    {"PrmAutoSearching", AUDIO_CMD_PRM_AUTO_SEARCHING, handler_PrmAutoSearching},
    {"GetPrmVersion", AUDIO_CMD_GET_PRM_VERSION, handler_GetPrmVersion},
    {"ReqBubRi", AUDIO_CMD_REQ_BUB_VI, handler_ReqBubRi},
    {"GetCallConnectState", AUDIO_CMD_GET_CALL_CONNECT_STATE, handler_AudioGetCallConnectState},
    {"GetFilePlayState", AUDIO_CMD_GET_FILE_PLAY_STATE, handler_AudioGetFilePlayState},
    {"GetStatusPinState", AUDIO_CMD_GET_STATUS_PIN_STATE, handler_AudioGetStatusPinState},
    {NULL, NULL, NULL},
};

static sp<IAudioManagerService> mAudioManager = NULL;

sp<IAudioManagerService> getAudioInterface()
{
    if( mAudioManager == NULL)
    {
        mAudioManager = interface_cast<IAudioManagerService>
                (defaultServiceManager()->getService(String16(AUDIO_SRV_NAME)));
    }
    return mAudioManager;
}

bool commandActionAudio(int32_t argc, char **argv) {

    if (argc == 0) {
        usage_Audio(NULL);
        return true;
    }

    DO_HANDLER(audio_cmds, argv[0], argc-1, argv+1);
}

char* usage_Audio(char *cmd) {
    printMessage(" sldd Audio [<command>]         - perform AudioManager's functions \n");
    printMessage("    <command> \n");
    PRINT_USAGE(audio_cmds);
    return NULL;
}

static bool handler_dtmfPlay(int32_t argc, char **argv)
{
    LOGV("handler_common_DTMF_Play : Start - argc:%d , argv[0]%s ",argc,argv[0]);
    getAudioInterface()->AudioDtmfPlay(argv[0]);

    printMessage("==============================\n");
    printMessage("handler_common_DTMF_Play Dial Number[%s] \n", argv[0]);
    printMessage("==============================\n");

    return true;
}

static bool handler_SetVolume(int32_t argc, char **argv)
{
    error_t ret;
    int32_t value = sl::atoi(argv[0]);
    LOGV("handler_SetVolume is %d", value);

    if(0<value && value<=50)
    {
        getAudioInterface()->AudioSetVolume((int8_t)value);
        ret = E_OK;
    }
    else
    {
        ret = E_INVALID_PARAM;
    }

    if(ret==E_OK)
    {
        printMessage("==============================\n");
        printMessage("Sucess - handler_SetVolume is %d \n", value);
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("Fail - Invalid value \n");
        printMessage("Value : 1~50 \n");
        printMessage("==============================\n");
    }
    return true;
}

static bool handler_SetLoopbackVolume(int32_t argc, char **argv)
{
    error_t ret;
    int32_t value = sl::atoi(argv[0]);
    LOGV("LoopbackVolume is %d", value);

    if(0<value && value<=50)
    {
        getAudioInterface()->AudioSetLoopbackVolume(value);
        ret = E_OK;
    }
    else
    {
        ret = E_INVALID_PARAM;
    }

    if(ret==E_OK)
    {
        printMessage("==============================\n");
        printMessage("Sucess - LoopbackVolume is %d \n", value);
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("Fail - Invalid value \n");
        printMessage("Value : 1~50 \n");
        printMessage("==============================\n");
    }
    return true;
}

static bool handler_SetAmpGain(int32_t argc, char **argv)
{
    error_t ret;
    int32_t value = sl::atoi(argv[0]);
    LOGV("handler_SetAmpGain : value:%d",value);

    if(value>=0 && value<=3 )
    {
        getAudioInterface()->AudioSetAmpGain(value);
        ret = E_OK;
    }
    else
    {
        ret = E_INVALID_PARAM;
    }

    if(ret==E_OK)
    {
        printMessage("==============================\n");
        printMessage("Sucess - handler_SetAmpGain is %d \n", value);
        printMessage("==============================\n");
    }
    else
    {
        printMessage("==============================\n");
        printMessage("Fail - Invalid value \n");
        printMessage("0 : 20db \n");
        printMessage("1 : 26db \n");
        printMessage("2 : 32db \n");
        printMessage("3 : 36db \n");
        printMessage("==============================\n");
    }
    return true;
}

static bool handler_SetAudioPath(int32_t argc, char **argv)
{
    error_t ret;
    int32_t value = sl::atoi(argv[0]);
    LOGV("handler_SetAudioPath : value:%d",value);

    getAudioInterface()->AudioSetAudioPathState(value);

    if(value>=1)
        printMessage("handler_SetAudioPath - HIGH!! \n");
    else if(value==0)
        printMessage("handler_SetAudioPath - LOW!! \n");
    else
        printMessage("Fail - Invalid value \n");
    return true;
}

static bool handler_SetMutePin(int32_t argc, char **argv)
{
    error_t ret;
    int32_t value = sl::atoi(argv[0]);
    LOGV("handler_SetMutePin : value:%d",value);

    getAudioInterface()->AudioSetAudioMutePinState(value);

    if(value>=1)
        printMessage("handler_SetMutePin - enable!! \n");
    else if(value==0)
        printMessage("handler_SetMutePin - disable!! \n");
    else
        printMessage("Fail - Invalid value \n");
    return true;
}

static bool handler_DTCAudio(int32_t argc, char **argv)
{
    printMessage("handler_DTCAudio \n");
    return true;
}

static bool handler_LoopbackEnable(int32_t argc, char **argv)
{
    int32_t value = sl::atoi(argv[0]);

    getAudioInterface()->AudioLoopback(value);
    printMessage("Loopback ENABLED : %d\n", value);
    
    return true;
}

static bool handler_LoopbackDisable(int32_t argc, char **argv)
{
    int32_t value = sl::atoi(argv[0]);

    getAudioInterface()->AudioLoopback(value);
    printMessage("Loopback DISABLED : %d\n", value);

    return true;
}

static bool handler_DspParmRead(int32_t argc, char **argv)
{
    uint32_t ret_value;
    int32_t dsp;
    int32_t iaddr;

    dsp = DSP1;
    iaddr=(int32_t)strtol(argv[0], NULL, 16);

    ret_value = getAudioInterface()->AudioDspParmRead(dsp , iaddr);

    printMessage("SUCESS!!! \n");
    printMessage("address : [0x%x] \n", iaddr);
    printMessage("value   : [0x%x] \n", ret_value);

    return true;
}

static bool handler_DspParmWrite(int32_t argc, char **argv)
{
    int32_t ret_value;
    int32_t dsp;
    int32_t iaddr;
    uint32_t length = strlen(argv[1]);

    dsp = DSP1;
    iaddr=(int32_t)strtol(argv[0], NULL, 16);

    LOGV("handler_DspParmWrite : %s, length: %d",argv[0], length);//temp

    if(length==6UL){
        ret_value = getAudioInterface()->AudioDspParmWrite(dsp, iaddr, (const char *)argv[1]);
        printMessage("SUCESS!!! \n");
    }
    else{
        printMessage("FAIL!!! \n");
        printMessage("check the value length(ffffff) \n");
    }

    return true;
}

static bool handler_DspParmStore(int32_t argc, char **argv)
{
    int32_t ret_value;

    LOGV("handler_DspParmStore : [%s]",argv[0]);

    printMessage("Please waiting...... \n");

    ret_value = getAudioInterface()->AudioDspParmStore((const char *)argv[0], 1, 1);
    if(ret_value==E_OK) {
        printMessage("SUCESS!!! \n");
    }
    else{
        printMessage("FAIL!!! \n");
    }
    return true;
}

static bool handler_DspImageLoading(int32_t argc, char **argv)
{
    int32_t ret_value;

    LOGV("handler_DspImageLoading : [%s]",argv[0]);
    if(argv[0]==NULL){
        ret_value = getAudioInterface()->AudioDspDefaultImageLoading();
    }
    else{
        ret_value = getAudioInterface()->AudioDspImageLoading((const char *)argv[0]);
    }

    if(ret_value==E_OK) {
        printMessage("SUCESS!!! \n");
    }
    else{
        printMessage("FAIL!!! \n");
    }

    return true;

}

static bool handler_AudioSetDspNum(int32_t argc, char **argv)
{
    uint32_t value = sl::atoi(argv[0]);

    getAudioInterface()->AudioSetDspNum(value);

    if(value<= 255)
        printMessage("handler_DspSelectNumber : %d\n",value);
    else
        printMessage("fail!! max value : 255 \n");
    return true;
}

static bool handler_AudioGetDspNum(int32_t argc, char **argv)
{
    uint8_t value;
    LOGV("handler_AudioGetDspNum()");
    value = getAudioInterface()->AudioGetDspNum();
    LOGV("value:%d",value);
    printMessage("Dsp Image Number : %d \n", value);

    return true;
}

static bool handler_AudioFilePlay(int32_t argc, char **argv)
{
    int32_t ret_value;

    LOGV("handler_AudioFilePlay : [%s]", argv[0]);
    if(argv[0]==NULL){
        ret_value = E_ERROR;
    }
    else{
        ret_value = getAudioInterface()->AudioFilePlay((const char *)argv[0]);
    }

    if(ret_value == E_OK) {
        printMessage("SUCESS!!! \n");
    }
    else{
        printMessage("FAIL!!! \n");
    }

    return true;
}

static bool handler_AudioFileStop(int32_t argc, char **argv)
{
    getAudioInterface()->AudioFileStop();
    printMessage("AudioFile Stop!! \n");
    return true;
}

static bool handler_AudioRingTonePlay(int32_t argc, char **argv)
{
    getAudioInterface()->AudioRingTonePlay();
    printMessage("RingTone Play!! \n");
    return true;
}

static bool handler_AudioRingToneStop(int32_t argc, char **argv)
{
    getAudioInterface()->AudioRingToneStop();
    printMessage("RingTone Stop!! \n");
    return true;
}

static bool handler_PrmAutoSearching(int32_t argc, char **argv)
{
    getAudioInterface()->AudioPrmAutoSearching();

    printMessage("PRM auto search sucess!! \n");
    return true;
}

static bool handler_GetPrmVersion(int32_t argc, char **argv)
{
    const char* temp;

    temp = getAudioInterface()->AudioGetPrmVersionName();
    printMessage("PRM Version [%s] \n",temp);

    return true;
}

static bool handler_ReqBubRi(int32_t argc, char **argv)
{
    getAudioInterface()->AudioReqBubRi();

    return true;
}

static bool handler_AudioGetCallConnectState(int32_t argc, char **argv)
{
    getAudioInterface()->AudioGetCallConnectState();

    return true;
}

static bool handler_AudioGetFilePlayState(int32_t argc, char **argv)
{
    getAudioInterface()->AudioGetFilePlayState();

    return true;
}

static bool handler_AudioGetStatusPinState(int32_t argc, char **argv)
{
    int32_t ret_value = 0;

    ret_value = getAudioInterface()->AudioGetStatusPinState();
    printMessage("handler_AudioGetStatusPinState : %d\n", ret_value);

    return true;
}

void register_Audio()
{
    registerCommands(MODULE_AUDIO_MANAGER, NULL, audio_cmds);
}

