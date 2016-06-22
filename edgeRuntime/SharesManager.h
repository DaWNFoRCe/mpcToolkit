//
//  SharesManager.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_SharesManager_h
#define edgeRuntime_SharesManager_h

//Generic Headers
#include <iostream>
#include <pthread.h>

//Library Headers
#include <cstdlib> 


//Custom Headers
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "StandardShareDao.h"
#include "SharesManager.h"
#include "Constants.h"
#include "List.h"

namespace Managers
{
    /**
     * @class SharesManager
     * @ingroup Managers
     * @brief
     * Facade class for shares I/O
     * @details Posses an unique Dao object on each instance. Implements the logic on transmission an reception of the information.
     * Methods utilize by default  the complete list of players for its functionality.
     * The facade is thought to be completely independent from the functionality of the runtime and endgine. It  should allow to send any type of shares with polymorphism
     * @note Not suppose to be used as singleton
     
     * @todo
     <ul>
     <li> Create methods fortransmission with custon mumber of players
     <li> The magment of the number of players should be managed only in this class and NOT in the Daos. Some administration is still delegated to the Daos, Specially on Send Methods.
     <li>Methods sould use generic types of sharing transmission.
     <li> Implement Exception Control.
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/13
     * @version 0.0.1.15
     */
    class SharesManager
    {
    private:
        
        
        //It is the interface of the controller - used to interact with sockets
        static Daos::StandardShareDao * standardShareDao_;        

        //Number of players - designed this way to avoid changes during execution on the ammount of participants
        static  Utils::List<Players::StandardPlayer> * players_;
        
        //send shares to players
        int saturateShares(  Daos::StandardShareDao * standardShareDao,  Utils::List<Players::StandardPlayer> * players,  Utils::List<Shares::StandardShare> * shares );
        //waits and copy the list of current shares
        int obtainShares( Utils::List<Shares::StandardShare> * shares);
        
    public:

        /**
         * @brief Englobes the current transmission functinoality. Transmit a share, and as a result receives the shares of the same iteration as a result.
         * @details Uses the current transaction number to obtain the corresponding shares after transmitting his. In case of failure returns NULL.
         * Transmits data to all players by default and shares to all players have to be provided.
         
         * @param shares to be transmitted to all players.
         * @param operation number to retrieve the information
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non serializable data
         </ul>
         */
        SharesManager(Players::StandardPlayer * player, Utils::List<Players::StandardPlayer> * players );
        
        /**
         * @brief Custom Constructor
         * @details Initialize estate variables, including unique dao.
         
         * @param player Represents player owning the Manager
         * @param players Cusstom List of all players for default communication
         * @return Custom Utils::List of the Shares of the corresponding operation.
         * @retval list The following are the values expected from this function:
           <ul>
              <li> List in case the operation has been succesfull
              <li> NULL in case operation fails.
           <\ul>
         * @exception NA
         * @todo
         <ul>
         <li>Build Exception Control
         </ul>
         */
        Utils::List<Shares::StandardShare> * transmitShares( Utils::List<Shares::StandardShare> * shares,int operationId);
        
        //Destructor
        ~SharesManager();
        
        
    };
    
}
#endif
