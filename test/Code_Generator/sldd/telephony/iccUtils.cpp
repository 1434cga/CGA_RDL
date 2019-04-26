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
 * IccUtils.cpp
 * @brief
 * Definition of IccUtils
 */

#include <cstring>
#include <Log.h>

#include "telephony/gsmAlphabet.h"
#include "telephony/iccUtils.h"

#define LOG_TAG "sldd_IccUtils"

IccUtils::IccUtils() {
}

IccUtils::~IccUtils() {
}

sp<String> IccUtils::bcdToString(const sp<ByteArray>& data, int32_t offset) {
    StringWrapper ret;

    for (int32_t i = offset; i < offset + data->size(); i++) {
        int32_t v;

        v = data->getByte(i) & 0xf;
        if (v > 9)  break;
        ret += (char)('0' + v);

        v = (data->getByte(i) >> 4) & 0xf;
        // Some PLMNs have 'f' as high nibble, ignore it
        if (v == 0xf) continue;
        if (v > 9)  break;
        ret += (char)('0' + v);
    }

    return new String(ret.c_str());
}

sp<String> IccUtils::bcdToString(uint8_t* data, int32_t offset, int32_t length) {
    StringWrapper ret;

    for (int32_t i = offset ; i < offset + length ; i++) {
        int32_t v;

        v = data[i] & 0xf;
        if (v > 9)  break;
        ret += (char)('0' + v);

        v = (data[i] >> 4) & 0xf;
        // Some PLMNs have 'f' as high nibble, ignore it
        if (v == 0xf) continue;
        if (v > 9)  break;
        ret += (char)('0' + v);
    }

    return new String(ret.c_str());
}

sp<String> IccUtils::cdmaBcdToString(uint8_t* data, int32_t offset, int32_t length) {
    return NULL;
}

int32_t IccUtils::gsmBcdByteToInt(uint8_t b) {
    int32_t ret = 0;

    // treat out-of-range BCD values as 0
    if ((b & 0xf0) <= 0x90) {
        ret = (b >> 4) & 0xf;
    }

    if ((b & 0x0f) <= 0x09) {
        ret +=  (b & 0xf) * 10;
    }

    return ret;
}

int32_t IccUtils::cdmaBcdByteToInt(uint8_t b) {
    int32_t ret = 0;

    // treat out-of-range BCD values as 0
    if ((b & 0xf0) <= 0x90) {
        ret = ((b >> 4) & 0xf) * 10;
    }

    if ((b & 0x0f) <= 0x09) {
        ret +=  (b & 0xf);
    }

    return ret;
}

sp<String> IccUtils::adnStringFieldToString(const sp<ByteArray>& data, int32_t offset) {
    return adnStringFieldToString(data->getBytes(), offset, data->size());
}

sp<String> IccUtils::adnStringFieldToString(uint8_t* data, int32_t offset, int32_t length) {
/*
    if (length == 0) {
        return String::EMPTY_STRING;
    }
    if (length >= 1) {
        if (data[offset] == (uint8_t) 0x80) {
            int32_t ucslen = (length - 1) / 2;
            sp<String> ret = NULL;

            //ret = new String(data, offset + 1, ucslen * 2, "utf-16be");

            if (ret != NULL) {
                // trim off trailing FFFF characters

                ucslen = ret->size();
                while (ucslen > 0 && ret->charAt(ucslen - 1) == '\uFFFF')
                    ucslen--;

                return ret->substr(0, ucslen);
            }
        }
    }

    bool isucs2 = false;
    char base = '\0';
    int32_t len = 0;

    if (length >= 3 && data[offset] == (uint8_t) 0x81) {
        len = data[offset + 1] & 0xFF;
        if (len > length - 3)
            len = length - 3;

        base = (char) ((data[offset + 2] & 0xFF) << 7);
        offset += 3;
        isucs2 = true;
    } else if (length >= 4 && data[offset] == (uint8_t) 0x82) {
        len = data[offset + 1] & 0xFF;
        if (len > length - 4)
            len = length - 4;

        base = (char) (((data[offset + 2] & 0xFF) << 8) |
                        (data[offset + 3] & 0xFF));
        offset += 4;
        isucs2 = true;
    }

    if (isucs2) {
        StringWrapper ret;

        while (len > 0) {
            // UCS2 subset case

            if (data[offset] < 0) {
                ret.append((char) (base + (data[offset] & 0x7F)));
                offset++;
                len--;
            }

            // GSM character set case

            int32_t count = 0;
            while (count < len && data[offset + count] >= 0)
                count++;

            //ret.append(GsmAlphabet::gsm8BitUnpackedToString(data, offset, count));

            offset += count;
            len -= count;
        }

        return new String(ret.c_str());
    }
*/
#if 0
    Resources resource = Resources.getSystem();
    String defaultCharset = "";
    try {
        defaultCharset =
                resource.getString(com.android.internal.R.string.gsm_alphabet_default_charset);
    } catch (NotFoundException e) {
        // Ignore Exception and defaultCharset is set to a empty string.
    }
#endif
    return GsmAlphabet::gsm8BitUnpackedToString(data, offset, length, "");
}

int32_t IccUtils::hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);

    //throw new RuntimeException ("invalid hex char '" + c + "'");
    RLOGE("RuntimeException: invalid hex char '%c'", c);
    return 0;
}

sp<ByteArray> IccUtils::hexStringToBytes(const sp<String>& s) {
    if (s == NULL) return NULL;

    int32_t sz = s->size();
    uint8_t* bytes = new uint8_t[sz/2];

    for (int32_t i=0 ; i <sz ; i+=2) {
        bytes[i/2] = (uint8_t) ((hexCharToInt(s->charAt(i)) << 4)
                            | hexCharToInt(s->charAt(i+1)));
    }

    sp<ByteArray> ba = new ByteArray(bytes, sz/2);
    delete[] bytes;

    return ba;
}

sp<ByteArray> IccUtils::hexStringToBytes(const char* s) {
    if (s == NULL) return NULL;

    int32_t sz = std::strlen(s);
    uint8_t* bytes = new uint8_t[sz/2];

    for (int32_t i=0 ; i <sz ; i+=2) {
        bytes[i/2] = (uint8_t) ((hexCharToInt(s[i]) << 4)
                            | hexCharToInt(s[i+1]));
    }

    sp<ByteArray> ba = new ByteArray(bytes, sz/2);
    delete[] bytes;

    return ba;
}

sp<String> IccUtils::bytesToHexString(const sp<ByteArray>& bytes) {
    if (bytes == NULL) return NULL;

    const StringWrapper hexChars("0123456789abcdef");
    StringWrapper ret;

    for (uint32_t i = 0 ; i < bytes->size() ; i++) {
        int32_t b;
        b = 0x0f & (bytes->getByte(i) >> 4);
        ret += hexChars.charAt(b);
        b = 0x0f & bytes->getByte(i);
        ret += hexChars.charAt(b);
    }

    return new String(ret.c_str());
}

sp<String> IccUtils::bytesToHexString(const uint8_t* bytes, uint32_t length) {
    if (bytes == NULL) return NULL;

    const StringWrapper hexChars("0123456789abcdef");
    StringWrapper ret;

    for (uint32_t i = 0 ; i < length ; i++) {
        int32_t b;
        b = 0x0f & (bytes[i] >> 4);
        ret += hexChars.charAt(b);
        b = 0x0f & bytes[i];
        ret += hexChars.charAt(b);
    }

    return new String(ret.c_str());
}

#if 0
sp<String> IccUtils::networkNameToString(uint8_t* data, int32_t offset, int32_t length) {
    return NULL;
}

sp<Bitmap> IccUtils::parseToBnW(uint8_t* data, int32_t length) {
    return NULL;
}

int32_t IccUtils::bitToRGB(int32_t bit) {
    return 0;
}

sp<Bitmap> IccUtils::parseToRGB(uint8_t* data, int32_t length, bool transparency) {
    return NULL;
}

int32_t[] IccUtils::mapTo2OrderBitColor(uint8_t* data, int32_t valueIndex, int32_t length, int32_t[] colorArray, int32_t bits) {
    return NULL;
}

int32_t[] IccUtils::mapToNon2OrderBitColor(uint8_t* data, int32_t valueIndex, int32_t length, int32_t[] colorArray, int32_t bits) {
    return NULL;
}

int32_t[] IccUtils::getCLUT(uint8_t* rawData, int32_t offset, int32_t number) {
    return NULL;
}
#endif

