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

#include "command.h"

Command::Command(int id, Type type):
    id_(id),
    type_(type),
    status_(Status::TO_BE_SENT)
{
}

Command::Type Command::getType() const
{
    return type_;
}

Command::Status Command::getStatus() const
{
    return status_;
}

Instruction Command::getInstruction() const
{
    return instruction_;
}

int Command::getId() const
{
    return id_;
}

int Command::getTargetPosition() const 
{
    return 0;
}

void Command::setStatus(Status status)
{
    status_ = status;
}

void Command::setInstruction(Instruction instruction)
{
    instruction_ = instruction;
}

void Command::setEndTime(int end_time_ms)
{
    end_time_ms_ = end_time_ms;
}

void Command::update(int now_ms)
{
    if (status_ == Status::EXECUTING)
    {
        if (now_ms >= end_time_ms_)
        {
            status_ = Status::DONE;
        }
    }
}

RelativeCommand::RelativeCommand(int id, Instruction instruction):
    Command(id, Type::RELATIVE)
{
    instruction_ = instruction;
}

AbsoluteCommand::AbsoluteCommand(int id, int target_position):
    Command(id, Type::ABSOLUTE),
    target_position_(target_position)
{
}

int AbsoluteCommand::getTargetPosition() const
{
    return target_position_;
}

CalibrationCommand::CalibrationCommand(int id):
    Command(id, Type::CALIBRATE)
{
    instruction_ = Instruction::UP;
}
