#pragma once
#include "RFParams.h"
#include "Arduino.h"
#include <deque>
// _______  HEAD ___ HEAD ___ HEAD __ SELECT__ DIR __
// stop 4 11001011 01111010 01010001 00000100 01010101 
// up4    11001011 01111010 01010001 00000100 00010001
// down4  11001011 01111010 01010001 00000100 00110011

// stop 3 11001011 01111010 01010001 00000011 01010101 
// up3    11001011 01111010 01010001 00000011 00010001

// stop2  11001011 01111010 01010001 00000010 01010101 

// stop1  11001011 01111010 01010001 01010001 01010101 

// stopA  11001011 01111010 01010001 00000000 01010101

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

    enum Command
    {
        UP,
        DOWN,
        STOP,
        UNKNOWN_COMMAND
    };

    enum CommandType
    {
        NORMAL,
        ABSOLUTE,
        UNKNOWN
    };
};

struct ShutterCommand
{
    Shutter::CommandType type = Shutter::CommandType::UNKNOWN;
    Shutter::Device device  = Shutter::Device::UNKNOWN_DEVICE; 
    Shutter::Command command  = Shutter::Command::UNKNOWN_COMMAND; 
    int position = 0;
};


class RNDShutterController
{
public:
    void execute();
    void addCommand(const ShutterCommand& command);
    RNDShutterController(int transmit_pin);
    ShutterCommand decodeCommand(String command);
    ShutterCommand decodeAbsoluteCommand(String device_str, String position_str);
    void sendCommand(Shutter::Device device, Shutter::Command command);
    void sendAbsoluteCommand(Shutter::Device device, int position);
private:
    void processCommand(const ShutterCommand& command); 
    void calibrate(Shutter::Device device);
    void sendWord(unsigned char command);
    int transmit_pin_;
    int number_of_transmissions_ = 5;
    RFParams params_;
    std::deque<ShutterCommand> command_queue_;
    std::vector<int> shutter_calibrations_ {0, 0, 0, 0};
    std::vector<int> shutter_positions_ {0, 0, 0, 0}; // 0: up, 100: down [dont use 100 pls]
};