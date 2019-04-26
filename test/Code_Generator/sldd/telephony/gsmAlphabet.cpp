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


#include <Log.h>
#include <utils/Unicode.h>
#include "telephony/gsmAlphabet.h"

#define LOG_TAG "sldd_GsmAlphabet"

const char16_t langTbl[][128] =
{
    {
    /*   0       1       2       3       4       5       6       7
         8       9       A       B       C       D       E       F    */
       '@', 0x00a3,    '$', 0x00a5, 0x00e8, 0x00e9, 0x00f9, 0x00ec,  /* 0x00 ~ 0x07 */
    0x00f2, 0x00c7,   '\n', 0x00d8, 0x00f8,   '\r', 0x00c5, 0x00e5,  /* 0x08 ~ 0x0F */
    0x0394,    '_', 0x03a6, 0x0393, 0x039b, 0x03a9, 0x03a0, 0x03a8,  /* 0x10 ~ 0x17 */
    0x03a3, 0x0398, 0x039e, 0xffff, 0x00c6, 0x00e6, 0x00df, 0x00c9,  /* 0x18 ~ 0x1F */
       ' ',    '!',    '"',    '#', 0x00a4,    '%',    '&',   '\'',  /* 0x20 ~ 0x27 */
       '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',  /* 0x28 ~ 0x2F */
       '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',  /* 0x30 ~ 0x37 */
       '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',  /* 0x38 ~ 0x3F */
    0x00a1,    'A',    'B',    'C',    'D',    'E',    'F',    'G',  /* 0x40 ~ 0x47 */
       'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',  /* 0x48 ~ 0x4F */
       'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',  /* 0x50 ~ 0x57 */
       'X',    'Y',    'Z', 0x00c4, 0x00d6, 0x00d1, 0x00dc, 0x00a7,  /* 0x58 ~ 0x5F */
    0x00bf,    'a',    'b',    'c',    'd',    'e',    'f',    'g',  /* 0x60 ~ 0x67 */
       'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',  /* 0x68 ~ 0x6F */
       'p',    'q',    'r',    's',    't',    'u',    'v',    'w',  /* 0x70 ~ 0x77 */
       'x',    'y',    'z', 0x00e4, 0x00f6, 0x00f1, 0x00fc, 0x00e0   /* 0x78 ~ 0x7F */
    },
};

const char16_t langShiftTbl[][128] =

{
    {
    /*   0       1       2       3       4       5       6       7
         8       9       A       B       C       D       E       F    */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x00 ~ 0x07 */
    0x0000, 0x0000, 0x000c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x08 ~ 0x0F */
    0x0000, 0x0000, 0x0000, 0x0000,    '^', 0x0000, 0x0000, 0x0000,  /* 0x10 ~ 0x17 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x18 ~ 0x1F */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x20 ~ 0x27 */
       '{',    '}', 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   '\\',  /* 0x28 ~ 0x2F */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x30 ~ 0x37 */
    0x0000, 0x0000, 0x0000, 0x0000,    '[',    '~',    ']', 0x0000,  /* 0x38 ~ 0x3F */
       '|', 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x40 ~ 0x47 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x48 ~ 0x4F */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x50 ~ 0x57 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x58 ~ 0x5F */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20AC, 0x0000, 0x0000,  /* 0x60 ~ 0x67 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x68 ~ 0x6F */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* 0x70 ~ 0x77 */
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000   /* 0x78 ~ 0x7F */
    },
};


const StringWrapper GsmAlphabet::sLanguageTables[] = {
        /* 3GPP TS 23.038 V9.1.1 section 6.2.1 - GSM 7 bit Default Alphabet
         01.....23.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.E.....F.....0.....1 */
        StringWrapper("@\\u00a3$\\u00a5\\u00e8\\u00e9\\u00f9\\u00ec\\u00f2\\u00c7\n\\u00d8\\u00f8\r\\u00c5\\u00e5\\u0394_"
            // 2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....
            "\\u03a6\\u0393\\u039b\\u03a9\\u03a0\\u03a8\\u03a3\\u0398\\u039e\\uffff\\u00c6\\u00e6\\u00df"
            // F.....012.34.....56789ABCDEF0123456789ABCDEF0.....123456789ABCDEF0123456789A
            "\\u00c9 !\"#\\u00a4%&'()*+,-./0123456789:;<=>?\\u00a1ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            // B.....C.....D.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....
            "\\u00c4\\u00d6\\u00d1\\u00dc\\u00a7\\u00bfabcdefghijklmnopqrstuvwxyz\\u00e4\\u00f6\\u00f1"
            // E.....F.....
            "\\u00fc\\u00e0"),

        /* A.3.1 Turkish National Language Locking Shift Table
         01.....23.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.E.....F.....0.....1 */
        StringWrapper("@\\u00a3$\\u00a5\\u20ac\\u00e9\\u00f9\\u0131\\u00f2\\u00c7\n\\u011e\\u011f\r\\u00c5\\u00e5\\u0394_"
            // 2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....
            "\\u03a6\\u0393\\u039b\\u03a9\\u03a0\\u03a8\\u03a3\\u0398\\u039e\\uffff\\u015e\\u015f\\u00df"
            // F.....012.34.....56789ABCDEF0123456789ABCDEF0.....123456789ABCDEF0123456789A
            "\\u00c9 !\"#\\u00a4%&'()*+,-./0123456789:;<=>?\\u0130ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            // B.....C.....D.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....
            "\\u00c4\\u00d6\\u00d1\\u00dc\\u00a7\\u00e7abcdefghijklmnopqrstuvwxyz\\u00e4\\u00f6\\u00f1"
            // E.....F.....
            "\\u00fc\\u00e0"),

        /* A.3.2 Void (no locking shift table for Spanish) */
        StringWrapper(""),

        /* A.3.3 Portuguese National Language Locking Shift Table
         01.....23.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.E.....F.....0.....1 */
        StringWrapper("@\\u00a3$\\u00a5\\u00ea\\u00e9\\u00fa\\u00ed\\u00f3\\u00e7\n\\u00d4\\u00f4\r\\u00c1\\u00e1\\u0394_"
            // 2.....3.....4.....5.....67.8.....9.....AB.....C.....D.....E.....F.....012.34.....
            "\\u00aa\\u00c7\\u00c0\\u221e^\\\\u20ac\\u00d3|\\uffff\\u00c2\\u00e2\\u00ca\\u00c9 !\"#\\u00ba"
            // 56789ABCDEF0123456789ABCDEF0.....123456789ABCDEF0123456789AB.....C.....D.....E.....
            "%&'()*+,-./0123456789:;<=>?\\u00cdABCDEFGHIJKLMNOPQRSTUVWXYZ\\u00c3\\u00d5\\u00da\\u00dc"
            // F.....0123456789ABCDEF0123456789AB.....C.....DE.....F.....
            "\\u00a7~abcdefghijklmnopqrstuvwxyz\\u00e3\\u00f5`\\u00fc\\u00e0"),

        /* A.3.4 Bengali National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....CD.EF.....0..... */
        StringWrapper("\\u0981\\u0982\\u0983\\u0985\\u0986\\u0987\\u0988\\u0989\\u098a\\u098b\n\\u098c \r \\u098f\\u0990"
            // 123.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....F.....
            "  \\u0993\\u0994\\u0995\\u0996\\u0997\\u0998\\u0999\\u099a\\uffff\\u099b\\u099c\\u099d\\u099e"
            // 012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789ABC
            " !\\u099f\\u09a0\\u09a1\\u09a2\\u09a3\\u09a4)(\\u09a5\\u09a6,\\u09a7.\\u09a80123456789:; "
            // D.....E.....F0.....1.....2.....3.....4.....56.....789A.....B.....C.....D.....
            "\\u09aa\\u09ab?\\u09ac\\u09ad\\u09ae\\u09af\\u09b0 \\u09b2   \\u09b6\\u09b7\\u09b8\\u09b9"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....789.....A.....BCD.....E.....
            "\\u09bc\\u09bd\\u09be\\u09bf\\u09c0\\u09c1\\u09c2\\u09c3\\u09c4  \\u09c7\\u09c8  \\u09cb\\u09cc"
            // F.....0.....123456789ABCDEF0123456789AB.....C.....D.....E.....F.....
            "\\u09cd\\u09ceabcdefghijklmnopqrstuvwxyz\\u09d7\\u09dc\\u09dd\\u09f0\\u09f1"),

        /* A.3.5 Gujarati National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.EF.....0.....*/
        StringWrapper("\\u0a81\\u0a82\\u0a83\\u0a85\\u0a86\\u0a87\\u0a88\\u0a89\\u0a8a\\u0a8b\n\\u0a8c\\u0a8d\r \\u0a8f\\u0a90"
            // 1.....23.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....
            "\\u0a91 \\u0a93\\u0a94\\u0a95\\u0a96\\u0a97\\u0a98\\u0a99\\u0a9a\\uffff\\u0a9b\\u0a9c\\u0a9d"
            // F.....012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789AB
            "\\u0a9e !\\u0a9f\\u0aa0\\u0aa1\\u0aa2\\u0aa3\\u0aa4)(\\u0aa5\\u0aa6,\\u0aa7.\\u0aa80123456789:;"
            // CD.....E.....F0.....1.....2.....3.....4.....56.....7.....89.....A.....B.....C.....
            " \\u0aaa\\u0aab?\\u0aac\\u0aad\\u0aae\\u0aaf\\u0ab0 \\u0ab2\\u0ab3 \\u0ab5\\u0ab6\\u0ab7\\u0ab8"
            // D.....E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89.....A.....
            "\\u0ab9\\u0abc\\u0abd\\u0abe\\u0abf\\u0ac0\\u0ac1\\u0ac2\\u0ac3\\u0ac4\\u0ac5 \\u0ac7\\u0ac8"
            // B.....CD.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....E.....
            "\\u0ac9 \\u0acb\\u0acc\\u0acd\\u0ad0abcdefghijklmnopqrstuvwxyz\\u0ae0\\u0ae1\\u0ae2\\u0ae3"
            // F.....
            "\\u0af1"),

        /* A.3.6 Hindi National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.E.....F.....*/
        StringWrapper("\\u0901\\u0902\\u0903\\u0905\\u0906\\u0907\\u0908\\u0909\\u090a\\u090b\n\\u090c\\u090d\r\\u090e\\u090f"
            // 0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....
            "\\u0910\\u0911\\u0912\\u0913\\u0914\\u0915\\u0916\\u0917\\u0918\\u0919\\u091a\\uffff\\u091b\\u091c"
            // E.....F.....012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....012345
            "\\u091d\\u091e !\\u091f\\u0920\\u0921\\u0922\\u0923\\u0924)(\\u0925\\u0926,\\u0927.\\u0928012345"
            // 6789ABC.....D.....E.....F0.....1.....2.....3.....4.....5.....6.....7.....8.....
            "6789:;\\u0929\\u092a\\u092b?\\u092c\\u092d\\u092e\\u092f\\u0930\\u0931\\u0932\\u0933\\u0934"
            // 9.....A.....B.....C.....D.....E.....F.....0.....1.....2.....3.....4.....5.....6.....
            "\\u0935\\u0936\\u0937\\u0938\\u0939\\u093c\\u093d\\u093e\\u093f\\u0940\\u0941\\u0942\\u0943\\u0944"
            // 7.....8.....9.....A.....B.....C.....D.....E.....F.....0.....123456789ABCDEF012345678
            "\\u0945\\u0946\\u0947\\u0948\\u0949\\u094a\\u094b\\u094c\\u094d\\u0950abcdefghijklmnopqrstuvwx"
            // 9AB.....C.....D.....E.....F.....
            "yz\\u0972\\u097b\\u097c\\u097e\\u097f"),

        /* A.3.7 Kannada National Language Locking Shift Table
           NOTE: TS 23.038 V9.1.1 shows code 0x24 as \\u0caa, corrected to \\u0ca1 (typo)
         01.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....CD.E.....F.....0.....1 */
        StringWrapper(" \\u0c82\\u0c83\\u0c85\\u0c86\\u0c87\\u0c88\\u0c89\\u0c8a\\u0c8b\n\\u0c8c \r\\u0c8e\\u0c8f\\u0c90 "
            // 2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....F.....
            "\\u0c92\\u0c93\\u0c94\\u0c95\\u0c96\\u0c97\\u0c98\\u0c99\\u0c9a\\uffff\\u0c9b\\u0c9c\\u0c9d\\u0c9e"
            // 012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789ABC
            " !\\u0c9f\\u0ca0\\u0ca1\\u0ca2\\u0ca3\\u0ca4)(\\u0ca5\\u0ca6,\\u0ca7.\\u0ca80123456789:; "
            // D.....E.....F0.....1.....2.....3.....4.....5.....6.....7.....89.....A.....B.....
            "\\u0caa\\u0cab?\\u0cac\\u0cad\\u0cae\\u0caf\\u0cb0\\u0cb1\\u0cb2\\u0cb3 \\u0cb5\\u0cb6\\u0cb7"
            // C.....D.....E.....F.....0.....1.....2.....3.....4.....5.....6.....78.....9.....
            "\\u0cb8\\u0cb9\\u0cbc\\u0cbd\\u0cbe\\u0cbf\\u0cc0\\u0cc1\\u0cc2\\u0cc3\\u0cc4 \\u0cc6\\u0cc7"
            // A.....BC.....D.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....
            "\\u0cc8 \\u0cca\\u0ccb\\u0ccc\\u0ccd\\u0cd5abcdefghijklmnopqrstuvwxyz\\u0cd6\\u0ce0\\u0ce1"
            // E.....F.....
            "\\u0ce2\\u0ce3"),

        /* A.3.8 Malayalam National Language Locking Shift Table
         01.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....CD.E.....F.....0.....1 */
        StringWrapper(" \\u0d02\\u0d03\\u0d05\\u0d06\\u0d07\\u0d08\\u0d09\\u0d0a\\u0d0b\n\\u0d0c \r\\u0d0e\\u0d0f\\u0d10 "
            // 2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....F.....
            "\\u0d12\\u0d13\\u0d14\\u0d15\\u0d16\\u0d17\\u0d18\\u0d19\\u0d1a\\uffff\\u0d1b\\u0d1c\\u0d1d\\u0d1e"
            // 012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789ABC
            " !\\u0d1f\\u0d20\\u0d21\\u0d22\\u0d23\\u0d24)(\\u0d25\\u0d26,\\u0d27.\\u0d280123456789:; "
            // D.....E.....F0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.....
            "\\u0d2a\\u0d2b?\\u0d2c\\u0d2d\\u0d2e\\u0d2f\\u0d30\\u0d31\\u0d32\\u0d33\\u0d34\\u0d35\\u0d36"
            // B.....C.....D.....EF.....0.....1.....2.....3.....4.....5.....6.....78.....9.....
            "\\u0d37\\u0d38\\u0d39 \\u0d3d\\u0d3e\\u0d3f\\u0d40\\u0d41\\u0d42\\u0d43\\u0d44 \\u0d46\\u0d47"
            // A.....BC.....D.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....
            "\\u0d48 \\u0d4a\\u0d4b\\u0d4c\\u0d4d\\u0d57abcdefghijklmnopqrstuvwxyz\\u0d60\\u0d61\\u0d62"
            // E.....F.....
            "\\u0d63\\u0d79"),

        /* A.3.9 Oriya National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....CD.EF.....0.....12 */
        StringWrapper("\\u0b01\\u0b02\\u0b03\\u0b05\\u0b06\\u0b07\\u0b08\\u0b09\\u0b0a\\u0b0b\n\\u0b0c \r \\u0b0f\\u0b10  "
            // 3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....F.....01
            "\\u0b13\\u0b14\\u0b15\\u0b16\\u0b17\\u0b18\\u0b19\\u0b1a\\uffff\\u0b1b\\u0b1c\\u0b1d\\u0b1e !"
            // 2.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789ABCD.....
            "\\u0b1f\\u0b20\\u0b21\\u0b22\\u0b23\\u0b24)(\\u0b25\\u0b26,\\u0b27.\\u0b280123456789:; \\u0b2a"
            // E.....F0.....1.....2.....3.....4.....56.....7.....89.....A.....B.....C.....D.....
            "\\u0b2b?\\u0b2c\\u0b2d\\u0b2e\\u0b2f\\u0b30 \\u0b32\\u0b33 \\u0b35\\u0b36\\u0b37\\u0b38\\u0b39"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....789.....A.....BCD.....E.....
            "\\u0b3c\\u0b3d\\u0b3e\\u0b3f\\u0b40\\u0b41\\u0b42\\u0b43\\u0b44  \\u0b47\\u0b48  \\u0b4b\\u0b4c"
            // F.....0.....123456789ABCDEF0123456789AB.....C.....D.....E.....F.....
            "\\u0b4d\\u0b56abcdefghijklmnopqrstuvwxyz\\u0b57\\u0b60\\u0b61\\u0b62\\u0b63"),

        /* A.3.10 Punjabi National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9A.BCD.EF.....0.....123.....4.....*/
        StringWrapper("\\u0a01\\u0a02\\u0a03\\u0a05\\u0a06\\u0a07\\u0a08\\u0a09\\u0a0a \n  \r \\u0a0f\\u0a10  \\u0a13\\u0a14"
            // 5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....F.....012.....3.....
            "\\u0a15\\u0a16\\u0a17\\u0a18\\u0a19\\u0a1a\\uffff\\u0a1b\\u0a1c\\u0a1d\\u0a1e !\\u0a1f\\u0a20"
            // 4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789ABCD.....E.....F0.....
            "\\u0a21\\u0a22\\u0a23\\u0a24)(\\u0a25\\u0a26,\\u0a27.\\u0a280123456789:; \\u0a2a\\u0a2b?\\u0a2c"
            // 1.....2.....3.....4.....56.....7.....89.....A.....BC.....D.....E.....F0.....1.....
            "\\u0a2d\\u0a2e\\u0a2f\\u0a30 \\u0a32\\u0a33 \\u0a35\\u0a36 \\u0a38\\u0a39\\u0a3c \\u0a3e\\u0a3f"
            // 2.....3.....4.....56789.....A.....BCD.....E.....F.....0.....123456789ABCDEF012345678
            "\\u0a40\\u0a41\\u0a42    \\u0a47\\u0a48  \\u0a4b\\u0a4c\\u0a4d\\u0a51abcdefghijklmnopqrstuvwx"
            // 9AB.....C.....D.....E.....F.....
            "yz\\u0a70\\u0a71\\u0a72\\u0a73\\u0a74"),

        /* A.3.11 Tamil National Language Locking Shift Table
         01.....2.....3.....4.....5.....6.....7.....8.....9A.BCD.E.....F.....0.....12.....3..... */
        StringWrapper(" \\u0b82\\u0b83\\u0b85\\u0b86\\u0b87\\u0b88\\u0b89\\u0b8a \n  \r\\u0b8e\\u0b8f\\u0b90 \\u0b92\\u0b93"
            // 4.....5.....6789.....A.....B.....CD.....EF.....012.....3456.....7.....89ABCDEF.....
            "\\u0b94\\u0b95   \\u0b99\\u0b9a\\uffff \\u0b9c \\u0b9e !\\u0b9f   \\u0ba3\\u0ba4)(  , .\\u0ba8"
            // 0123456789ABC.....D.....EF012.....3.....4.....5.....6.....7.....8.....9.....A.....
            "0123456789:;\\u0ba9\\u0baa ?  \\u0bae\\u0baf\\u0bb0\\u0bb1\\u0bb2\\u0bb3\\u0bb4\\u0bb5\\u0bb6"
            // B.....C.....D.....EF0.....1.....2.....3.....4.....5678.....9.....A.....BC.....D.....
            "\\u0bb7\\u0bb8\\u0bb9  \\u0bbe\\u0bbf\\u0bc0\\u0bc1\\u0bc2   \\u0bc6\\u0bc7\\u0bc8 \\u0bca\\u0bcb"
            // E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....E.....F.....
            "\\u0bcc\\u0bcd\\u0bd0abcdefghijklmnopqrstuvwxyz\\u0bd7\\u0bf0\\u0bf1\\u0bf2\\u0bf9"),

        /* A.3.12 Telugu National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....CD.E.....F.....0.....*/
        StringWrapper("\\u0c01\\u0c02\\u0c03\\u0c05\\u0c06\\u0c07\\u0c08\\u0c09\\u0c0a\\u0c0b\n\\u0c0c \r\\u0c0e\\u0c0f\\u0c10"
            // 12.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....E.....
            " \\u0c12\\u0c13\\u0c14\\u0c15\\u0c16\\u0c17\\u0c18\\u0c19\\u0c1a\\uffff\\u0c1b\\u0c1c\\u0c1d"
            // F.....012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....0123456789AB
            "\\u0c1e !\\u0c1f\\u0c20\\u0c21\\u0c22\\u0c23\\u0c24)(\\u0c25\\u0c26,\\u0c27.\\u0c280123456789:;"
            // CD.....E.....F0.....1.....2.....3.....4.....5.....6.....7.....89.....A.....B.....
            " \\u0c2a\\u0c2b?\\u0c2c\\u0c2d\\u0c2e\\u0c2f\\u0c30\\u0c31\\u0c32\\u0c33 \\u0c35\\u0c36\\u0c37"
            // C.....D.....EF.....0.....1.....2.....3.....4.....5.....6.....78.....9.....A.....B
            "\\u0c38\\u0c39 \\u0c3d\\u0c3e\\u0c3f\\u0c40\\u0c41\\u0c42\\u0c43\\u0c44 \\u0c46\\u0c47\\u0c48 "
            // C.....D.....E.....F.....0.....123456789ABCDEF0123456789AB.....C.....D.....E.....
            "\\u0c4a\\u0c4b\\u0c4c\\u0c4d\\u0c55abcdefghijklmnopqrstuvwxyz\\u0c56\\u0c60\\u0c61\\u0c62"
            // F.....
            "\\u0c63"),

        /* A.3.13 Urdu National Language Locking Shift Table
         0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.B.....C.....D.E.....F.....*/
        StringWrapper("\\u0627\\u0622\\u0628\\u067b\\u0680\\u067e\\u06a6\\u062a\\u06c2\\u067f\n\\u0679\\u067d\r\\u067a\\u067c"
            // 0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.....B.....C.....D.....
            "\\u062b\\u062c\\u0681\\u0684\\u0683\\u0685\\u0686\\u0687\\u062d\\u062e\\u062f\\uffff\\u068c\\u0688"
            // E.....F.....012.....3.....4.....5.....6.....7.....89A.....B.....CD.....EF.....012345
            "\\u0689\\u068a !\\u068f\\u068d\\u0630\\u0631\\u0691\\u0693)(\\u0699\\u0632,\\u0696.\\u0698012345"
            // 6789ABC.....D.....E.....F0.....1.....2.....3.....4.....5.....6.....7.....8.....
            "6789:;\\u069a\\u0633\\u0634?\\u0635\\u0636\\u0637\\u0638\\u0639\\u0641\\u0642\\u06a9\\u06aa"
            // 9.....A.....B.....C.....D.....E.....F.....0.....1.....2.....3.....4.....5.....6.....
            "\\u06ab\\u06af\\u06b3\\u06b1\\u0644\\u0645\\u0646\\u06ba\\u06bb\\u06bc\\u0648\\u06c4\\u06d5\\u06c1"
            // 7.....8.....9.....A.....B.....C.....D.....E.....F.....0.....123456789ABCDEF012345678
            "\\u06be\\u0621\\u06cc\\u06d0\\u06d2\\u064d\\u0650\\u064f\\u0657\\u0654abcdefghijklmnopqrstuvwx"
            // 9AB.....C.....D.....E.....F.....
            "yz\\u0655\\u0651\\u0653\\u0656\\u0670")
    };

const StringWrapper GsmAlphabet::sLanguageShiftTables[] = {
        /* 6.2.1.1 GSM 7 bit Default Alphabet Extension Table
         0123456789A.....BCDEF0123456789ABCDEF0123456789ABCDEF.0123456789ABCDEF0123456789ABCDEF */
        StringWrapper("          \\u000c         ^                   {}     \\            [~] |               "
            // 0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            "                     \\u20ac                          "),

        /* A.2.1 Turkish National Language Single Shift Table
         0123456789A.....BCDEF0123456789ABCDEF0123456789ABCDEF.0123456789ABCDEF01234567.....8 */
        StringWrapper("          \\u000c         ^                   {}     \\            [~] |      \\u011e "
            // 9.....ABCDEF0123.....456789ABCDEF0123.....45.....67.....89.....ABCDEF0123.....
            "\\u0130         \\u015e               \\u00e7 \\u20ac \\u011f \\u0131         \\u015f"
            // 456789ABCDEF
            "            "),

        /* A.2.2 Spanish National Language Single Shift Table
         0123456789.....A.....BCDEF0123456789ABCDEF0123456789ABCDEF.0123456789ABCDEF01.....23 */
        StringWrapper("         \\u00e7\\u000c         ^                   {}     \\            [~] |\\u00c1  "
            // 456789.....ABCDEF.....012345.....6789ABCDEF01.....2345.....6789.....ABCDEF.....012
            "     \\u00cd     \\u00d3     \\u00da           \\u00e1   \\u20ac   \\u00ed     \\u00f3   "
            // 345.....6789ABCDEF
            "  \\u00fa          "),

        /* A.2.3 Portuguese National Language Single Shift Table
         012345.....6789.....A.....B.....C.....DE.....F.....012.....3.....45.....6.....7.....8....*/
        StringWrapper("     \\u00ea   \\u00e7\\u000c\\u00d4\\u00f4 \\u00c1\\u00e1  \\u03a6\\u0393^\\u03a9\\u03a0\\u03a8\\u03a3"
            // 9.....ABCDEF.....0123456789ABCDEF.0123456789ABCDEF01.....23456789.....ABCDE
            "\\u0398     \\u00ca        {}     \\            [~] |\\u00c0       \\u00cd     "
            // F.....012345.....6789AB.....C.....DEF01.....2345.....6789.....ABCDEF.....01234
            "\\u00d3     \\u00da     \\u00c3\\u00d5    \\u00c2   \\u20ac   \\u00ed     \\u00f3     "
            // 5.....6789AB.....C.....DEF.....
            "\\u00fa     \\u00e3\\u00f5  \\u00e2"),

        /* A.2.4 Bengali National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u09e6\\u09e7 \\u09e8\\u09e9"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u09ea\\u09eb\\u09ec\\u09ed\\u09ee\\u09ef\\u09df\\u09e0\\u09e1\\u09e2{}\\u09e3\\u09f2\\u09f3"
            // D.....E.....F.0.....1.....2.....3.....4.....56789ABCDEF0123456789ABCDEF
            "\\u09f4\\u09f5\\\\u09f6\\u09f7\\u09f8\\u09f9\\u09fa       [~] |ABCDEFGHIJKLMNO"
            // 0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            "PQRSTUVWXYZ          \\u20ac                          "),

        /* A.2.5 Gujarati National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0ae6\\u0ae7"
            // E.....F.....0.....1.....2.....3.....4.....5.....6789ABCDEF.0123456789ABCDEF
            "\\u0ae8\\u0ae9\\u0aea\\u0aeb\\u0aec\\u0aed\\u0aee\\u0aef  {}     \\            [~] "
            // 0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            "|ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac                          "),

        /* A.2.6 Hindi National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0966\\u0967"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u0968\\u0969\\u096a\\u096b\\u096c\\u096d\\u096e\\u096f\\u0951\\u0952{}\\u0953\\u0954\\u0958"
            // D.....E.....F.0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.....
            "\\u0959\\u095a\\\\u095b\\u095c\\u095d\\u095e\\u095f\\u0960\\u0961\\u0962\\u0963\\u0970\\u0971"
            // BCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            " [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac                          "),

        /* A.2.7 Kannada National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0ce6\\u0ce7"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....BCDEF.01234567
            "\\u0ce8\\u0ce9\\u0cea\\u0ceb\\u0cec\\u0ced\\u0cee\\u0cef\\u0cde\\u0cf1{}\\u0cf2    \\        "
            // 89ABCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            "    [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac                          "),

        /* A.2.8 Malayalam National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0d66\\u0d67"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u0d68\\u0d69\\u0d6a\\u0d6b\\u0d6c\\u0d6d\\u0d6e\\u0d6f\\u0d70\\u0d71{}\\u0d72\\u0d73\\u0d74"
            // D.....E.....F.0.....1.....2.....3.....4.....56789ABCDEF0123456789ABCDEF0123456789A
            "\\u0d75\\u0d7a\\\\u0d7b\\u0d7c\\u0d7d\\u0d7e\\u0d7f       [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            // BCDEF012345.....6789ABCDEF0123456789ABCDEF
            "          \\u20ac                          "),

        /* A.2.9 Oriya National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0b66\\u0b67"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....DE
            "\\u0b68\\u0b69\\u0b6a\\u0b6b\\u0b6c\\u0b6d\\u0b6e\\u0b6f\\u0b5c\\u0b5d{}\\u0b5f\\u0b70\\u0b71  "
            // F.0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF0123456789A
            "\\            [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac                     "
            // BCDEF
            "     "),

        /* A.2.10 Punjabi National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0a66\\u0a67"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u0a68\\u0a69\\u0a6a\\u0a6b\\u0a6c\\u0a6d\\u0a6e\\u0a6f\\u0a59\\u0a5a{}\\u0a5b\\u0a5c\\u0a5e"
            // D.....EF.0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF01
            "\\u0a75 \\            [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac            "
            // 23456789ABCDEF
            "              "),

        /* A.2.11 Tamil National Language Single Shift Table
           NOTE: TS 23.038 V9.1.1 shows code 0x24 as \\u0bef, corrected to \\u0bee (typo)
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0964\\u0965 \\u0be6\\u0be7"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u0be8\\u0be9\\u0bea\\u0beb\\u0bec\\u0bed\\u0bee\\u0bef\\u0bf3\\u0bf4{}\\u0bf5\\u0bf6\\u0bf7"
            // D.....E.....F.0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABC
            "\\u0bf8\\u0bfa\\            [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac       "
            // DEF0123456789ABCDEF
            "                   "),

        /* A.2.12 Telugu National Language Single Shift Table
           NOTE: TS 23.038 V9.1.1 shows code 0x22-0x23 as \\u06cc\\u06cd, corrected to \\u0c6c\\u0c6d
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789ABC.....D.....E.....F..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*   \\u0c66\\u0c67\\u0c68\\u0c69"
            // 0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....D.....E.....F.
            "\\u0c6a\\u0c6b\\u0c6c\\u0c6d\\u0c6e\\u0c6f\\u0c58\\u0c59{}\\u0c78\\u0c79\\u0c7a\\u0c7b\\u0c7c\\"
            // 0.....1.....2.....3456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345.....6789ABCD
            "\\u0c7d\\u0c7e\\u0c7f         [~] |ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac        "
            // EF0123456789ABCDEF
            "                  "),

        /* A.2.13 Urdu National Language Single Shift Table
         01.....23.....4.....5.6.....789A.....BCDEF0123.....45.....6789.....A.....BC.....D..... */
        StringWrapper("@\\u00a3$\\u00a5\\u00bf\"\\u00a4%&'\\u000c*+ -/<=>\\u00a1^\\u00a1_#*\\u0600\\u0601 \\u06f0\\u06f1"
            // E.....F.....0.....1.....2.....3.....4.....5.....6.....7.....89A.....B.....C.....
            "\\u06f2\\u06f3\\u06f4\\u06f5\\u06f6\\u06f7\\u06f8\\u06f9\\u060c\\u060d{}\\u060e\\u060f\\u0610"
            // D.....E.....F.0.....1.....2.....3.....4.....5.....6.....7.....8.....9.....A.....
            "\\u0611\\u0612\\\\u0613\\u0614\\u061b\\u061f\\u0640\\u0652\\u0658\\u066b\\u066c\\u0672\\u0673"
            // B.....CDEF.....0123456789ABCDEF0123456789ABCDEF012345.....6789ABCDEF0123456789ABCDEF
            "\\u06cd[~]\\u06d4|ABCDEFGHIJKLMNOPQRSTUVWXYZ          \\u20ac                          ")
    };

GsmAlphabet::init GsmAlphabet::initializer;

GsmAlphabet::init::init() {

}

String16 GsmAlphabet::gsm7BitPackedToString(uint8_t* pdu, int32_t offset,
        int32_t lengthSeptets) {
    return gsm7BitPackedToString(pdu, offset, lengthSeptets, 0, 0, 0);
}

String16 GsmAlphabet::gsm7BitPackedToString(uint8_t* pdu, int32_t offset,
        int32_t lengthSeptets, int32_t numPaddingBits, int32_t languageTable, int32_t shiftTable) {
    String16 ret;
	int32_t max_user_data_septets = 160;
    char16_t septepBuf[(sizeof(char16_t) * max_user_data_septets) + 1] = {0};
    uint16_t septepBufLen = 0;

    if (languageTable < 0 || languageTable > sTableLength) {
        RLOGW("unknown language table %d, using default", languageTable);
        languageTable = 0;
    }
    if (shiftTable < 0 || shiftTable > sTableLength) {
        RLOGW("unknown single shift table %d, using default", shiftTable);
        shiftTable = 0;
    }

    bool prevCharWasEscape = false;
#if 1
    const char16_t* languageTableToChar = langTbl[0];
    const char16_t* shiftTableToChar = langShiftTbl[0];
#else
    StringWrapper languageTableToChar = sLanguageTables[languageTable];
    StringWrapper shiftTableToChar = sLanguageShiftTables[shiftTable];

    if (languageTableToChar.isEmpty()) {
        RLOGW("no language table for code %d, using default", languageTable);
        languageTableToChar = sLanguageTables[0];
    }
    if (shiftTableToChar.isEmpty()) {
        RLOGW("no single shift table for code %d, using default", shiftTable);
        shiftTableToChar = sLanguageShiftTables[0];
    }
#endif

    for (int32_t i = 0 ; i < lengthSeptets ; i++) {
        int32_t bitOffset = (7 * i) + numPaddingBits;

        int32_t byteOffset = bitOffset / 8;
        int32_t shift = bitOffset % 8;
        int32_t gsmVal;

        gsmVal = (0x7f & (pdu[offset + byteOffset] >> shift));

        // if it crosses a byte boundary
        if (shift > 1) {
            // set msb bits to 0
            gsmVal &= 0x7f >> (shift - 1);

            gsmVal |= 0x7f & (pdu[offset + byteOffset + 1] << (8 - shift));
        }

        if (prevCharWasEscape) {
            if (gsmVal == GSM_EXTENDED_ESCAPE) {
                //ret += ' '; // display ' ' for reserved double escape sequence
                septepBuf[septepBufLen++] = ' ';
            } else {
                char16_t c = shiftTableToChar[gsmVal];
                if (c == ' ') {
                    septepBuf[septepBufLen++] = languageTableToChar[gsmVal];
                } else {
                    septepBuf[septepBufLen++] = c;
                }
            }
            prevCharWasEscape = false;
        } else if (gsmVal == GSM_EXTENDED_ESCAPE) {
            prevCharWasEscape = true;
        } else {
            septepBuf[septepBufLen++] = languageTableToChar[gsmVal];
        }
    }

    septepBuf[septepBufLen] = '\0';
    ret.setTo(septepBuf);

    return ret;
}

/**
 * Convert a GSM alphabet string that's stored in 8-bit unpacked
 * format (as it often appears in SIM records) into a String
 *
 * Field may be padded with trailing 0xff's. The decode stops
 * at the first 0xff encountered.
 *
 * Additionally, in some country(ex. Korea), there are non-ASCII or MBCS characters.
 * If a character set is given, characters in data are treat as MBCS.
 */
sp<String> GsmAlphabet::gsm8BitUnpackedToString(uint8_t* data, int32_t offset, int32_t length) {
    return gsm8BitUnpackedToString(data, offset, length, "");
}

/**
 * Convert a GSM alphabet string that's stored in 8-bit unpacked
 * format (as it often appears in SIM records) into a String
 *
 * Field may be padded with trailing 0xff's. The decode stops
 * at the first 0xff encountered.
 *
 * Additionally, in some country(ex. Korea), there are non-ASCII or MBCS characters.
 * If a character set is given, characters in data are treat as MBCS.
 */
sp<String> GsmAlphabet::gsm8BitUnpackedToString(uint8_t* data, int32_t offset, int32_t length, const char* characterset) {
/*    bool isMbcs = false;*///CID-708032
/*
    Charset charset = null;
    ByteBuffer mbcsBuffer = null;
*/
/*
    if (!TextUtils.isEmpty(characterset)
            && !characterset.equalsIgnoreCase("us-ascii")
            && Charset.isSupported(characterset)) {
        isMbcs = true;
        charset = Charset.forName(characterset);
        mbcsBuffer = ByteBuffer.allocate(2);
    }
*/
    // Always use GSM 7 bit default alphabet table for this method
    StringWrapper languageTableToChar = sLanguageTables[0];
    StringWrapper shiftTableToChar = sLanguageShiftTables[0];

    StringWrapper ret;
    bool prevWasEscape = false;
    for (int32_t i = offset ; i < offset + length ; i++) {
        // Never underestimate the pain that can be caused
        // by signed bytes
        int32_t c = data[i] & 0xff;

        if (c == 0xff) {
            break;
        } else if (c == GSM_EXTENDED_ESCAPE) {
            if (prevWasEscape) {
                // Two escape chars in a row
                // We treat this as a space
                // See Note 1 in table 6.2.1.1 of TS 23.038 v7.00
                ret += ' ';
                prevWasEscape = false;
            } else {
                prevWasEscape = true;
            }
        } else {
            if (prevWasEscape) {
                char shiftChar = shiftTableToChar.charAt(c);
                if (shiftChar == ' ') {
                    // display character from main table if not present in shift table
                    ret += languageTableToChar.charAt(c);
                } else {
                    ret += shiftChar;
                }
            } else {
                // CID 12140 : 'Constant' variable guards dead code (DEADCODE)
                // FROM : if (!isMbcs || c < 0x80 || i + 1 >= offset + length) {
                // TO :
                if (c < 0x80 || i + 1 >= offset + length) {
				// CID 12140 END
                    ret += languageTableToChar.charAt(c);
                } /*else {
                    // isMbcs must be true. So both mbcsBuffer and charset are initialized.
                    mbcsBuffer.clear();
                    mbcsBuffer.put(data, i++, 2);
                    mbcsBuffer.flip();
                    ret.append(charset.decode(mbcsBuffer).toString());
                }*/
            }
            prevWasEscape = false;
        }
    }

    return new String(ret.c_str());
}

/**
 * Convert a string into an 8-bit unpacked GSM alphabet byte array.
 * Always uses GSM default 7-bit alphabet and extension table.
 * @param s the string to encode
 * @return the 8-bit GSM encoded byte array for the string
 */
sp<ByteArray> GsmAlphabet::stringToGsm8BitPacked(const sp<String>& s) {
/*
    byte[] ret;

    int32_t septets = countGsmSeptetsUsingTables(s, true, 0, 0);

    // Enough for all the septets and the length byte prefix
    ret = new byte[septets];

    stringToGsm8BitUnpackedField(s, ret, 0, ret.length);

    return ret;
*/
    return NULL;
}

/**
 * Write a String into a GSM 8-bit unpacked field of
 * Field is padded with 0xff's, string is truncated if necessary
 *
 * @param s the string to encode
 * @param dest the destination byte array
 * @param offset the starting offset for the encoded string
 * @param length the maximum number of bytes to write
 */
void GsmAlphabet::stringToGsm8BitUnpackedField(const sp<String>& s, const uint8_t* dest, int32_t offset, int32_t length) {

}

/**
 * Returns the count of 7-bit GSM alphabet characters
 * needed to represent this character. Counts unencodable char as 1 septet.
 * @param c the character to examine
 * @return the number of septets for this character
 */
int32_t GsmAlphabet::countGsmSeptets(char c) {
    return countGsmSeptets(c, false);
}

int32_t GsmAlphabet::countGsmSeptets(char c, bool throwsException) {
/*
    if (sCharsToGsmTables[0].get(c, -1) != -1) {
        return 1;
    }

    if (sCharsToShiftTables[0].get(c, -1) != -1) {
        return 2;
    }
*/
    return 1;

/*
    if (throwsException) {
        //throw new EncodeException(c);
        RLOGE("EncodeException %c", c);
        return 1;
    } else {
        // count as a space char
        return 1;
    }
*/
}

/**
 * Returns the count of 7-bit GSM alphabet characters needed
 * to represent this string, using the specified 7-bit language table
 * and extension table (0 for GSM default tables).
 * @param s the Unicode string that will be encoded
 * @param use7bitOnly allow using space in place of unencodable character if true,
 *     otherwise, return -1 if any characters are unencodable
 * @param languageTable the 7 bit language table, or 0 for the default GSM alphabet
 * @param languageShiftTable the 7 bit single shift language table, or 0 for the default
 *     GSM extension table
 * @return the septet count for s using the specified language tables, or -1 if any
 *     characters are unencodable and use7bitOnly is false
 */
int32_t GsmAlphabet::countGsmSeptetsUsingTables(char s, bool use7bitOnly, int32_t languageTable, int32_t languageShiftTable) {
    return -1;
}
