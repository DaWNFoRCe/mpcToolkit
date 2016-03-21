//
//  SocketPool.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Custom Headers
#include "SocketPool.h"
#include "IStandardSocket.h"
#include "UdpSocket.h"
#include "Constants.h"
#include "List.h"

namespace Pools
{
    //Pools instantiation
    Utils::List<Sockets::IStandardSocket > * SocketPool::inSockets_=NULL;
    Utils::List<Sockets::IStandardSocket > * SocketPool::outSockets_=NULL;
    
    //Implementation of SocketPool Methods
    int SocketPool::configureByDefault(Players::StandardPlayer * player)// Enables Default Configuration
    {
        //All Buffers instantiated of size 1
        SocketPool::inSockets_= new Utils::List<Sockets::IStandardSocket >(1);
        SocketPool::outSockets_= new Utils::List<Sockets::IStandardSocket >(1);
        
        //Creation of Sockets
        Sockets::IStandardSocket * inSocket= (Sockets::IStandardSocket *) new Sockets::UdpSocket(player, Utilities::Constants::LISTENING_SOCKET);
        Sockets::IStandardSocket * outSocket= (Sockets::IStandardSocket *) new Sockets::UdpSocket(player,Utilities::Constants::EXIT_SOCKET);
        SocketPool::inSockets_->add(inSocket);
        SocketPool::outSockets_->add(outSocket);
        //Return if success 1
        return 1;
    };
    
    Sockets::IStandardSocket * SocketPool::getInSocket()
    {
        Sockets::UdpSocket * socket=NULL;
        socket=(Sockets::UdpSocket * )SocketPool::inSockets_->get(0);        
        return  socket;
    };   
    Sockets::IStandardSocket * SocketPool::getOutSocket()
    {
        Sockets::UdpSocket * socket=NULL;
        socket=(Sockets::UdpSocket * )SocketPool::outSockets_->get(0);        
        return  socket;
    }; 
}
