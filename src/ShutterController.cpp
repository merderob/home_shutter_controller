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

#include "ShutterController.h"

// _______  HEAD ___ HEAD ___ HEAD __ SELECT__ DIR __
// stop 4 11001011 01111010 01010001 00000100 01010101 
// up4    11001011 01111010 01010001 00000100 00010001
// down4  11001011 01111010 01010001 00000100 00110011

// stop 3 11001011 01111010 01010001 00000011 01010101 
// up3    11001011 01111010 01010001 00000011 00010001

// stop2  11001011 01111010 01010001 00000010 01010101 

// stop1  11001011 01111010 01010001 01010001 01010101 

// stopA  11001011 01111010 01010001 00000000 01010101

const unsigned char HEADER [] = {0b11001011, 0b01111010, 0b01010001};
const unsigned char COMMAND_PALETTE [] = {0b00010001, 0b00110011, 0b001010101};
const unsigned char DEVICE_PALETTE [] =  {0b000000001, 0b00000010, 0b00000011, 0b00000100, 0b00000000};
const char HEADER_LENGTH = 3;

ShutterController::ShutterController(int transmit_pin):
    transmit_pin_(transmit_pin)
{
}

ShutterCommand ShutterController::decodeCommand(String command)
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

ShutterCommand ShutterController::decodeAbsoluteCommand (String device_str, String position_str)
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
  if (command.device == Shutter::Device::UNKNOWN_DEVICE || command.type == Shutter::CommandType::UNKNOWN
      || command.device == Shutter::Device::ALL // Unsupported for now. TODO!
  )
  {
      return;
  }

  if (command.type == Shutter::CommandType::NORMAL)
  {
      sendCommand(command.device, command.command);
  }
  else if (command.type == Shutter::CommandType::ABSOLUTE)
  {
      sendAbsoluteCommand(command.device, command.position);
  }
}

void ShutterController::addCommand(const ShutterCommand& command)
{
    command_queue_.push_back(command);
}

void ShutterController::sendCommand(Shutter::Device device, Shutter::Command command)
{
    for (int transmission_num = 0; transmission_num < params_.number_of_transmissions; ++transmission_num)
    {
        digitalWrite(transmit_pin_, HIGH);
        delayMicroseconds(params_.sync_on);
        digitalWrite(transmit_pin_, LOW);
        delayMicroseconds(params_.sync_off);
        for (int header_id = 0; header_id < HEADER_LENGTH; ++header_id)
        {
            sendWord(HEADER[header_id]);
        }
        sendWord(DEVICE_PALETTE[device]);
        sendWord(COMMAND_PALETTE[command]);
        delayMicroseconds(params_.delay_between_packets_send);  
    }
}

void ShutterController::sendAbsoluteCommand(Shutter::Device device, int position)
{
  if(!shutter_calibrations_[device])
  {
      calibrate(device);
  }
  int delta_p = position - shutter_positions_[device];
  // 100 incr ... 25 s
  int dt_wait_ms = static_cast<int>((static_cast<double>(std::abs(delta_p)) / 100.0) * 25.0) * 1000;
  if (delta_p > 0)
  {
      // Shutter should move down
      sendCommand(device, Shutter::Command::DOWN);
      delay(dt_wait_ms);
  }
  else //  (delta_p < 0)
  {
    // Shutter should move up
    sendCommand(device, Shutter::Command::UP);
    delay(dt_wait_ms);
  }
  sendCommand(device, Shutter::Command::STOP);
  shutter_positions_[device] = position;
}

void ShutterController::calibrate(Shutter::Device device)
{
    sendCommand(device, Shutter::Command::UP);
    delay(25000);
    shutter_calibrations_[device] = true;
    shutter_positions_[device] = 0;
}


void ShutterController::sendWord(unsigned char command) 
{
  for (unsigned char k = 0; k < 8; k++) {
    if ((command >> (7 - k)) & 1 )
    {
      // one 
      digitalWrite(transmit_pin_, HIGH);
      delayMicroseconds(params_.one_high_send);
      digitalWrite(transmit_pin_, LOW);    
      delayMicroseconds(params_.one_low_send);
    }
    else
    {
      // Zero
      digitalWrite(transmit_pin_, HIGH);
      delayMicroseconds(params_.zero_high_send);
      digitalWrite(transmit_pin_, LOW);    
      delayMicroseconds(params_.zero_low_send);
    }
  }
}
