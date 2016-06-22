//
//  StandardShareDao.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_StandardShareDao_h
#define edgeRuntime_StandardShareDao_h
//Generic Headers
#include <iostream>

//Custom Headers
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "IStandardSocket.h"
#include "UdpSocket.h"

namespace Daos
{
    /**
     * @class StandardShareDao
     * @ingroup Daos
     * @brief
     * The following class provides  access to the communication controller
     * @details Class brings normalized access to information, Each thread that wants to send information should instantiate one Dao Class.
     *   The class in any case is responsible to look for an available socket an acomodate the information. Once the Socket has been used has to be returned to the Pool.
     *   For every type of Share should be a Dao to support it.
     *   Each Daos::StandradShareDao class has only one exit socket and in socket.
     * @note Not suppose to be used as singleton
     
     * @todo
     <ul>
     <li> Revise a way to be able to paralelize the access to sending the information
     <li> Create static methods to share and obtain information from a restricted amount of players.
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/06
     * @version 0.0.1.15
     */
    class StandardShareDao
    {
    private:
        bool alerts_;//control variable
        Sockets::IStandardSocket * inSocket_;// entrance socket assigned to Dao
        Sockets::IStandardSocket * outSocket_;// Exit socket Assigned to Dao
    public:
        
        /**
         * @brief Custom constructor, initialize the state variables.
         * @details It obtains a Socket from the Pool of the given Protocol Type and Instatiate State Variables
         * @note Only valid Constructor
         * @param player Represents the player owning the Dao (The  player who sends the data)
         * @param protocolType Uses Utilities::Constants to identify the type of supported protocol commmunication type that the Dao is going to establish
         * @param alerts Activates Verbose mode
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control
         </ul>
         */
        StandardShareDao(Players::StandardPlayer * player, int protocolType, bool alerts);
        
        /**
         * @brief Method to retrieve the share sent by a given player
         * @details Looks for the data on the working buffer. It has to be noted that only retrieve shares that have arrived and from the current operation counter position.
         * @param player who transmitted the share
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control in case the player has not sent the data (limited set of interactions)
         </ul>
         */
        Shares::StandardShare * obtainShareFromPlayer(Players::StandardPlayer * player);
        
        /**
         * @brief Method to send a share to a given player
         * @details Method is not aware of ulterior functionality and hence is unaware of Shamir Sharing needs. Functionality Method thought to be able  to send any share to any player.
         * @param share  to be transmitted
         * @param player sending the share
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Transmision and on exception control in case the player can not send the data (limited set of interactions)
         </ul>
         */
        int sendShareToPlayer(Shares::StandardShare * share, Players::StandardPlayer * player);
        
        /**
         * @brief Utility method used to transform a share to a sequence of char characters that can be transmitted by the controller
         * @details Uses a separator defined in Utilities::Constants and concatenate all estate variables:
         * @code
         char * var =StandardShareDao::serializeStandardShare(myShare);
         cout<< var;
         id%value$operation%
         * @endcode
         * @param share data to be serialized
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non serializable data
         </ul>
         */
        static char * serializeStandardShare(Shares::StandardShare * share);
        
        /**
         * @brief Utility method used to transform a serialized share into  an Object Shares::StandardShare
         * @details Typically the controller will give as a result the communication, a sequence of characters -numbers and a especial character-, this messages use the separator defined by Utilities::Constants to split and instantiate the estate of the Share.
         * @code
         Shares::StandardShare  var =StandardShareDao::buildDTO(myMessage);
         * @endcode
         *@param share data to be serialized
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non serializable data
         </ul>
         */
        static Shares::StandardShare * buildDto(char * message);
        
        /**
         @brief Simple Getter
         @return if alerts are activated
         */
        bool isAlerts();
        
        /**
         @brief Simple Setter
         @param alerts activates/deactivates alerts
         */
        void setAlerts(bool alerts);
        
    };
}

#endif


