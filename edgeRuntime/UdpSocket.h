//
//  UdpSocket.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_Socket_h
#define edgeRuntime_Socket_h

//Library Headers
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

//Custom Headers
#include "IStandardSocket.h"
#include "StandardShare.h"
#include "StandardPlayer.h"
namespace Sockets 
{
    /**
     * @class UdpSocket
     * @ingroup Sockets
     * @brief
     * Class represents socket instances with an in or out handler.
     * @details 
     * Class impementes Sockets::IStandardSocket. 
     * Each class only represents an in or an out socket and build a single handler for that matter.
     * @note Not suppose to be used as singleton.
     * @todo
     <ul>
     <li> Revise a way to be able to paralelize the access to sending the information.
     <li> Create static methods to share and obtain information from a restricted amount of players.
     <li> The Controller should have their own constants - or at least in the same class. 
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/06
     * @version 0.0.1.15
     */
    class UdpSocket: public Sockets::IStandardSocket
    {
    private: 
        Players::StandardPlayer * player_; // player hosting the socket
        int handleIn_; //in handler of the socket
        int handleOut_; //out handler of the socket
    public:
        
        /**
         * @brief
         * Custom constructor, initialize the state variables.
         * @details 
         * It creates the handles (in and out) with an UDP configuration as corresponding.
         * @note Only valid Constructor
         * @param player Represents the player owning the socket (The  player who sends the data)
         * @param type Uses Utilities::Constants to identify the type of supported protocol commmunication type that the UDP socket is going to establish
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control
         </ul>
         */
        UdpSocket(Players::StandardPlayer * player, int type );

        /**
         * @brief 
         * Sends message to a given player.
         * @details 
         * Uses the handle it was created to send a string through the UDP socket.
         * @param player Represents the player owning the Dao (The  player who sends the data).
         * @param type Uses Utilities::Constants to identify the type of supported protocol commmunication type that the UDP socket is going to establish.
         * @param alerts Activates verbose mode for method.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control
         </ul>
         */
        virtual int  sendTo(Players::StandardPlayer * player,std::string data,bool alerts);
        
        /**
         * @brief
         * Retreives message to a given player.
         * @details
         * Uses the handle it was created to retreive a string through the UDP socket.
         * @note
         *  Application (Parent user classes) has to be waiting a message.
         * @param player Represents the player owning the Dao (The  player who sends the data).
         * @param type Uses Utilities::Constants to identify the type of supported protocol commmunication type that the UDP socket is going to establish.
         * @param alerts Activates verbose mode for method.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control.
         </ul>
         */
        virtual char * receiveFrom(Players::StandardPlayer * player, bool alerts);
        
        //default destructor
        virtual ~UdpSocket();
    };
}

#endif
