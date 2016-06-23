//
//  SharesManager.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>
#include <pthread.h>

//Library Headers
#include <cstdlib> 
//#include <Carbon/Carbon.h>
#include <sys/wait.h>
#include <cstdlib>


//Custom Headers
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "StandardShareDao.h"
#include "SharesManager.h"
#include "EngineBuffers.h"
#include "Constants.h"
#include "ShareUtil.h"


namespace Managers 
{
 
    
    //Initialize dao
    Daos::StandardShareDao * SharesManager::standardShareDao_=NULL;
    //Initialize list players
    Utils::List<Players::StandardPlayer> * SharesManager::players_=NULL;
    
    //ShareManager Methods Implementation

    //Method in charge of transmitting shares to players
    int SharesManager::saturateShares(  Daos::StandardShareDao * standardShareDao, Utils::List<Players::StandardPlayer> * players, Utils::List<Shares::StandardShare> * shares )
    {
        //send shares to all players
        for (int i=0; i<players->getLength(); i++) {

                    SharesManager::standardShareDao_->sendShareToPlayer(shares->get(i), players->get(i));
        }
        //infinite loop to retreive corresponding shares

        while (true) {
            bool full= true; //break condition
            //checks whether all corresponding shares have arrived
            for (int i=0; i<SharesManager::players_->getLength();i++ )
            {
                int  localVector=Buffers::EngineBuffers::incomingSharesBuffer_.size();
                 Shares::StandardShare* aux=Buffers::EngineBuffers::nextOperationBuffer_->get(i);
                if(aux==NULL || aux->getPlayerId()==0 || aux->getOperationId()!=Buffers::EngineBuffers::operationCounter_+1)
                {
                    full=false;
                    
                }
                
            }   
            // if all shares have arrived it copies them to the working buffer
            if (full)
            {
                for (int i=0; i<players->getLength(); i++) {
                    Shares::StandardShare *aux=Buffers::EngineBuffers::workingBuffer_->get(i);
                    Buffers::EngineBuffers::workingBuffer_->set(i,NULL);
                    delete aux;
                    Buffers::EngineBuffers::workingBuffer_->set(i, Buffers::EngineBuffers::nextOperationBuffer_->get(i));
                }
                
                break;
            }

        }
        //return succes or faliure
        return Utilities::Constants::TRANS_SUCCESS;
    };
    
    //obtain the shares
    int SharesManager::obtainShares(Utils::List<Shares::StandardShare> * shares)
    {
        return  saturateShares(SharesManager::standardShareDao_, players_, shares);
    }; 
    
    
    //Custom constructor
    SharesManager::SharesManager(Players::StandardPlayer * player,Utils::List<Players::StandardPlayer> * players)    
    {
        //Initialize variables
        SharesManager::standardShareDao_= new Daos::StandardShareDao(player,Utilities::Constants::UDP_SOCKET,false);
        SharesManager::players_=players;
  

        
    };
    
    
    //public access method for transmission functionality
    Utils::List<Shares::StandardShare> * SharesManager::transmitShares(Utils::List<Shares::StandardShare> * shares, int operationId)
    {
        //set the operation id on the shares to be sent
        for (int i=0; i<shares->getLength(); i++) 
        {
            shares->get(i)->setOperationId(operationId);
        }
        //copy the retreived data and free next operation buffer
        Utils::List<Shares::StandardShare>*list= NULL;

        if( this->obtainShares(shares)== Utilities::Constants::TRANS_SUCCESS)
        {
            
            list= Buffers::EngineBuffers::workingBuffer_->clone();



        }


        return list;
        
        
    };
    
    //destructor
    SharesManager::~SharesManager()
    {
        delete this->standardShareDao_;
    };

}


