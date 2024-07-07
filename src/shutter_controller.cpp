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

ShutterController::ShutterController(int transmit_pin):
    transmitter_(transmit_pin)
{
    shutters_[Shutter::Device::LIVING_DOOR] = 
      Shutter(
        ShutterParams::living_door_device_id,
        ShutterParams::living_room_door_time_up, 
        ShutterParams::living_room_door_time_down);
    shutters_[Shutter::Device::LIVING_WINDOW] = 
      Shutter(
        ShutterParams::living_window_device_id,
        ShutterParams::living_room_window_time_up, 
        ShutterParams::living_room_window_time_down);
    shutters_[Shutter::Device::BEDROOM_DOOR] = 
      Shutter(
        ShutterParams::bedroom_door_device_id,
        ShutterParams::bedroom_door_time_up, 
        ShutterParams::bedroom_door_time_down);
    shutters_[Shutter::Device::BEDROOM_WINDOW] = 
      Shutter(
        ShutterParams::bedroom_window_device_id,
        ShutterParams::bedroom_window_time_up, 
        ShutterParams::bedroom_window_time_down);
}

ShutterCommand ShutterController::decodeCommand(const String& command)
{
    ShutterCommand decoded_command;
    // A command has the following format: "3,up"
    if (command[1] != ',')
    {
        return decoded_command;
    }

    decoded_command.type = Shutter::CommandType::NORMAL;
    char device_char = command[0];
    char dir_char = command[2];

    switch (device_char)
    {
        case '0':
            decoded_command.device = Shutter::Device::BEDROOM_WINDOW;
            break;
        case '1':
            decoded_command.device = Shutter::Device::BEDROOM_DOOR;
            break;
        case '2':
            decoded_command.device = Shutter::Device::LIVING_WINDOW;
            break;
        case '3':
            decoded_command.device = Shutter::Device::LIVING_DOOR;
            break;
    }

    switch (dir_char)
    {
        case 'u':
            decoded_command.command = Shutter::Command::UP;
            break;
        case 's':
            decoded_command.command = Shutter::Command::STOP;
            break;
        case 'd':
            decoded_command.command = Shutter::Command::DOWN;
            break;
    }

    return decoded_command;
}

ShutterCommand ShutterController::decodeAbsoluteCommand (const String& device_str, const String& position_str)
{
    ShutterCommand decoded_command;
    decoded_command.type = Shutter::CommandType::ABSOLUTE;
    // Select device
    if (device_str == "living_room_door")
    {
        decoded_command.device = Shutter::Device::LIVING_DOOR;
    }
    else if (device_str == "living_room_window")
    {
        decoded_command.device = Shutter::Device::LIVING_WINDOW;
    }
    else if (device_str == "bedroom_door")
    {
        decoded_command.device = Shutter::Device::BEDROOM_DOOR;
    }
    else if (device_str == "bedroom_window")
    {
        decoded_command.device = Shutter::Device::BEDROOM_WINDOW;
    }

    const int received_position = position_str.toInt();
    const int decoded_position = std::max(0, std::min(received_position, 100));
    decoded_command.position = decoded_position;
    return decoded_command;
}

ShutterCommand ShutterController::decodeCalibrationCommand(const String& device_str)
{
    ShutterCommand ret;
    ret.type = Shutter::CommandType::CALIBRATE;
    if (device_str == "0")
    {
        ret.device = Shutter::Device::LIVING_DOOR;
    }
    else if (device_str == "1")
    {
        ret.device = Shutter::Device::LIVING_WINDOW;
    }
    else if (device_str == "2")
    {
        ret.device = Shutter::Device::LIVING_WINDOW;
    }
        else if (device_str == "3")
    {
        ret.device = Shutter::Device::LIVING_WINDOW;
    }
    else
    {
        ret.device = Shutter::Device::UNKNOWN_DEVICE;
    }
    return ret;
}

void ShutterController::execute()
{
    if (!command_queue_.empty())
    {
        const ShutterCommand& current_command = command_queue_.front();
        processCommand(current_command);
        command_queue_.pop_front();
    }
}

void ShutterController::processCommand(const ShutterCommand& command)
{
    if (command.device == Shutter::Device::UNKNOWN_DEVICE || 
        command.type == Shutter::CommandType::UNKNOWN || 
        command.device == Shutter::Device::ALL) // Unsupported for now. TODO!
    {
        return;
    }

    auto& shutter = shutters_[command.device];
    if (command.type == Shutter::CommandType::NORMAL)
    {
        sendNormalCommand(shutter, command.command);
    }
    else if (command.type == Shutter::CommandType::ABSOLUTE)
    {
        sendAbsoluteCommand(shutter, command.position);
    }
    else if (command.type == Shutter::CommandType::CALIBRATE)
    {
        calibrate(shutter);
    }
}

void ShutterController::addCommand(const ShutterCommand& command)
{
    command_queue_.push_back(command);
}

void ShutterController::sendNormalCommand(Shutter& shutter, Shutter::Command command)
{
    transmitter_.sendCommand(shutter.getId(), command);
}

void ShutterController::sendAbsoluteCommand(Shutter& shutter, int position)
{
    if (!shutter.calibrated())
    {
        calibrate(shutter);
    }
    int delta_p = position - shutter.getPosition();
    // 100 incr ... 25 s
    if (delta_p > 0)
    {
        // Shutter should move down
        const auto dt_down = std::round((static_cast<double>(std::abs(delta_p)) / 100.0) * shutter.getTimeDown() * 1000.0);
        transmitter_.sendCommand(shutter.getId(), Shutter::Command::DOWN);
        delay(dt_down);
    }
    else //  (delta_p < 0)
    {
        // Shutter should move up
        const auto dt_up = std::round((static_cast<double>(std::abs(delta_p)) / 100.0) * shutter.getTimeUp() * 1000.0);
        transmitter_.sendCommand(shutter.getId(), Shutter::Command::UP);
        delay(dt_up);
    }
    transmitter_.sendCommand(shutter.getId(), Shutter::Command::STOP);
    shutter.setPosition(position);
}

void ShutterController::calibrate(Shutter& shutter)
{
    transmitter_.sendCommand(shutter.getId(), Shutter::Command::UP);
    const auto time_up_ms = static_cast<unsigned long>(shutter.getTimeUp() * 1000.0);
    delay(time_up_ms);
    shutter.setPosition(0);
    shutter.setCalibrated(true);
}
