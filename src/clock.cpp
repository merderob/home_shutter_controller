// Copyright © 2024 Robert Takacs
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "clock.h"

Clock::Clock()
{
    TimeChangeRule huDST = {"CEST", Second, Sun, Mar, 30, 120};  // Daylight time = UTC + 2 hrs
    TimeChangeRule huSTD = {"CET", First, Sun, Oct, 26, 60};   // Standard time = UTC + 1 hr
    time_zone_ = std::make_unique<Timezone>(huDST, huSTD);
    time_client_ = std::make_unique<NTPClient>(ntp_udp_, params::clock::ntp_address, params::clock::ntp_offset, params::clock::ntp_interval_ms);
}

void Clock::init()
{
    time_client_->begin();
}

void Clock::update()
{
    time_client_->update();
}

#ifdef DEBUG
void Clock::printDateTime() const
{
    time_t now = time_client_->getEpochTime();
    const auto t = time_zone_->toLocal(now);
    char buf[32];
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d",
        hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t));
    Serial.println(buf);
}
#endif
