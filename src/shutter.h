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
#include "command.h"
#include "transmitter.h"
#include <memory>
#include <deque>

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
    Shutter();

    /// @brief  Constructor.
    /// @param time_up Time required to move up.
    /// @param time_down Time required to move down.
    Shutter(std::shared_ptr<Transmitter> transmitter, unsigned char id, double time_up, double time_down);

    bool calibrated() const;

    void addCommand(std::unique_ptr<Command> command);

    void execute();

    void clearQueue();

private:
    void executeSend(const std::unique_ptr<Command>& command);

    void executeDone(const std::unique_ptr<Command>& command);

    /// @brief The device id.
    unsigned char device_id_ = 0b000000111;
    /// @brief The current position (0: up, 100: down). 
    int position_ = 0;
    /// @brief Stores if the position is valid (the shutter is calibrated).
    bool calibrated_ = false; 
    /// @brief Time required to move up [s].
    double time_up_ = 0.0;
    /// @brief Time required to move down. [s]
    double time_down_ = 0.0;

    std::deque<std::unique_ptr<Command>> commands_;
    std::shared_ptr<Transmitter> transmitter_;
};
