//
//  ShamirGenerator.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_ShamirGenerator_h
#define edgeRuntime_ShamirGenerator_h

//Generic Headers
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "StandardShareDao.h"
#include "List.h"
namespace ShareGenerators
{
    /**
     * @class ShamirGenerator
     * @ingroup ShareGenerators
     * @brief
     * Engine of Shamir Shares
     * @details Creates Shamir Shares to be transmitted depending on the total number of players. It also includes shares operations relative to Shamir sharing \cite Shamir79.
     
     * @todo
     <ul>
     <li> Create methods for generation with custon mumber of players
     <li> Implement Exception Control.
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/13
     * @version 0.0.1.15
     */
    class ShamirGenerator
    {
    private:

        Players::StandardPlayer * player_; //local player
        long p_; //number of players
    public:
        
        /**
         * @brief Custom class constructor
         * @details Unique constructor. Initialize all estate variables
         * @param player  Represents the player owning the Generator
         * @param operation number to retrieve the information
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non serializable data
         </ul>
         */
        ShamirGenerator(Players::StandardPlayer * player, long p);
        
        /**
         * @brief Generate shamir shares for this player
         * @details given the number of players it fills a list with the corresponding shares of a secret.
         * @param secret represents the value to be divided into shares
         * @param players total number of players. It allows to determine how many shares are generated
         * @param list an empty list of shares that in case of success is filled with the shares.
         * @exception NA
         * @todo
         <ul>
                <li> Method is not dynamic on the number of players. It is only capable to execute with 3. Fix this bug such that it can be used for more than 3 players
                <li>Build and override method using only estate variables
         </ul>
         */
        int  generateShares(long secret, int players,Utils::List<Shares::StandardShare> * list);
        
        /**
         * @brief Generate regeneration shares for the multiplication
         * @details given the number of players it fills a list with the corresponding reshares of a share.
         * @param share represents the value to be divided into shares
         * @param secret represents the value to be divided into shares
         * @param players total number of players. It allows to determine how many shares are generated
         * @param list an empty list of shares that in case of success is filled with the shares.
         * @exception NA
         * @todo
         <ul>
         <li>Build and override method using only estate variables
         </ul>
         */
        int  multiplicationRegeneration(Shares::StandardShare * share, int players,Utils::List<Shares::StandardShare> * list);
        
        //class destructor
        virtual ~ShamirGenerator();
        
    };
}


#endif
