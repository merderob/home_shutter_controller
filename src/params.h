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

#pragma once

// Define the macro to disable transmission, and to enable printing to Serial.
//#define DEBUG

namespace params {
namespace shutter {

    const unsigned char bedroom_window_device_id = 0b000000001;
    constexpr double bedroom_window_time_up = 26.695;
    constexpr double bedroom_window_time_down = 26.1;

    const unsigned char bedroom_door_device_id = 0b00000010;
    constexpr double bedroom_door_time_up = 26.457;
    constexpr double bedroom_door_time_down = 25.06;

    const unsigned char living_window_device_id = 0b00000011;
    constexpr double living_room_window_time_up = 24.5;
    constexpr double living_room_window_time_down = 25.06;

    const unsigned char living_door_device_id = 0b00000100;
    constexpr double living_room_door_time_up = 26.1;
    constexpr double living_room_door_time_down = 24.76;

    const unsigned char all_device_id = 0b00000000;
    const unsigned char none_device_id = 0b00000101;

} // end namespace shutter

namespace clock {

    const long ntp_offset = 0;
    constexpr char ntp_address [] =  "hu.pool.ntp.org";
    const unsigned long ntp_interval_ms =  60 * 1000;

} // end namespace clock 

} // end namespace params
