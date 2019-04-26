
#ifndef SLDD_TELEPHONY_COMMON_H
#define SLDD_TELEPHONY_COMMON_H

#include <utils/external/mindroid/lang/String.h>
#include <utils/external/mindroid/lang/StringWrapper.h>

#include <TelephonyManagerService/base/INetworkQueryService.h>
#include <TelephonyManagerService/base/PhoneConstants.h>
#include <TelephonyManagerService/base/dataconnection/IDataService.h>
#include <TelephonyManagerService/base/RILConstants.h>
#include <TelephonyManagerService/base/ServiceState.h>
#include <TelephonyManagerService/base/SmsManager.h>

#include <TelephonyManagerService/base/TelephonyManager.h>
#include <TelephonyManagerService/utils/ByteArray.h>
#include <TelephonyManagerService/utils/Util.h>

#include "telephony/iccUtils.h"

static const char *TopsStatusAsString(int32_t i, const char *def = "??") {
    switch(i) {
        case TelephonyManager::TOPS_STATUS_DISABLED:                return "Disabled";
        case TelephonyManager::TOPS_STATUS_NOT_ACTIVE:              return "Not active";
        case TelephonyManager::TOPS_STATUS_ACTIVE_NOT_CONNECTED:    return "Active, not connected";
        case TelephonyManager::TOPS_STATUS_ACTIVE_CONNECTED:        return "Active, connected";
        case TelephonyManager::TOPS_STATUS_ACTIVE_FALLBACK:         return "Active, fallback";
        case TelephonyManager::TOPS_STATUS_ACTIVE_MINIMUM_FALLBACK: return "Active, ensuring minimum duration of fallback";
        default:                                                    return def;
    }
}

static const char *TopsNumberAsString(int32_t i, const char *def = "??") {
    switch (i) {
        case TelephonyManager::TOPS_NUMBER_FALLBACKS_TOTAL:             return "Number of fallbacks total";
        case TelephonyManager::TOPS_NUMBER_FALLBACKS_TOTAL_SINCE:       return "Number of fallbacks total since last successful connection";
        case TelephonyManager::TOPS_NUMBER_DISABLED_TOO_MANY_FALLBACKS: return "Number of times connectivity has been disabled due to too many fallbacks";
        case TelephonyManager::TOPS_NUMBER_CONNECTED_TOTAL:             return "Number of times connected total";
        case TelephonyManager::TOPS_NUMBER_CONNECTED_SINCE:             return "Number of times connected since (last) start of connectivity system";
        default:                                                        return def;
    }
}

#endif // SLDD_TELEPHONY_COMMON_H