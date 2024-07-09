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

#include "shutter_controller.h"
#include "shutter_params.h"

ShutterController::ShutterController(int transmit_pin)
{
    transmitter_ = std::make_shared<Transmitter>(transmit_pin);
    shutters_[Shutter::Device::LIVING_DOOR] = 
      Shutter(
        transmitter_,
        ShutterParams::living_door_device_id,
        ShutterParams::living_room_door_time_up, 
        ShutterParams::living_room_door_time_down);
    shutters_[Shutter::Device::LIVING_WINDOW] = 
      Shutter(
        transmitter_,
        ShutterParams::living_window_device_id,
        ShutterParams::living_room_window_time_up, 
        ShutterParams::living_room_window_time_down);
    shutters_[Shutter::Device::BEDROOM_DOOR] = 
      Shutter(
        transmitter_,
        ShutterParams::bedroom_door_device_id,
        ShutterParams::bedroom_door_time_up, 
        ShutterParams::bedroom_door_time_down);
    shutters_[Shutter::Device::BEDROOM_WINDOW] = 
      Shutter(
        transmitter_,
        ShutterParams::bedroom_window_device_id,
        ShutterParams::bedroom_window_time_up, 
        ShutterParams::bedroom_window_time_down);
}

void ShutterController::createRelativeCommand(const String& command)
{
    // A command has the following format: "3,up"
    if (command[1] != ',')
    {
        return;
    }

    Shutter::Device device = Shutter::Device::UNKNOWN_DEVICE;
    char device_char = command[0];
    switch (device_char)
    {
        case '0':
            device = Shutter::Device::BEDROOM_WINDOW;
            break;
        case '1':
            device = Shutter::Device::BEDROOM_DOOR;
            break;
        case '2':
            device = Shutter::Device::LIVING_WINDOW;
            break;
        case '3':
            device = Shutter::Device::LIVING_DOOR;
            break;
        default:
            return;
    }

    Instruction instruction = Instruction::UNKNOWN;
    char dir_char = command[2];
    switch (dir_char)
    {
        case 'u':
            instruction = Instruction::UP;
            break;
        case 's':
            instruction = Instruction::STOP;
            break;
        case 'd':
            instruction = Instruction::DOWN;
            break;
        default:
            return;
    }
    if (instruction == Instruction::STOP)
    {
        shutters_[device].clearQueue();
    }
    shutters_[device].addCommand(std::make_unique<RelativeCommand>(++current_cmd_id_, instruction));
}

void ShutterController::createAbsoluteCommand(const String& device_str, const String& position_str)
{
    Shutter::Device device = Shutter::Device::UNKNOWN_DEVICE;
    if (device_str == "living_room_door")
    {
        device = Shutter::Device::LIVING_DOOR;
    }
    else if (device_str == "living_room_window")
    {
        device = Shutter::Device::LIVING_WINDOW;
    }
    else if (device_str == "bedroom_door")
    {
        device = Shutter::Device::BEDROOM_DOOR;
    }
    else if (device_str == "bedroom_window")
    {
        device = Shutter::Device::BEDROOM_WINDOW;
    }
    else
    {
        return;
    }

    if (!shutters_[device].calibrated())
    {
        shutters_[device].addCommand(std::make_unique<CalibrationCommand>(++current_cmd_id_));
    }

    const int received_position = position_str.toInt();
    const int target_position = std::max(0, std::min(received_position, 100));
    shutters_[device].addCommand(std::make_unique<AbsoluteCommand>(++current_cmd_id_, target_position));
}

void ShutterController::createCalibrationCommand(const String& device_str)
{
    Shutter::Device device = Shutter::Device::UNKNOWN_DEVICE;
    if (device_str == "0")
    {
        device = Shutter::Device::LIVING_DOOR;
    }
    else if (device_str == "1")
    {
        device = Shutter::Device::LIVING_WINDOW;
    }
    else if (device_str == "2")
    {
        device = Shutter::Device::LIVING_WINDOW;
    }
        else if (device_str == "3")
    {
        device = Shutter::Device::LIVING_WINDOW;
    }
    else
    {
        return;
    }
    shutters_[device].addCommand(std::make_unique<CalibrationCommand>(++current_cmd_id_));
}

void ShutterController::execute()
{
    for (auto& shutter: shutters_)
    {
        shutter.execute();
    }
}
