#ifndef SLDD_TIME_MANAGER_H_
#define SLDD_TIME_MANAGER_H_


#include <iostream>
#include <time.h>
#include <sys/time.h>

#include <binder/IServiceManager.h>
#include <TimeManagerService/ITimeChangedReceiver.h>
#include <TimeManagerService/ITimeManagerService.h>
#include <TimeManagerService/ITimeManagerServiceType.h>

#include "corebase/ServiceTable.h"

#define MODULE_TIME_MANAGER "tm"

bool commandActionTime(int32_t argc, char **argv);
char* usage_time(char *cmd);
void register_time();

bool handler_setTime_t(int32_t argc, char **argv);
bool handler_getTime_t(int32_t argc, char **argv);
bool handler_TZoff_t(int32_t argc, char **argv);
bool handler_DST_t(int32_t argc, char **argv);
bool handler_testTime_t();
bool handler_testTimePri_t(int32_t argc, char **argv);
bool handler_testTimeProp_t(int32_t argc, char **argv);
bool handler_testTimeUtils_t(int32_t argc, char **argv);
bool handler_testTimeNoti_t(int32_t argc, char **argv);

error_t test_setime(int32_t type, int64_t timeMs,
    int32_t offset = timeservice::UNKNOWN,
    int32_t dst = timeservice::UNKNOWN,
    int32_t forced = timeservice::DISABLE);
error_t test_setime(int32_t type, const char* timeMs,
    int32_t offset = timeservice::UNKNOWN,
    int32_t dst = timeservice::UNKNOWN,
    int32_t forced = timeservice::DISABLE);
void test_gettime();
error_t test_gettime_user();
time_t parse_SystemTime(char *s);
void default_test_pri();

class TimeReceiver : public RefBase, public BnTimeChangedReceiver
{
public:
    TimeReceiver() {}
    virtual ~TimeReceiver() {}

    virtual void onTimeReceived(int32_t time_type, int64_t GMT_msec, int64_t diff_time_msec)
        { std::cout << "### onTimeReceived type = " << time_type << ", GMT_msec = " << GMT_msec << std::endl; }
    virtual void onTimeZoneReceived(int32_t time_type, int32_t offset_min, int32_t dst)
        {std::cout << "### onTimeZoneReceived type = " << time_type << ", offset_min = " << offset_min << std::endl; }
};

class TimeDeathRecipient : public IBinder::DeathRecipient
{
public:
    TimeDeathRecipient() {}
    virtual ~TimeDeathRecipient() {}
    virtual void binderDied(const wp<IBinder>& who)
        { std::cout << "Died  Time-Service..." << std::endl;}
};

#endif //SLDD_TIME_MANAGER_H_

