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

#ifndef GamepadProvider_h
#define GamepadProvider_h

#if ENABLE(GAMEPAD)

#include <wtf/HashSet.h>
#include <wtf/Vector.h>

namespace WebCore {

class GamepadProviderClient;
class PlatformGamepad;

class WEBCORE_EXPORT GamepadProvider {
#if PLATFORM(WKC)
    WTF_MAKE_FAST_ALLOCATED;
#endif
public:
    virtual ~GamepadProvider() { }

    WEBCORE_EXPORT static GamepadProvider& singleton();
    WEBCORE_EXPORT static void setSharedProvider(GamepadProvider&);

    virtual void startMonitoringGamepads(GamepadProviderClient*);
    virtual void stopMonitoringGamepads(GamepadProviderClient*);
    virtual const Vector<PlatformGamepad*>& platformGamepads();

protected:
    void dispatchPlatformGamepadInputActivity();
    void setShouldMakeGamepadsVisibile() { m_shouldMakeGamepadsVisible = true; }
    HashSet<GamepadProviderClient*> m_clients;

private:
    bool m_shouldMakeGamepadsVisible { false };
};

} // namespace WebCore

#endif // ENABLE(GAMEPAD)
#endif // GamepadProvider_h
