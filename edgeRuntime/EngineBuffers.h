//
//  EngineBuffers.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

#ifndef edgeRuntime_EngineBuffers_h
#define edgeRuntime_EngineBuffers_h
//Generic Headers
#include <vector>

//Custom Headers
#include "StandardShare.h"
#include "StandardShareDao.h"
#include "List.h"


namespace Buffers
{
    /**
     * @class EngineBuffers
     * @ingroup Buffers
     * @brief
     * EngineBuffers provides access to accross application data buffers
     * @details  
     * Buffers are necessary to mantain  information received from the Listener, and made available for the application as a hole.
         Additionally, the class stores some general parametrization of the application i.e. the operation counter, allows to identify the next operation to be executed
     * @note This class does not need to be instantiated
     
     * @todo
     <ul>
     <li> ELIMINATE THE CLASS ----- Migrate the Buffers to the Engine they do not belong to the application, but to the Engine.
     <li> Revise a way to be able to paralelize the access to sending the information
     <\ul>
     * @author Abdelrahaman Aly
     * @date  2013/11/29
     * @version 0.0.1.15
     */
    class EngineBuffers
    {
    private:
    public:
        static std::vector<Shares::StandardShare *>  syncBuffer_;
        static std::vector<Shares::StandardShare *>  incomingSharesBuffer_; //buffer of all arriving shares
        static Utils::List<Shares::StandardShare > * workingBuffer_;  //buffer of last complete set of operations
        static Utils::List<Shares::StandardShare> * nextOperationBuffer_;//extra buffer to warranty the conservation of the next operation data
        static int operationCounter_;//counter of the total number of operations of the application.
        
        
    };
}

#endif
