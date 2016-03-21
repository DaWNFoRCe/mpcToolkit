//
//  StandardPlayer.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Custom Headers
#include "StandardPlayer.h"

namespace Players
{
    //Method Implementations of Standard Player
    
    //Custom constructor
    StandardPlayer::StandardPlayer(int player, int port, int * ipAdress)
    {
        //state variable initialization
        this->player_=player;
        this->port_=port;
        this->ipAdress_=ipAdress;
    
    };

    
    StandardPlayer::StandardPlayer()
    {
        //state variables initialize to default values
        this->player_=0;
        this->port_=0;
        this->ipAdress_=NULL;
        
    };

    //set ip address
    void StandardPlayer::setIpAdress(int * ipAdress)
    {
        this->ipAdress_=ipAdress;
    };
    
    //get ip address
    int * StandardPlayer::getIpAdress()
    {
        return this->ipAdress_;  
    };
    
    //set player id
    void StandardPlayer::setPlayer(int player)
    {
        this->player_=player;  
    };
    
    //get player id
    int StandardPlayer::getPlayer()
    {
        return this->player_;
    };
    
    //set player port
    void StandardPlayer::setPort(int port)
    {
        this->port_=port;  
    };
    
    //get player port
    int StandardPlayer::getPort()
    {
        return this->port_;
    };
}