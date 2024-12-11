#pragma once
// see https://godbolt.org/z/1PjW637Mh

/**

The MIT License (MIT)

Copyright (c) SimpleHacks, Henry Gabryjelski
https://github.com/SimpleHacks/UtilHeaders

All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef COMPILE_TIMESTAMP_H
#define COMPILE_TIMESTAMP_H

#ifndef __has_feature
    #define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

// This allows conditional declaration of the below as `constexpr`,
// unless the compiler has not implemented the relevant feature.
#if __cpp_constexpr >= 200704  || __has_feature(cxx_constexpr) // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2235.pdf
    #define __COMPILE_TIMESTAMP_H_CONSTEXPR constexpr
#else
    #define __COMPILE_TIMESTAMP_H_CONSTEXPR
#endif    

// GCC preprocess __TIMESTAMP__ to one of:
//     "??? ??? ?? ??:??:?? ????" -- when last edit not determined
//     "Sun Sep 16 01:03:52 1973" -- last edit of the file
//      0....-....1....-....2...  -- indices to each character
// Thus, must define error checks and define that return value when error.
// Many options exist, including a unix tick of zero.  But, since this header
// also supports DOS date & time (e.g., for FAT structures), chose the following:
//     "Mon Jan  1 00:00:00 1980" -- What the macros effectively return on error

#define __TIMESTAMP_FAILURE__ ( \
  (__TIMESTAMP__ [ 0u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 1u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 2u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 3u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 4u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 5u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 6u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 7u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 8u] == '?') ? 1 : \
  (__TIMESTAMP__ [ 9u] == '?') ? 1 : \
  (__TIMESTAMP__ [10u] == '?') ? 1 : \
  (__TIMESTAMP__ [11u] == '?') ? 1 : \
  (__TIMESTAMP__ [12u] == '?') ? 1 : \
  (__TIMESTAMP__ [13u] == '?') ? 1 : \
  (__TIMESTAMP__ [14u] == '?') ? 1 : \
  (__TIMESTAMP__ [15u] == '?') ? 1 : \
  (__TIMESTAMP__ [16u] == '?') ? 1 : \
  (__TIMESTAMP__ [17u] == '?') ? 1 : \
  (__TIMESTAMP__ [18u] == '?') ? 1 : \
  (__TIMESTAMP__ [19u] == '?') ? 1 : \
  (__TIMESTAMP__ [20u] == '?') ? 1 : \
  (__TIMESTAMP__ [21u] == '?') ? 1 : \
  (__TIMESTAMP__ [22u] == '?') ? 1 : \
  (__TIMESTAMP__ [23u] == '?') ? 1 : \
  0 )
#define __TIMESTAMP_YEAR_IMPL__ ((( \
  (__TIMESTAMP__ [20u] - '0')  * 10u + \
  (__TIMESTAMP__ [21u] - '0')) * 10u + \
  (__TIMESTAMP__ [22u] - '0')) * 10u + \
  (__TIMESTAMP__ [23u] - '0'))
#define __TIMESTAMP_MONTH_IMPL__ ( \
      (__TIMESTAMP__ [6u] == 'n' && __TIMESTAMP__ [5u] == 'a') ?  1u  /*Jan*/ \
    : (__TIMESTAMP__ [6u] == 'b'                             ) ?  2u  /*Feb*/ \
    : (__TIMESTAMP__ [6u] == 'r' && __TIMESTAMP__ [5u] == 'a') ?  3u  /*Mar*/ \
    : (__TIMESTAMP__ [6u] == 'r'                             ) ?  4u  /*Apr*/ \
    : (__TIMESTAMP__ [6u] == 'y'                             ) ?  5u  /*May*/ \
    : (__TIMESTAMP__ [6u] == 'n'                             ) ?  6u  /*Jun*/ \
    : (__TIMESTAMP__ [6u] == 'l'                             ) ?  7u  /*Jul*/ \
    : (__TIMESTAMP__ [6u] == 'g'                             ) ?  8u  /*Aug*/ \
    : (__TIMESTAMP__ [6u] == 'p'                             ) ?  9u  /*Sep*/ \
    : (__TIMESTAMP__ [6u] == 't'                             ) ? 10u  /*Oct*/ \
    : (__TIMESTAMP__ [6u] == 'v'                             ) ? 11u  /*Nov*/ \
    :                                                            12u  /*Dec*/ )
#define __TIMESTAMP_DAY_IMPL__ (( \
   (__TIMESTAMP__ [8u] == ' ' ? 0u : __TIMESTAMP__ [8u] - '0') * 10u) + \
   (__TIMESTAMP__ [9u] - '0')                                           )
#define __TIMESTAMP_HOUR_IMPL__ (( \
   (__TIMESTAMP__ [11u] - '0') * 10u) + \
   (__TIMESTAMP__ [12u] - '0')          )
#define __TIMESTAMP_MINUTE_IMPL__ (( \
   (__TIMESTAMP__ [14u] - '0') * 10u) + \
   (__TIMESTAMP__ [15u] - '0')          )
#define __TIMESTAMP_SECONDS_IMPL__ (( \
   (__TIMESTAMP__ [17u] - '0') * 10u) + \
   (__TIMESTAMP__ [18u] - '0')          )
#define __TIMESTAMP_MSDOS_DATE_IMPL__       ( \
  ((__TIMESTAMP_YEAR_INT__  - 1980u) << 9u) | \
  ( __TIMESTAMP_MONTH_INT__          << 5u) | \
  ( __TIMESTAMP_DAY_INT__            << 0u) )
#define __TIMESTAMP_MSDOS_TIME_IMPL__   ( \
  ( __TIMESTAMP_HOUR_INT__      << 11u) | \
  ( __TIMESTAMP_MINUTE_INT__    <<  5u) | \
  ( __TIMESTAMP_SECONDS_INT__   <<  0u) )

// And the resulting usable macros....
#define __TIMESTAMP_YEAR_INT__       ( __TIMESTAMP_FAILURE__ ? 1980u : __TIMESTAMP_YEAR_IMPL__    )
#define __TIMESTAMP_MONTH_INT__      ( __TIMESTAMP_FAILURE__ ?    1u : __TIMESTAMP_MONTH_IMPL__   )
#define __TIMESTAMP_DAY_INT__        ( __TIMESTAMP_FAILURE__ ?    1u : __TIMESTAMP_DAY_IMPL__     )
#define __TIMESTAMP_HOUR_INT__       ( __TIMESTAMP_FAILURE__ ?    0u : __TIMESTAMP_HOUR_IMPL__    )
#define __TIMESTAMP_MINUTE_INT__     ( __TIMESTAMP_FAILURE__ ?    0u : __TIMESTAMP_MINUTE_IMPL__  )
#define __TIMESTAMP_SECONDS_INT__    ( __TIMESTAMP_FAILURE__ ?    0u : __TIMESTAMP_SECONDS_IMPL__ )

#define __TIMESTAMP_MSDOS_DATE_INT__ ( __TIMESTAMP_MSDOS_DATE_IMPL__ )
#define __TIMESTAMP_MSDOS_TIME_INT__ ( __TIMESTAMP_MSDOS_TIME_IMPL__ )

__COMPILE_TIMESTAMP_H_CONSTEXPR
static const char __TIMESTAMP_ISO8601_DATE__[] =
{
    __TIMESTAMP_FAILURE__ ? '1' : (char)(( (__TIMESTAMP_YEAR_INT__    / 1000) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '9' : (char)(( (__TIMESTAMP_YEAR_INT__    /  100) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_YEAR_INT__    /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_YEAR_INT__    /    1) % 10 ) + '0'),
    '-',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MONTH_INT__   /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MONTH_INT__   /    1) % 10 ) + '0'),
    '-',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_DAY_INT__     /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_DAY_INT__     /    1) % 10 ) + '0'),
    '\0'
};

__COMPILE_TIMESTAMP_H_CONSTEXPR
const char __TIMESTAMP_ISO8601_DATETIME__[] =
{
    __TIMESTAMP_FAILURE__ ? '1' : (char)(( (__TIMESTAMP_YEAR_INT__    / 1000) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '9' : (char)(( (__TIMESTAMP_YEAR_INT__    /  100) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_YEAR_INT__    /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_YEAR_INT__    /    1) % 10 ) + '0'),
    '-',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MONTH_INT__   /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MONTH_INT__   /    1) % 10 ) + '0'),
    '-',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_DAY_INT__     /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_DAY_INT__     /    1) % 10 ) + '0'),
    'T',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_HOUR_INT__    /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_HOUR_INT__    /    1) % 10 ) + '0'),
    ':',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MINUTE_INT__  /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_MINUTE_INT__  /    1) % 10 ) + '0'),
    ':',
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_SECONDS_INT__ /   10) % 10 ) + '0'),
    __TIMESTAMP_FAILURE__ ? '0' : (char)(( (__TIMESTAMP_SECONDS_INT__ /    1) % 10 ) + '0'),
    '\0'
};
#endif // COMPILE_TIMESTAMP_H
