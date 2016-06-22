//
//  StandardShare.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_StandardShare_h
#define edgeRuntime_StandardShare_h

//Generic Headers
#include <iostream>

//Custom Headers
#include "List.h"

namespace Shares
{
    /**
     * @class StandardShare
     * @ingroup Shares
     * @brief
     * Plain Bean class of Shares
     * @details
     * contains communicational and identification data needed for transmission process, and value of the share
     * @author Abdelrahaman Aly
     * @date  2013/11/06
     * @version 0.0.1.15
     */
    class StandardShare
    {
    private:
        long value_; //calculated share value
        int playerId_; //player id who wons the share
        int operationId_; //operation id where the share was generated
       // Utils::List<Shares::StandardShare> * bits_; //bit decomposition of the share (also in shared form)
            
    public:
        
        /**
         * @brief Default class constructor
         * @details  Initialize all estate variables with default values
         
         * @exception NA
         * @todo
         <ul>

         <li>Build Exceptions for non compatible data, in case of ocupied ports
         </ul>
         */
        StandardShare();
        
        /**
         @brief Simple Getter
         @return value shared form of the value corresponding to a given player.
         */
        long getValue();
        
        /**
         @brief Simple Setter
         @param value shared form of the value corresponding to a given player.
         */
        void setValue(long value);
        
        /**
         @brief Simple Getter
         @return playerId player id of player who owns the share
         */
        int getPlayerId();
        
        /**
         @brief Simple Setter
         @param playerId player id of player who owns the share
         */
        void setPlayerId(int player);
        
        /**
         @brief Simple Getter
         @return playerId operation id of when the share was generated. In case it has not been transmitted it is pointing to 0.
         */
        int getOperationId();
        
        /**
         @brief Simple Setter
         @param playerId operation id of when the share was generated. In case it has not been transmitted it is pointing to 0.
         */
        void setOperationId(int operation);
        
        /**
         @brief Simple Getter
         @return bits a Utils::List full of the shared bit representation of the value in case it exists, it holds NULL else.
         */
        //Utils::List<Shares::StandardShare> * getBits();
        
        /**
         @brief Simple Setter
         @param bits a Utils::List full of the shared bit representation of the value in case it exists, it holds NULL else.
         */
        //void setBits(Utils::List<Shares::StandardShare> * bits);
        
        /**
         @brief Simple clone method
         @return share. A new instance of the current share object with the same estate variables values.
         */
        StandardShare * clone();
        
        /**
         @brief Simple toString method
         @return shareString. All state variables values of the current values, concatenated by an especial character.
         */
        std::string * toString();
        
        //standard destructor
        ~StandardShare();
    };
}    





#endif
