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

#include <array>

#include "rf_params.h"
#include "instruction.h"

/// @brief Class acting as a transmitter instance.
class Transmitter
{
public:
    /// @brief Constructor.
    /// @param transmit_pin The transmit pin.
    Transmitter (int transmit_pin);

    /// @param device_id The commanded device's id.
    /// @param instruction The command sent.
    /// @return True, if the command was successfully sent.
    bool sendCommand(unsigned char device_id, Instruction instruction);
private:
    /// @brief Sends a command over the transmit pin.
    /// @param command The command to send.
    void sendWord(unsigned char command);
    /// @brief Sends the synchronization signal before the transmission.
    void sendSync();
    /// @brief Sends the message headers.
    void sendHeader();

    /// @brief The transmit pin on the board.
    int transmit_pin_;
    /// @brief parameter container structure.
    RFParams params_;

    const std::array<char, 3> header_ {0b11001011, 0b01111010, 0b01010001};
    std::array<char, 3> instructions_;
};
