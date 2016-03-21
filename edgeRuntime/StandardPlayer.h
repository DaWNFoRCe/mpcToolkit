//
//  StandardPlayer.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_StandardPlayer_h
#define edgeRuntime_StandardPlayer_h

//Generic Headers
#include <string.h>
namespace Players
{
    /**
     * @class StandardPlayer
     * @ingroup Players
     * @brief
     * Player Bean class
     * @details
     * contains communicational and identification data needed for transmission process
     * @todo
     <ul>
     <li> change player for idPlayer
     <li> correct address - now reads adress
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/06
     * @version 0.0.1.15
     */
    class StandardPlayer
    {
    private:
        int * ipAdress_; //ip address of player
        int port_; //service port
        int player_; //player id i.e 1, 2, 3, etc.
    public:
        /**
         * @brief Custom class constructor
         * @details  Initialize all estate variables
         * @param player  Represents the player id number
         * @param port service port
         * @param ipAddress int vector of size 4 to store service ip address
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non compatible data, specially ip Address
         </ul>
         */
        StandardPlayer(int player, int port, int * ipAdress);
        
        /**
         * @brief Default class constructor
         * @details  Initialize all estate variables with default values

         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non compatible data, in case of ocupied ports
         </ul>
         */
        StandardPlayer();
        
        /**
         @brief Simple Getter
         @return ipAddress int vector of size 4 to store service ip address
         */
        int * getIpAdress();
        
        /**
         @brief Simple Setter
         @param ipAdress int vector of size 4 to store service ip address
         */
        void setIpAdress(int * ipAdress);
        
        /**
         @brief Simple Getter
         @return player  id i.e 1, 2, 3, etc.
         */
        int getPlayer();
        
        /**
         @brief Simple Setter
         @param player  id i.e 1, 2, 3, etc.
         */
        void setPlayer(int player);
        
        /**
         @brief Simple Getter
         @return port service port of player.
         */
        int getPort();
        
        /**
         @brief Simple Setter
         @param port service port of player.
         */
        void setPort(int port);
    };
}

#endif
