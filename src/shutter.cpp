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

#include <Arduino.h>

#include "shutter.h"
#include "shutter_params.h"

Shutter::Shutter() : 
    device_id_(ShutterParams::none_device_id), position_(0), calibrated_(false), time_up_(0), time_down_(0)
{

}

Shutter::Shutter(std::shared_ptr<Transmitter> Transmitter, unsigned char id, double time_up, double time_down): 
    device_id_(id), position_(0), time_up_(time_up), time_down_(time_down)
{
}

bool Shutter::calibrated() const
{
    return calibrated_;
}

void Shutter::addCommand(std::unique_ptr<Command> command)
{
    commands_.push_back(std::move(command));
}

void Shutter::executeSend(const std::unique_ptr<Command>& command)
{
    const auto sent = transmitter_->sendCommand(device_id_, command->getInstruction());
    const auto maybe_sent_at = millis();
    switch (command->getType())
    {
    case Command::Type::RELATIVE:
    {
        if (!sent)
        {
            break;
        }

        if (command->getInstruction() == Instruction::STOP)
        {
            command->setEndTime(maybe_sent_at);
        }
        else if (command->getInstruction() == Instruction::DOWN)
        {
            const auto time_down_ms = static_cast<int>(time_down_ * 1000.0);
            command->setEndTime(maybe_sent_at + time_down_ms);
        }
        else // UP and else
        {
            const auto time_up_ms = static_cast<int>(time_up_ * 1000.0);
            command->setEndTime(maybe_sent_at + time_up_ms);
        }
        break;
    }
    case Command::Type::CALIBRATE:
    {            
        if (!sent)
        {
            break;
        }

        const auto time_up_ms = static_cast<int>(time_up_ * 1000.0);
        command->setEndTime(maybe_sent_at + time_up_ms);
        break;
    }
        break;
    case Command::Type::ABSOLUTE:
    {
        int delta_p = command->getTargetPosition() - position_;
        int dt_ms = 0;
        // 100 incr ... 25 s
        if (delta_p > 0)
        {
            // Shutter should move down
            dt_ms = std::round((static_cast<double>(std::abs(delta_p)) / 100.0) * time_down_ * 1000.0);
        }
        else //  (delta_p < 0)
        {
            // Shutter should move up
            dt_ms = std::round((static_cast<double>(std::abs(delta_p)) / 100.0) * time_up_ * 1000.0);
        }
        if (sent)
        {        
            command->setEndTime(maybe_sent_at + dt_ms);
            commands_.push_back(std::make_unique<RelativeCommand>(command->getId(), Instruction::STOP));
        }
        else // ! sent
        {
            Instruction instruction = delta_p > 0 ? Instruction::DOWN : Instruction::UP;
            command->setInstruction(instruction);
        }
        break;
    }
    default:
        break;
    }

    if (sent)
    {
        command->setStatus(Command::Status::EXECUTING);
    }
}

void Shutter::executeDone(const std::unique_ptr<Command>& command)
{
    switch (command->getType())
    {
    case Command::Type::RELATIVE:
        break;
    case Command::Type::ABSOLUTE:
    {
        position_ = command->getTargetPosition();
        break;
    }
    case Command::Type::CALIBRATE:
        calibrated_ = true;
        position_ = 0;
        break;
    default:
        break;
    }
}

void Shutter::execute()
{
    if (commands_.empty())
    {
        return;
    }
    
    auto& command = commands_.front();
    const auto now_ms = millis();
    command->update(now_ms);
    switch (command->getStatus())
    {
    case Command::Status::TO_BE_SENT:
        executeSend(command);
        break;
    case Command::Status::EXECUTING:
        break;
    case Command::Status::DONE:
        executeDone(command);
        commands_.pop_front();
        break;
    default:
        return;
    }
}

void Shutter::clearQueue()
{
    commands_.clear();
}
