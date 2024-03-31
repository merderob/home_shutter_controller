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

#include "transmitter.h"
#include <Arduino.h>

Transmitter::Transmitter(int transmit_pin): transmit_pin_(transmit_pin)
{
    //GPIO 1 (TX) swap the pin to a GPIO.
    // TODO redo thiss
    pinMode(transmit_pin_, FUNCTION_3); 
    // Initialize the output variables as outputs
    pinMode(transmit_pin_, OUTPUT);
    commands_[Shutter::Command::UP] = 0b00010001;
    commands_[Shutter::Command::DOWN] = 0b00110011;
    commands_[Shutter::Command::STOP] = 0b001010101;
}

void Transmitter::sendWord(unsigned char command) 
{
    for (unsigned char k = 0; k < 8; k++) 
    {
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

void Transmitter::sendSync()
{
    digitalWrite(transmit_pin_, HIGH);
    delayMicroseconds(params_.sync_on);
    digitalWrite(transmit_pin_, LOW);
    delayMicroseconds(params_.sync_off);
}

void Transmitter::sendHeader()
{
    for (auto header_char : header_)
    {
        sendWord(header_char);
    }
}

void Transmitter::sendCommand(unsigned char device_id, Shutter::Command command)
{
    for (int transmission_num = 0; transmission_num < params_.number_of_transmissions; ++transmission_num)
    {
        sendSync();
        sendHeader();
        sendWord(device_id);
        sendWord(commands_[command]);
        delayMicroseconds(params_.delay_between_packets_send);  
    }
}


