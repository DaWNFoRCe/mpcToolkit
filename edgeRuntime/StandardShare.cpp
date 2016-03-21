//
//  StandardSahre.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 06/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Custom Headers
#include "StandardShare.h"
#include "List.h"
#include "ShareUtil.h"

namespace Shares{
    
    //Method implementations of StandardShare
    
    //Default constructor
    StandardShare::StandardShare()
    {
     //   this->bits_=NULL;
        //this->setBits(NULL);
        this->setOperationId(-2);
        this->setPlayerId(0);
        this->setValue(0);
        
    };
    //getters and setters methods

    //get shared value
    long StandardShare::getValue()
    {
        return this->value_;
    };

    //set shared value
    void StandardShare::setValue(long value)
    {
        this->value_=value;
    };

    //get player id
    int StandardShare::getPlayerId()
    {
        return this->playerId_;
    };

    //set player id
    void StandardShare::setPlayerId(int player)
    {
       this->playerId_=player;
    }; 
    
    //get operation id
    int StandardShare::getOperationId()
    {
        return this->operationId_;
    };
    
    //set operation id
    void StandardShare::setOperationId(int operationId)
    {
        this->operationId_=operationId;
    };
    
    //get shared bits
    //Utils::List<Shares::StandardShare> * StandardShare::getBits()
    //{
    //    return this->bits_;
    //};
    
    //set shared bits
    //void StandardShare::setBits(Utils::List<Shares::StandardShare> * bits)
    //{
    //    this->bits_= bits;
    //};
    
    //clone the state variables of this object.
    StandardShare * StandardShare::clone()
    {
        StandardShare * share = new StandardShare();
        share->setPlayerId(this->getPlayerId());
        share->setValue(this->getValue());
        share->setOperationId(this->getOperationId());
       // if(this->getBits()!= NULL) share->setBits(this->getBits()->clone());
        return share;
    };
    
    StandardShare::~StandardShare()
    {
//        if (bits_!=NULL)
//        {
//            Utilities::ShareUtil::destroyList(bits_);
//        }
        //TODO: Destroy object and objects in list
    };
}
