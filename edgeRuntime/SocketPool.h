//
//  SocketPool.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahman Aly. All rights reserved.
//

#ifndef edgeRuntime_UdpSocketPool_h
#define edgeRuntime_UdpSocketPool_h

//Custom Headers
#include "StandardPlayer.h"
#include "List.h"
#include "IStandardSocket.h"

namespace Pools
{
    /**
     * @class SocketPool
     * @ingroup Pools
     * @brief
     * Provides access to diffetent sockets to send and receive data.
     * @details You choose amongst the different configuration options and then throught the provided methods obtain active sockets.  
         The Class is also in charge to mantain the buffers with sockets both for in and out communicatiosn
     * @todo 
         <ul>
            <li> Implement a method to return the sockets to the pool.
            <li> Augment the offer of configuration methods.
        <\ul>
     * @author Abdelrahaman Aly
     * @date  2014/02/11
     * @version 0.0.1.15
     */
    class SocketPool
    {
    private:
        static Utils::List<Sockets::IStandardSocket> * inSockets_;  //Buffer for in Sockets
        static Utils::List<Sockets::IStandardSocket> * outSockets_; //Buffer for out Sockets
        static SocketPool * socketPool_; // singleton Instance of the class
        

    public:
        
        /**
         * @brief Default configuration for the Socket Pool.
         * @details Creates one socket for each  of the buffers,  an add them to the respective buffers, this configuration is thought especifically for secuencial use. Given that communications are secuencial it is ideal to only open an individual instance for both communication procedures. Sockets created under this configuration correspond to Sockets:UdpSockets.
         * @param palyer to whom the socket belongs to
         * @return The procedure status.
         * @retval status The following are the procedure status:
                    <ul>
                        <li>1= success
                        <li>0=faliure
                    </ul>
         * @exception NA
         * @todo
                <ul>
                    <li>Build Exceptions for Pooling and on exception control return 0
                </ul>
         */
        static int configureByDefault(Players::StandardPlayer * player);
        
        /**
         * @brief Returns Next Avalilable Socket for incoming transmission.
         * @details The amount of available sockets depends solely on the configuration selected previosuly and the number of processes curently using the sockets. Only in sockets (sockets on the incoming buffer) are provided.
         * @return A socket for incoming transmission.
         * @retval Sockets::IStandardSocket
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Socket Retrieval. In case no socket present to be returned. A mechanism has to be built for that purpose
         </ul>
         */
        static Sockets::IStandardSocket * getInSocket ();
        
        /**
         * @brief Returns Next Avalilable Socket for output transmission.
         * @details The amount of available sockets depends solely on the configuration selected previosuly and the number of processes curently using the sockets. Only out sockets (sockets on the output buffer) are provided.
         * @return A socket for output transmission.
         * @retval Sockets::IStandardSocket
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Socket Retrieval. In case no socket present to be returned. A mechanism has to be built for that purpose
         </ul>
         */
        static Sockets::IStandardSocket * getOutSocket ();
        
        /**
         * @brief Returns Singleton Instance in case is needed
         * @return Unique instance of SocketPool
         * @retval Sockets::IStandardSocket
         * @note It only returns the first shortest path found amongst all the possibilities, no optimization is present.
         * @exception NA
         * @todo
         <ul>
         <li>Build Exceptions for Socket Retrieval. In case no socket present to be returned. A mechanism has to be built for that purpose
         </ul>
         
         */
        static SocketPool * getInstance();
        
        
    };    
}



#endif
