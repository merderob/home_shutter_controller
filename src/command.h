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

#include "instruction.h"
#include <array>

class Command
{
public:
    enum Type
    {
        RELATIVE,
        ABSOLUTE,
        CALIBRATE,
        UNKNOWN
    };

    enum Status
    {
        TO_BE_SENT,
        EXECUTING,
        DONE
    };

    Command (int id, Type type);

    Type getType() const;
    Status getStatus() const;
    Instruction getInstruction() const;
    int getId() const;
    virtual int getTargetPosition() const;

    void setStatus(Status status);
    void setInstruction(Instruction instruction);
    void setEndTime(int end_time_ms);

    virtual void update(int now_ms);

protected:
    Instruction instruction_ = Instruction::UNKNOWN;

private:
    int id_;
    Type type_;
    Status status_;
    int end_time_ms_;

};

class RelativeCommand : public Command
{
public:
    RelativeCommand(int id, Instruction direction);
};

class AbsoluteCommand : public Command
{
public:
    AbsoluteCommand(int id, int target_position);
    int getTargetPosition() const override;
private:
    /// @brief The absolute target position to command.
    int target_position_ = 0;
    std::array<Command*, 2> commands_;
};

class CalibrationCommand : public Command
{
public:
    CalibrationCommand(int id);
};
