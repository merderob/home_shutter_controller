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

/// @brief Class encapsulating a command instance.
class Command
{
public:
    /// @brief Enum for the command type.
    enum Type
    {
        RELATIVE,
        ABSOLUTE,
        CALIBRATE,
        UNKNOWN
    };

    /// @brief Enum for a command status.
    enum Status
    {
        TO_BE_SENT,
        EXECUTING,
        DONE
    };

    /// @brief Constructor.
    /// @param id The command identifier, shared across all shutters.
    /// @param type The command type.
    Command (int id, Type type);

    /// @brief Gets the command type.
    /// @return The command type.
    Type getType() const;
    /// @brief Gets the command status.
    /// @return The command status.
    Status getStatus() const;
    /// @brief Gets the command's instruction.
    /// @return The command's instruction.
    Instruction getInstruction() const;
    /// @brief Returns the command's identifier.
    /// @return The command's identifier.
    int getId() const;
    /// @brief Gets the command's target position. Only used in Absolute command (because of the lack of RTTI).
    /// @return The command's target position, 0 (top target position) if not an Absolute command.
    virtual int getTargetPosition() const;

    void setStatus(Status status);
    void setInstruction(Instruction instruction);
    void setEndTime(int end_time_ms);

    /// @brief Executes the command's update cycle (sets the status to DONE if necessary).
    /// @param now_ms The execution time in ms.
    void update(int now_ms);

protected:
    /// @brief The command's instruction.
    Instruction instruction_ = Instruction::UNKNOWN;

private:
    /// @brief The command's identifier, shared across all shutters.
    int id_;
    /// @brief The command's type.
    Type type_;
    /// @brief The command's status.
    Status status_;
    /// @brief The end time of the command, as ms.
    int end_time_ms_;
};

/// @brief Class for a relative command.
class RelativeCommand : public Command
{
public:
    /// @brief Constructor.
    /// @param id The command identifier.
    /// @param instruction The instruction of the relative command. 
    RelativeCommand(int id, Instruction instruction);
};

/// @brief Class for an absolute command.
class AbsoluteCommand : public Command
{
public:
    /// @brief Constructor.
    /// @param id The command identifier.
    /// @param target_position The target position.
    AbsoluteCommand(int id, int target_position);
    
    /// @brief Returns the target position of the command.
    /// @return The target position.
    int getTargetPosition() const override;

private:
    /// @brief The absolute target position to command (0 = top, 100 = bottom).
    int target_position_ = 0;
};

/// @brief Class for a calibration command.
class CalibrationCommand : public Command
{
public:
    /// @brief Constructor.
    /// @param id The command identifier.
    CalibrationCommand(int id);
};
