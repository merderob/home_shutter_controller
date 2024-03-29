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
#include "rf_params.h"
#include "Arduino.h"
#include <deque>
#include <array>

// _______  HEAD ___ HEAD ___ HEAD __ SELECT__ DIR __
// stop 4 11001011 01111010 01010001 00000100 01010101 
// up4    11001011 01111010 01010001 00000100 00010001
// down4  11001011 01111010 01010001 00000100 00110011

// stop 3 11001011 01111010 01010001 00000011 01010101 
// up3    11001011 01111010 01010001 00000011 00010001

// stop2  11001011 01111010 01010001 00000010 01010101 

// stop1  11001011 01111010 01010001 01010001 01010101 

// stopA  11001011 01111010 01010001 00000000 01010101

class Shutter
{
public:
    enum Device
    {
        BEDROOM_WINDOW,
        BEDROOM_DOOR,
        LIVING_WINDOW,
        LIVING_DOOR,
        ALL,
        UNKNOWN_DEVICE
    };

    enum Command
    {
        UP,
        DOWN,
        STOP,
        UNKNOWN_COMMAND
    };

    enum CommandType
    {
        NORMAL,
        ABSOLUTE,
        UNKNOWN
    };
};

/// @brief Shorthand structure for a shutter command.
struct ShutterCommand
{
    /// @brief The command type.
    Shutter::CommandType type = Shutter::CommandType::UNKNOWN;
    /// @brief The commanded device.
    Shutter::Device device  = Shutter::Device::UNKNOWN_DEVICE; 
    /// @brief The commanded direction (in case of a relative command).
    Shutter::Command command  = Shutter::Command::UNKNOWN_COMMAND; 
    /// @brief The absolute position to command.
    int position = 0;
};

/// @brief Class encapsulating the shutter controller logic.
class ShutterController
{
public:
    /// @brief  Constructor.
    /// @param transmit_pin The transmit pin on the board.
    ShutterController(int transmit_pin);
    /// @brief Executes the main control loop.
    void execute();
    /// @brief Adds a command to the command queue.
    /// @param command Reference to the added command.
    void addCommand(const ShutterCommand& command);
    /// @brief Decodoes a command from the input string.
    /// @param command The command to decode.
    /// @return A shutter command representation of the input command.
    ShutterCommand decodeCommand(String command);
    /// @brief Decodes an absolute command based on the inputs.
    /// @param device_str The string representation of the commanded device.
    /// @param position_str The string representation of the absolute target position.
    /// @return A shutter command representation of the input command.
    ShutterCommand decodeAbsoluteCommand(String device_str, String position_str);
    void sendAbsoluteCommand(Shutter::Device device, int position);
private:
    /// @brief The inner command sending function.
    /// @param device The commanded device.
    /// @param command The command sent.
    void sendCommand(Shutter::Device device, Shutter::Command command);
    /// @brief Processes a command.
    /// @param command Reference to the commamnd instance.
    void processCommand(const ShutterCommand& command);
    /// @brief Calibrates a device.
    /// @param device The device to calibrate. 
    void calibrate(Shutter::Device device);
    /// @brief Sends a command over the transmit pin.
    /// @param command The command to send.
    void sendWord(unsigned char command);

    /// @brief The transmit pin on the board.
    int transmit_pin_;
    /// @brief The command queue.
    std::deque<ShutterCommand> command_queue_;
    /// @brief Container storing if the shutters has been calibrated.
    std::array<bool, 4> shutter_calibrations_ {false, false, false, false};
    /// @brief Container storing the absolute positions of the shutters.
    std::array<int, 4> shutter_positions_ {0, 0, 0, 0}; // 0: up, 100: down [dont use 100 pls]
    /// @brief parameter container structure.
    RFParams params_;
};
