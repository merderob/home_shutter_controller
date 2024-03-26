// Copyright © 2023 -2024 Robert Takacs
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

/// @brief Struct containing the parameters for the RF communication.
struct RFParams
{
    /// @brief The high value of a "zero" bit when sending data. [us]
    static const int zero_high_send = 350;
    /// @brief The low value of a "zero" bit when sending data. [us]
    static const int zero_low_send = 700;
    /// @brief The high value of a "zero" bit when receiving data. [us]
    static const int zero_high_receive = 350;
    /// @brief The low value of a "zero" bit when receiving data. [us]
    static const int zero_low_receive = 700;

    /// @brief The high value of a "one" bit when sending data [us]
    static const int one_high_send = 630;
    /// @brief The low value of a "one" bit when sending data [us]
    static const int one_low_send = 300;
    /// @brief The low value of a "one" bit when receiving data [us]
    static const int one_high_receive = 630;
    /// @brief The low value of a "one" bit when receiving data [us]
    static const int one_low_receive = 400;
    
    /// @brief The high value of the synchronization pattern.
    static const int sync_on = 4700;
    /// @brief The low value of the synchronization pattern.
    static const int sync_off = 1500; 

    /// @brief  The delay between ttwo packets when sending data. [us]
    static const int delay_between_packets_send = 7400;
    /// @brief  The delay between ttwo packets when receiving [us]
    static const int delay_between_packets_receive = 7800;
    /// @brief The tolerance of the High-Low bit reception. Unused for now.
    static const int delay_tolerance = 200;
    /// @brief The number of transmitting the same command.
    static const int number_of_transmissions = 5;
};
