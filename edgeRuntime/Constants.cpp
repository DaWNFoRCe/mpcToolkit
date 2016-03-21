//
//  Constants.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Custom Headers
#include "Constants.h"
namespace Utilities
{
    
    const int Constants::TRANS_SUCCESS=1;
    const int Constants::TRANS_FAILURE=0;
    const int  Constants::UDP_SOCKET=1;
    const int  Constants::TCP_SOCKET=2;
    const char *  Constants::SERIALIZATION_SEPARATOR="%";
    const int Constants::LISTENER_THREADS=1;
    const int Constants::MAX_LIST_SIZE=2050;
    const int Constants::MAX_SIZE_RANDOM=10;
    const int Constants::OPERATION_COUNTER_DEFAULT_VALUE=0;
    const bool Constants::LISTENER_ALERTS=false;
    const bool Constants::SENDER_ALERTS=false;
    const int Constants::EXIT_SOCKET=1;
    const int Constants::LISTENING_SOCKET=2;
    const long Constants::SYSTEM_P=9182237390125665823L;//9182237390125665823L;//64081;//1024628340621757567;
    const int Constants::SYSTEM_B=63;//16;//60;
    const int Constants::SHAREMIND_CARRY=1;
    //Shamir Share Constants -for Toft comparison methods to work z+k+l(n) <<P
    const int Constants::SYSTEM_L=32;
    const int Constants::SYSTEM_K=29;
}

