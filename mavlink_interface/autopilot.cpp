//
//  ardupilot.cpp
//  mavlink_interface
//
//  Created by Soubhi Hadri on 5/15/18.
//  Copyright © 2018 ardupilot. All rights reserved.
//

#include "autopilot.hpp"

autopilot::autopilot():
    io_service_(std::make_shared<boost::asio::io_service>()),
    socket_(std::make_shared<boost::asio::ip::udp::socket>(*io_service_)),
    endpoint_(std::make_shared<boost::asio::ip::udp::endpoint>()),
    commands_queue_(std::make_shared<std::queue<std::shared_ptr<mavlink_message_t>>>())
    {
        
    
}

void autopilot::receive(const std::string ip, const int port){
    std::vector<uint8_t> buffer(1024);//MAVLINK_MAX_PACKET_LEN);
    socket_->open(boost::asio::ip::udp::v4());
    socket_->bind(boost::asio::ip::udp::endpoint(
                                                 boost::asio::ip::address::from_string(ip), port));
    while(true){
        mavlink_message_t msg;
        std::fill(buffer.begin(), buffer.end(), 0);
        
        std::vector<uint8_t> buffer(MAVLINK_MAX_PACKET_LEN);
        int i = 0;
        mavlink_status_t status;
        auto buffer_size = socket_->receive_from(boost::asio::buffer(buffer),
                                                *endpoint_);        
        if(buffer_size > 0) {
            for(i = 0; i < buffer_size; ++i) {
                if(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
                    printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n",msg.sysid, msg.compid, msg.len, msg.msgid);
                    std::cout << std::endl;
                }
            }
        }
    }
    
}

void autopilot::connect(const std::string ip, const int port){

    connection_thread_ = std::thread(&autopilot::receive,this,ip,port);
    send_thread_ = std::thread(&autopilot::send,this);
//    connection_thread_.join();
}

void autopilot::send(){
    while(true){
        std::shared_ptr<mavlink_message_t> msg;
        if(commands_queue_->size()>0){
            std::cout<<"Sent msg"<<std::endl;
            msg = commands_queue_->front();
            commands_queue_->pop();
            auto bytes_sent = socket_->send_to(boost::asio::buffer(msg.get(),
                                                                   sizeof(*msg)), *endpoint_);
        }
    }
}

void autopilot::arm(){
    send_long_mavlink_command(MAV_CMD_COMPONENT_ARM_DISARM,0,0,1,0,0,0,0,0,0);
}

void autopilot::disarm(){
    send_long_mavlink_command(MAV_CMD_COMPONENT_ARM_DISARM,0,0,0,0,0,0,0,0,0);
}

void autopilot::send_long_mavlink_command(MAV_CMD cmd_id,const int target_system, const int target_component, const float param1,
                               const float param2, const float param3, const float param4,
                               const float param5, const float param6, const float param7){
    mavlink_command_long_t cmd_msg;
    memset(&cmd_msg, 0, sizeof(cmd_msg));
    cmd_msg.command = cmd_id;
    cmd_msg.target_system = 0;
    cmd_msg.target_component = 0;
    cmd_msg.confirmation = 0;
    cmd_msg.param1 = static_cast<float>(0);
    cmd_msg.param2 = static_cast<float>(0);
    cmd_msg.param3 = static_cast<float>(0);
    cmd_msg.param4 = static_cast<float>(0);
    cmd_msg.param5 = static_cast<float>(0);
    cmd_msg.param6 = static_cast<float>(0);
    cmd_msg.param7 = static_cast<float>(0);
    auto msg = std::make_unique<mavlink_message_t>();
    mavlink_msg_command_long_encode(0, 0, msg.get(), &cmd_msg);
    commands_queue_->push(std::move(msg));
}



