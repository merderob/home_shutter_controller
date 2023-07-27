#pragma once

struct RFParams
{
    /// @brief The high value of a "zero" bit when sending data. [us]
    static const int zero_high_send = 350;
    /// @brief The low value of a "zero" bit when sending data. [us]
    static const int zero_low_send = 700;
    /// @brief The high value of a "zero" bit when receiving data. [us]
    static const int zero_high_receive = 350;
    /// @brief The low value of a "zero" bit when receiving data. [us]
    static const int zero_low_receive = 700;

    /// @brief The high value of a "one" bit when sending data [us]
    static const int one_high_send = 630;
    /// @brief The low value of a "one" bit when sending data [us]
    static const int one_low_send = 300;
    /// @brief The low value of a "one" bit when receiving data [us]
    static const int one_high_receive = 630;
    /// @brief The low value of a "one" bit when receiving data [us]
    static const int one_low_receive = 400;
    
    static const int sync_on = 4700;
    static const int sync_off = 1500; 

    static const int delay_between_packets_send = 7400;
    /// @brief  The delay between ttwo packets when receiving [us]
    static const int delay_between_packets_receive = 7800;

    static const int delay_tolerance = 200; // TODO
};