#!/usr/bin/env python
#
# Copyright (C) 2014 Apple Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.

import os
import sys


def enumerablePseudoType(stringPseudoType):
    output = ['CSSSelector::PseudoClass']

    if stringPseudoType.endswith('('):
        stringPseudoType = stringPseudoType[:-1]

    webkitPrefix = '-webkit-'
    if (stringPseudoType.startswith(webkitPrefix)):
        stringPseudoType = stringPseudoType[len(webkitPrefix):]

    khtmlPrefix = '-khtml-'
    if (stringPseudoType.startswith(khtmlPrefix)):
        stringPseudoType = stringPseudoType[len(khtmlPrefix):]

    substring_start = 0
    next_dash_position = stringPseudoType.find('-')
    while (next_dash_position != -1):
        output.append(stringPseudoType[substring_start].upper())
        output.append(stringPseudoType[substring_start + 1:next_dash_position])
        substring_start = next_dash_position + 1
        next_dash_position = stringPseudoType.find('-', substring_start)

    output.append(stringPseudoType[substring_start].upper())
    output.append(stringPseudoType[substring_start + 1:])
    return ''.join(output)


def expand_ifdef_condition(condition):
    return condition.replace('(', '_').replace(')', '=1')

output_file = open('SelectorPseudoClassAndCompatibilityElementMap.gperf', 'w')

output_file.write("""
%{
/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

// This file is automatically generated from SelectorPseudoTypeMap.in by makeprop, do not edit by hand.

#include "config.h"
#include "SelectorPseudoTypeMap.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wdeprecated-register"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#endif

namespace WebCore {

struct SelectorPseudoClassOrCompatibilityPseudoElementEntry {
    const char* name;
    PseudoClassOrCompatibilityPseudoElement pseudoTypes;
};

%}
%struct-type
%define initializer-suffix ,{CSSSelector::PseudoClassUnknown,CSSSelector::PseudoElementUnknown}
%define class-name SelectorPseudoClassAndCompatibilityElementMapHash
%omit-struct-type
%language=C++
%readonly-tables
%global-table
%ignore-case
%compare-strncmp
%enum

struct SelectorPseudoClassOrCompatibilityPseudoElementEntry;

%%
""")

webcore_defines = [i.strip() for i in sys.argv[-1].split(' ')]

longest_keyword = 0

ignore_until_endif = False
input_file = open(sys.argv[1], 'r')
for line in input_file:
    line = line.strip()
    if not line:
        continue

    if line.startswith('#if '):
        condition = line[4:].strip()
        if expand_ifdef_condition(condition) not in webcore_defines:
            ignore_until_endif = True
        continue

    if line.startswith('#endif'):
        ignore_until_endif = False
        continue

    if ignore_until_endif:
        continue

    keyword_definition = line.split(',')
    if len(keyword_definition) == 1:
        keyword = keyword_definition[0].strip()
        output_file.write('"%s", {%s, CSSSelector::PseudoElementUnknown}\n' % (keyword, enumerablePseudoType(keyword)))
    else:
        output_file.write('"%s", {CSSSelector::%s, CSSSelector::%s}\n' % (keyword_definition[0].strip(), keyword_definition[1].strip(), keyword_definition[2].strip()))

    longest_keyword = max(longest_keyword, len(keyword))

output_file.write("""%%

static inline const SelectorPseudoClassOrCompatibilityPseudoElementEntry* parsePseudoClassAndCompatibilityElementString(const LChar* characters, unsigned length)
{
    return SelectorPseudoClassAndCompatibilityElementMapHash::in_word_set(reinterpret_cast<const char*>(characters), length);
}""")

output_file.write("""

static inline const SelectorPseudoClassOrCompatibilityPseudoElementEntry* parsePseudoClassAndCompatibilityElementString(const UChar* characters, unsigned length)
{
    const unsigned maxKeywordLength = %s;
    LChar buffer[maxKeywordLength];
    if (length > maxKeywordLength)
        return nullptr;

    for (unsigned i = 0; i < length; ++i) {
        UChar character = characters[i];
        if (character & ~0xff)
            return nullptr;

        buffer[i] = static_cast<LChar>(character);
    }
    return parsePseudoClassAndCompatibilityElementString(buffer, length);
}
""" % longest_keyword)

output_file.write("""
PseudoClassOrCompatibilityPseudoElement parsePseudoClassAndCompatibilityElementString(const CSSParserString& pseudoTypeString)
{
    const SelectorPseudoClassOrCompatibilityPseudoElementEntry* entry;
    if (pseudoTypeString.is8Bit())
        entry = parsePseudoClassAndCompatibilityElementString(pseudoTypeString.characters8(), pseudoTypeString.length());
    else
        entry = parsePseudoClassAndCompatibilityElementString(pseudoTypeString.characters16(), pseudoTypeString.length());

    if (entry)
        return entry->pseudoTypes;
#if !PLATFORM(WKC)
    return { CSSSelector::PseudoClassUnknown, CSSSelector::PseudoElementUnknown };
#else
    PseudoClassOrCompatibilityPseudoElement ret = { CSSSelector::PseudoClassUnknown, CSSSelector::PseudoElementUnknown };
    return ret;
#endif
}

} // namespace WebCore

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

""")
output_file.close()

gperf_command = 'gperf'
if 'GPERF' in os.environ:
    gperf_command = os.environ['GPERF']

gperf_return = os.system("%s --key-positions=\"*\" -m 10 -s 2 SelectorPseudoClassAndCompatibilityElementMap.gperf --output-file=SelectorPseudoClassAndCompatibilityElementMap.cpp" % gperf_command)
if gperf_return != 0:
    print("Error when generating SelectorPseudoClassAndCompatibilityElementMap.cpp from SelectorPseudoClassAndCompatibilityElementMap.gperf :(")
    sys.exit(gperf_return)
