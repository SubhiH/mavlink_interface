//
//  main.cpp
//  mavlink_interface
//
//  Created by Soubhi Hadri on 5/15/18.
//  Copyright © 2018 ardupilot. All rights reserved.
//

#include <iostream>
#include <boost/iostreams/stream.hpp>
#include <boost/chrono.hpp>

#include "autopilot.hpp"



int main(int argc, const char * argv[]) {
    // insert code here...
    autopilot pilot;
    pilot.connect("127.0.0.1",14554);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    pilot.arm();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Hello, World!\n";
    return 0;
}
