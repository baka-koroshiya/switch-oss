/*
 * Copyright (C) 2011 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LocalizedDateCache_h
#define LocalizedDateCache_h

#include "DateComponents.h"
#include "FontCascade.h"
#include <wtf/HashMap.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/RetainPtr.h>

// FIXME: Rename this file to LocalizedDataCacheIOS.mm and remove this guard.
#if PLATFORM(IOS)

namespace WebCore {
    
class MeasureTextClient {
public:
    virtual float measureText(const String&) const = 0;
    virtual ~MeasureTextClient() { }
};

class LocalizedDateCache {
public:
    NSDateFormatter *formatterForDateType(DateComponents::Type);
    float maximumWidthForDateType(DateComponents::Type, const FontCascade&, const MeasureTextClient&);
    void localeChanged();

private:
    LocalizedDateCache();
    ~LocalizedDateCache();

    NSDateFormatter *createFormatterForType(DateComponents::Type);
    float calculateMaximumWidth(DateComponents::Type, const MeasureTextClient&);

    // Using int instead of DateComponents::Type for the key because the enum
    // does not have a default hash and hash traits. Usage of the maps
    // casts the DateComponents::Type into an int as the key.
    typedef HashMap<int, RetainPtr<NSDateFormatter>> DateTypeFormatterMap;
    typedef HashMap<int, float> DateTypeMaxWidthMap;
    DateTypeFormatterMap m_formatterMap;
    DateTypeMaxWidthMap m_maxWidthMap;
    FontCascade m_font;

    friend LocalizedDateCache& localizedDateCache();
    friend NeverDestroyed<LocalizedDateCache>;
};

// Singleton.
LocalizedDateCache& localizedDateCache();

} // namespace WebCore

#endif // PLATFORM(IOS)
#endif // LocalizedDateCache_h
