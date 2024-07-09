// Copyright © 2023 Robert Takacs
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
#include "shutter.h"
#include "transmitter.h"

#include "Arduino.h"
#include <array>

/// @brief Class encapsulating the shutter controller logic.
class ShutterController
{
public:
    /// @brief  Constructor.
    /// @param transmit_pin The transmit pin on the board.
    ShutterController(int transmit_pin);

    /// @brief Executes the main control loop.
    void execute();

    /// @brief Decodoes a command from the input string.
    /// @param command The command to decode.
    /// @return A shutter command representation of the input command.
    void createRelativeCommand(const String& command);

    /// @brief Decodes an absolute command based on the inputs.
    /// @param device_str The string representation of the commanded device.
    /// @param position_str The string representation of the absolute target position.
    /// @return A shutter command representation of the input command.
    void createAbsoluteCommand(const String& device_str, const String& position_str);

    /// @brief Decodes a calibration command.
    /// @param device_str The string representation of the commanded device.
    /// @return A shutter command representation of the input command.
    void createCalibrationCommand(const String& device_str);

private:
    /// @brief Container storing the shutters.
    std::array<Shutter, 4> shutters_; //
    /// @brief The transmitter.
    std::shared_ptr<Transmitter> transmitter_;
    int current_cmd_id_ = -1;
};
