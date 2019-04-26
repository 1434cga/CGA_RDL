/* This source code is converted from Android Open Source Project to port into telematics,
 * and this is header from original file,
 * android/frameworks/opt/telephony/src/java/com/android/inter/telephony/uicc/IccUtils.java
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

/**
 * @copyright
 * Copyright (c) 2015 by LG Electronics Inc.
 * This program or software including the accompanying associated documentation
 * ("Software") is the proprietary software of LG Electronics Inc. and or its
 * licensors, and may only be used, duplicated, modified or distributed pursuant
 * to the terms and conditions of a separate written license agreement between you
 * and LG Electronics Inc. ("Authorized License"). Except as set forth in an
 * Authorized License, LG Electronics Inc. grants no license (express or implied),
 * rights to use, or waiver of any kind with respect to the Software, and LG
 * Electronics Inc. expressly reserves all rights in and to the Software and all
 * intellectual property therein. If you have no Authorized License, then you have
 * no rights to use the Software in any ways, and should immediately notify LG
 * Electronics Inc. and discontinue all use of the Software.
 *
 * @file
 * IccUtils.h
 * @brief
 * Declaration of IccUtils
 */


#ifndef ICC_UTILS_H
#define ICC_UTILS_H

#include <utils/RefBase.h>
#include <utils/external/mindroid/lang/String.h>
#include <TelephonyManagerService/utils/ByteArray.h>

using android::RefBase;
using android::sp;
using mindroid::String;

class IccUtils : public RefBase
{
public:
    IccUtils();
    virtual ~IccUtils();

    /**
    * Many fields in GSM SIM's are stored as nibble-swizzled BCD
    *
    * Assumes left-justified field that may be padded right with 0xf
    * values.
    * Stops on invalid BCD value, returning string so far
    */
    static sp<String> bcdToString(const sp<ByteArray>& data, int32_t offset);
    static sp<String> bcdToString(uint8_t* data, int32_t offset, int32_t length);

    /**
     * Decode cdma byte into String.
     */
    static sp<String> cdmaBcdToString(uint8_t* data, int32_t offset, int32_t length);

    /**
     * Decodes a GSM-style BCD byte, returning an int ranging from 0-99.
     *
     * In GSM land, the least significant BCD digit is stored in the most
     * significant nibble.
     *
     * Out-of-range digits are treated as 0 for the sake of the time stamp,
     * because of this:
     *
     * TS 23.040 section 9.2.3.11
     * "if the MS receives a non-integer value in the SCTS, it shall
     * assume the digit is set to 0 but shall store the entire field
     * exactly as received"
     */
    static int32_t gsmBcdByteToInt(uint8_t b);
    static int32_t cdmaBcdByteToInt(uint8_t b);
    static sp<String> adnStringFieldToString(const sp<ByteArray>& data, int32_t offset);
    static sp<String> adnStringFieldToString(uint8_t* data, int32_t offset, int32_t length);

    /**
    * Converts a hex String to a byte array.
    *
    * @param[in] s A string of hexadecimal characters, must be an even number of
    *              chars long
    *
    * @return byte array representation
    */
    static sp<ByteArray> hexStringToBytes(const sp<String>& s);
    static sp<ByteArray> hexStringToBytes(const char* s);

    /**
    * Converts a byte array into a String of hexadecimal characters.
    *
    * @param[in] bytes an array of bytes
    *
    * @return hex string representation of bytes array
    */
    static sp<String> bytesToHexString(const sp<ByteArray>& bytes);
    static sp<String> bytesToHexString(const uint8_t* bytes, uint32_t length);
#if 0
    /**
    * Convert a TS 24.008 Section 10.5.3.5a Network Name field to a string
    * "offset" points to "octet 3", the coding scheme byte
    * empty string returned on decode error
    */
    static sp<String> networkNameToString(uint8_t* data, int32_t offset, int32_t length);
    static sp<Bitmap> parseToBnW(uint8_t* data, int32_t length);
    static sp<Bitmap> parseToRGB(uint8_t* data, int32_t length, bool transparency);
#endif

private:
    static int32_t hexCharToInt(char c);
#if 0
    static int32_t bitToRGB(int32_t bit);
    static int32_t[] mapTo2OrderBitColor(uint8_t* data, int32_t valueIndex, int32_t length, int32_t[] colorArray, int32_t bits);
    static int[] mapToNon2OrderBitColor(uint8_t* data, int32_t valueIndex, int32_t length, int32_t[] colorArray, int32_t bits);
    static int[] getCLUT(uint8_t* rawData, int32_t offset, int32_t number);
#endif
};

#endif
