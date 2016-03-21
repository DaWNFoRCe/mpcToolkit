//
//  IStandardSocket.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 15/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_IStandardSocket_h
#define edgeRuntime_IStandardSocket_h

//Custom Headers
#include "StandardPlayer.h"

namespace Sockets
{
    /**
     * @class IStandardSocket
     * @ingroup Sockets
     * @brief
     * Parent Class represents socket instances with an in or out handler. To be understood as an Abstract class
     * @details
     * Meant to be abstract class. Not Meant To Be Implmented.
     * @author Abdelrahaman Aly
     * @date  2013/11/15
     * @version 0.0.1.15
     */
    class IStandardSocket
    {
    private:

    public:
        
        //constructor. Not to be implemented.
        IStandardSocket();
        /**
         * @brief
         * Abstract send method. Sends message to a given player.
         * @details
         * It should use the handle it was created to send a string through the UDP socket.
         * @param player Represents the player owning the Dao (The  player who sends the data).
         * @param type Uses Utilities::Constants to identify the type of supported protocol commmunication type that the UDP socket is going to establish.
         * @param alerts Activates verbose mode for method.
         * @exception NA
         */
        virtual int sendTo(Players::StandardPlayer * player, std::string data, bool alerts)=0;
        
        /**
         * @brief
         * Abstract receive method. Retreives message to a given player.
         * @details
         * It should use the handle it was created to retreive a string through the UDP socket.
         * @note
         *  Application (Parent user classes) has to be waiting a message.
         * @param player Represents the player owning the Dao (The  player who sends the data).
         * @param type Uses Utilities::Constants to identify the type of supported protocol commmunication type that the UDP socket is going to establish.
         * @param alerts Activates verbose mode for method.
         * @exception NA
         */
        virtual char * receiveFrom(Players::StandardPlayer * player, bool alerts)=0;
        
        //standard destructor
        virtual ~IStandardSocket();
        
    };
}



#endif
