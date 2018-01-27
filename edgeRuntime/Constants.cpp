//
//  Constants.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>

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
    const int Constants::MAX_LIST_SIZE=2501;
    const int Constants::MAX_SIZE_RANDOM=10;
    const int Constants::OPERATION_COUNTER_DEFAULT_VALUE=0;
    const bool Constants::LISTENER_ALERTS=false;
    const bool Constants::SENDER_ALERTS=false;
    const int Constants::EXIT_SOCKET=1;
    const int Constants::LISTENING_SOCKET=2;
    const long Constants::SYSTEM_P=9182237390125665823L;//9182237390125665823L;//64081;//1024628340621757567;
    const NTL::ZZ_p Constants::SYSTEM_BIG_P= NTL::conv<NTL::ZZ_p>("121999492637070040497464880653482451122159715698431661862504934268987469885677710797799523307422120568454593141727668682332216679465216347609718241998150443969871262326615939878834844507147192404574401325870276945218845272041195113380201145626974399759092850500988371156171063899568397919181947787377580179491");
    const int Constants::SYSTEM_B=63;//16;//60;
    const int Constants::SHAREMIND_CARRY=1;
    //Shamir Share Constants -for Toft comparison methods to work z+k+l(n) <<P
    const int Constants::SYSTEM_L=32;
    const int Constants::SYSTEM_K=29;
}

