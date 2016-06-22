//
//  ShareUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/28/14.
//
//

#ifndef edgeRuntime_ShareUtil_h
#define edgeRuntime_ShareUtil_h

//Generic Headers

//Custom Headers
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"

namespace Utilities
{
    
    /**
     * @class ShareUtil
     * @ingroup Utilities
     * @brief
     * Utility class for Share operations.
     * @details Static class. Do not instantiate. This class has to be used to create all shares when new types of them arise.
     * @todo
     <ul>
     <li> Create a private constructor.
     <li> Implement Exception Control.
     <li>  Change the class methods to make them more modular, real object oriented, like the lagrange functionality
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2014/01/28
     * @version 0.0.1.15
     */
    class ShareUtil
    {
    public:
        
        /**
         * @brief Method use to create standard shares.
         * @details Receive raw data and in turn produce a share
         * @param playerId player who owns the share to be sent.
         * @param value  is the value of the share
         * @return share Configured as a share of playerId with value value.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static Shares::StandardShare * wrapStandardShare(int playerId,long value);
        
        /**
         * @brief Method use to swap standard shares.
         * @details Receive two shares and interchange their values. This is not a secure method, reveals the memory flow. It has to be used correctly in context to avoid information leaks.
         * @param playerId player who owns the share to be sent.
         * @param value  is the value of the share
         * @return share Configured as a share of playerId with value value.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static int exchangeShares(Shares::StandardShare ** a, Shares::StandardShare ** b);
        
        /**
         * @brief Method use to create a list of standard shares belonging to the same player.
         * @details Receive raw data and in turn produce a Utils::List of shares
         * @param playerId player who owns the shares to be sent.
         * @param values  List of the values of the shares.
         * @param size size of the values vector.
         * @return shares Configured share list of playerId with value values.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static Utils::List<Shares::StandardShare>* wrapStandardShareList(int  playerId,int *values, int size);
        
        
        /**
         * @brief Method use to destroy a list of standard shares belonging to the same player.
         * @details Receive a list of data and in turn destroys all instances in the container and the container itself
         * @param list container of objects to be destroyed.
         * @return status returns 1 if the result was correct, 0 otherwise.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static int destroyList( Utils::List<Shares::StandardShare> * list);
        
        /**
         * @brief Method use to destroy a matrix of standard shares belonging to the same player.
         * @details Receive a matrix of data and in turn destroys all instances in the container and the container itself
         * @param matrix container of objects to be destroyed.
         * @return status returns 1 if the result was correct, 0 otherwise.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for non interpolable data
         </ul>
         */
        static int destroyMatrix( Utils::Matrix<Shares::StandardShare> * matrix);
        
        static Utils::Matrix<Shares::StandardShare> *  buildMatrix( Utils::Matrix<long> * matrix, int playerId);
        
        
    };
    
}


#endif
