//
//  Constants.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 15/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_Constants_h
#define edgeRuntime_Constants_h

namespace Utilities {

    /**
     * @class Constants
     * @ingroup Utilities
     * @brief
     * Held all the parametrization of the application, and the system constants.
     * @details
     * Using a class like this to manage Constans in an application in C will mask at compilation time
     the value of the constants, not allowing to be trated as literals on compilation time.
     Defining their values directly in this file would change that reality. As their values are visible
     at compilation time you would be allow to use them to define arrays and to use switches

     * @note This class does not need to be instantiated
     
     * @todo
     <ul>
     <li> revise the use of all the constants
     <li> load constants from an external file
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/15
     * @version 0.0.1.15
     */
    class Constants
    {
    public:
        //transmission constants
        static const int TRANS_SUCCESS;
        static const int TRANS_FAILURE;
        static const int  UDP_SOCKET;
        static const int  TCP_SOCKET;
        static const char *  SERIALIZATION_SEPARATOR;
        static const int LISTENER_THREADS;
        static const int MAX_LIST_SIZE;
        static const int MAX_SIZE_RANDOM;
        static const int OPERATION_COUNTER_DEFAULT_VALUE;
        static const bool LISTENER_ALERTS;
        static const bool SENDER_ALERTS;
        static const int EXIT_SOCKET;
        static const int LISTENING_SOCKET;

        //Parametrization
        
        //engine constants
        static const long SYSTEM_P;
        static const int SYSTEM_B;
        static const int SHAREMIND_CARRY;
        
        //Shamir Share Constants -for Toft comparison methods to work z+k+l(n) <<P
        static const int SYSTEM_L;
        static const int SYSTEM_K;
    };
}

#endif
