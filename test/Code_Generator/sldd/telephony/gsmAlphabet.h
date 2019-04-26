/* This source code is converted from Android Open Source Project to port into telematics,
 * and this is header from original file,
 * android/frameworks/opt/telephony/src/java/com/android/inter/telephony/GsmAlphabet.java
 */
/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GSM_ALPHABET_H
#define GSM_ALPHABET_H

#include <utils/RefBase.h>
#include <utils/String16.h>
#include <utils/external/mindroid/lang/StringWrapper.h>
#include <TelephonyManagerService/utils/ByteArray.h>

using android::RefBase;
using android::sp;
using mindroid::StringWrapper;
using android::String16;

class GsmAlphabet : public RefBase
{
public:
    static const uint8_t GSM_EXTENDED_ESCAPE = 0x1B;
    static const uint32_t UDH_SEPTET_COST_LENGTH = 1;
    static const uint32_t UDH_SEPTET_COST_ONE_SHIFT_TABLE = 4;
    static const uint32_t UDH_SEPTET_COST_TWO_SHIFT_TABLES = 7;
    static const uint32_t UDH_SEPTET_COST_CONCATENATED_MESSAGE = 6;

    class TextEncodingDetails {
    public:
        int32_t msgCount;
        int32_t codeUnitCount;
        int32_t codeUnitsRemaining;
        int32_t codeUnitSize;
        int32_t languageTable;
        int32_t languageShiftTable;

        sp<String> toString() {
            return NULL;
        }
    };

#if 0
    static int32_t charToGsm(char c);
    static int32_t charToGsm(char c);
    static int32_t charToGsmExtended(char c);
    static char gsmToChar(int32_t gsmChar);
    static char gsmExtendedToChar(int32_t gsmChar);
    static uint8_t* stringToGsm7BitPackedWithHeader(sp<String> data, uint8_t* header);
    static uint8_t* stringToGsm7BitPackedWithHeader(sp<String> data, uint8_t* header,
            int32_t languageTable, int32_t languageShiftTable);
    static uint8_t* stringToGsm7BitPacked(sp<String> data);
    static uint8_t* stringToGsm7BitPacked(sp<String> data, int32_t languageTable,
            int32_t languageShiftTable);
    static uint8_t* stringToGsm7BitPacked(sp<String> data, int32_t startingSeptetOffset,
            int32_t languageTable, int32_t languageShiftTable);
#endif
    static String16 gsm7BitPackedToString(uint8_t* pdu, int32_t offset, int32_t lengthSeptets);
    static String16 gsm7BitPackedToString(uint8_t* pdu, int32_t offset, int32_t lengthSeptets, int32_t numPaddingBits, int32_t languageTable, int32_t shiftTable);
    static sp<String> gsm8BitUnpackedToString(uint8_t* data, int32_t offset, int32_t length);
    static sp<String> gsm8BitUnpackedToString(uint8_t* data, int32_t offset, int32_t length, const char* characterset);
    static sp<ByteArray> stringToGsm8BitPacked(const sp<String>& s);
    static void stringToGsm8BitUnpackedField(const sp<String>& s, const uint8_t* dest, int32_t offset, int32_t length);
    static int32_t countGsmSeptets(char c);
    static int32_t countGsmSeptets(char c, bool throwsException);
    static int32_t countGsmSeptetsUsingTables(char s, bool use7bitOnly, int32_t languageTable, int32_t languageShiftTable);
#if 0
    static TextEncodingDetails countGsmSeptets(CharSequence s, boolean use7bitOnly);
    static int32_t findGsmSeptetLimitIndex(String s, int32_t start, int32_t limit, int32_t langTable, int32_t langShiftTable);

    static synchronized void setEnabledSingleShiftTables(int32_t* tables);
    static synchronized void setEnabledLockingShiftTables(int32_t* tables);
    static synchronized int32_t* getEnabledSingleShiftTables();
    static synchronized int32_t* getEnabledLockingShiftTables();
#endif

private:
    static const StringWrapper sCharsToGsmTables[];
    static const StringWrapper sCharsToShiftTables[];
#if 0
    static int32_t* sEnabledSingleShiftTables;
    static int32_t* sEnabledLockingShiftTables;
    static int32_t sHighestEnabledSingleShiftCode;
    static boolean sDisableCountryEncodingCheck = false;
#endif
    static const StringWrapper sLanguageTables[];
    static const StringWrapper sLanguageShiftTables[];
    static const int32_t sTableLength = 14;

#if 0
    class LanguagePairCount {
    public:
        int32_t languageCode;
        int32_t* septetCounts;
        int32_t* unencodableCounts;
        LanguagePairCount(int32_t code);
    };
#endif

private:
    GsmAlphabet() {}

    static struct init
    {
        init();

    } initializer;

#if 0
    static void packSmsChar(uint8_t* packedChars, int32_t bitOffset, int32_t value);
    static void enableCountrySpecificEncodings();
#endif

};

#endif
