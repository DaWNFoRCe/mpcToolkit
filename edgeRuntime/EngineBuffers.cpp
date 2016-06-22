//
//  EngineBuffers.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Custom Headers
#include "EngineBuffers.h"
#include "StandardShareDao.h"
#include "StandardShare.h"
#include "List.h"
#include "Constants.h"

namespace Buffers
{
    //Variable Instantiaiton from EngineBuffer class
    std::vector<Shares::StandardShare *>   EngineBuffers::syncBuffer_;
    std::vector<Shares::StandardShare *>   EngineBuffers::incomingSharesBuffer_;
    Utils::List<Shares::StandardShare> * EngineBuffers::workingBuffer_=NULL; 
    Utils::List<Shares::StandardShare> * EngineBuffers::nextOperationBuffer_=NULL;
    
    int EngineBuffers::operationCounter_=Utilities::Constants::OPERATION_COUNTER_DEFAULT_VALUE;
    
    
}