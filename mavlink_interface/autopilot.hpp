//
//  ardupilot.hpp
//  mavlink_interface
//
//  Created by Soubhi Hadri on 5/15/18.
//  Copyright © 2018 ardupilot. All rights reserved.
//

#ifndef autopilot_hpp
#define autopilot_hpp

#include <stdio.h>
#include <iostream>
#include <thread>
#include <queue>
#include <boost/asio.hpp>
#include "thirdparty/ardupilotmega/mavlink.h"

class autopilot{
public:
    autopilot();
    void connect(const std::string ip, const int port);
    
    void arm();
    void disarm();
    
    
private:
    void receive(const std::string ip, const int port);
    void send();
    void send_long_mavlink_command(MAV_CMD cmd_id,const int target_system, const int target_component, const float param1,
                                   const float param2, const float param3, const float param4,
                                   const float param5, const float param6, const float param7);
    
    std::thread connection_thread_;
    std::thread send_thread_;
    std::shared_ptr<boost::asio::io_service> io_service_ ;
    std::shared_ptr<boost::asio::ip::udp::socket> socket_;
    std::shared_ptr<boost::asio::ip::udp::endpoint> endpoint_;
    
    std::shared_ptr<std::queue<std::shared_ptr<mavlink_message_t>>> commands_queue_;

    
};
#endif /* autopilot_hpp */
