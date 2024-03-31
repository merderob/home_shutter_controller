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

#include "shutter.h"
#include "shutter_params.h"

Shutter::Shutter() : device_id_(ShutterParams::none_device_id), position_(0), calibrated_(false), time_up_(0), time_down_(0)
{

}

Shutter::Shutter(unsigned char id, double time_up, double time_down): 
    device_id_(id), position_(0), time_up_(time_up), time_down_(time_down)
{
}

bool Shutter::calibrated() const
{
    return calibrated_;
}

void Shutter::setCalibrated(bool calibrated)
{
    calibrated_ = calibrated;
}

int Shutter::getPosition() const
{
    return position_;
}

void Shutter::setPosition(int position)
{
    position_ = position;
}

unsigned char Shutter::getId() const
{
    return device_id_;
}


double Shutter::getTimeUp() const
{
    return time_up_;
}

double Shutter::getTimeDown() const
{
    return time_down_;
}
