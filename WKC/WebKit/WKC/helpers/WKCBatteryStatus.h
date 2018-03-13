/*
 * Copyright (c) 2013-2014 ACCESS CO., LTD. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef _WKC_HELPERS_WKC_BATTERYSTATUS_H_
#define _WKC_HELPERS_WKC_BATTERYSTATUS_H_

namespace WKC {

class BatteryStatusPrivate;

class WKC_API BatteryStatus {
public:
    BatteryStatus(bool charging, double chargingTime, double dischargingTime, double level);
    ~BatteryStatus();

    friend class BatteryControllerPrivate;

private:
    // Heap allocation by operator new/delete is disallowed.
    // This restriction is to avoid memory leaks or crashes.
    void* operator new(size_t);
    void* operator new[](size_t);
    void operator delete(void*);
    void operator delete[](void*);

    bool m_charging;
    double m_chargingTime;
    double m_dischargingTime;
    double m_level;
};

} // namespace

#endif // _WKC_HELPERS_WKC_BATTERYSTATUS_H_
