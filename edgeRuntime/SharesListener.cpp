//
//  SharesListener.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 29/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>
#include <unistd.h>

//Custom Headers
#include "SharesListener.h"
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "List.h"
#include "EngineBuffers.h"
#include "Constants.h"
#include <list>

namespace Listeners
{
    //State variables instantiation
    Daos::StandardShareDao * SharesListener::standardShareDao_=NULL;
    bool SharesListener::alive_=false;
    SharesListener * SharesListener::listener_=NULL;
    Players::StandardPlayer * SharesListener::player_=NULL;
    Utils::List<Players::StandardPlayer> * SharesListener::players_=NULL;
    
    //Implementation of methods from SharesListener
    void * Listeners::SharesListener::listenerProcess(void *threadid)
    {
        //Listener Main Loop
        while (true) 
        {
            //Obtain the Shares that have been captured by the network card
            Shares::StandardShare * aux = SharesListener::standardShareDao_->obtainShareFromPlayer(NULL);
            //if(aux->getOperationId()>-1) std::cout<<"wow is here: "<< aux->getValue() <<"  "<<aux->getPlayerId() <<"\n";
            //Clasifies Share
            if (aux != NULL) 
            {
                if (aux->getPlayerId()==0 && aux->getOperationId()==-1)
                {
                    if (Buffers::EngineBuffers::syncBuffer_[aux->getValue()-1]==NULL)
                    {   
                        //allows sync with share 0

                        Buffers::EngineBuffers::syncBuffer_[aux->getValue()-1]=aux;
                    }
                    else if(Buffers::EngineBuffers::syncBuffer_[aux->getValue()-1]->getOperationId()==-1)
                    {
                        Shares::StandardShare * destroy =Buffers::EngineBuffers::syncBuffer_[aux->getValue()-1];
                        Buffers::EngineBuffers::syncBuffer_[aux->getValue()-1]=aux;
                        delete destroy;
                    }
                    else
                    {
                        delete aux;
                    }
                
                    continue;
                }
                
                //save the corresponding share
                Buffers::EngineBuffers::incomingSharesBuffer_.push_back(aux);

            }
            
            //moves information from incoming  buffer to working buffer once all the information from the players needed
            for ( int i =0; i<Buffers::EngineBuffers::incomingSharesBuffer_.size();) 
            {
                if ((Buffers::EngineBuffers::incomingSharesBuffer_[i])!=NULL )
                    if(Buffers::EngineBuffers::incomingSharesBuffer_[i]->getOperationId()==Buffers::EngineBuffers::operationCounter_+1) 
                    {
                        //you could instead send them directly and make this operation faster
                        Buffers::EngineBuffers::nextOperationBuffer_->set(Buffers::EngineBuffers::incomingSharesBuffer_[i]->getPlayerId()-1, Buffers::EngineBuffers::incomingSharesBuffer_[i]->clone());
                        std::swap(Buffers::EngineBuffers::incomingSharesBuffer_[i],Buffers::EngineBuffers::incomingSharesBuffer_.back());
                        Shares::StandardShare * incoming_aux=Buffers::EngineBuffers::incomingSharesBuffer_.back();
                        Buffers::EngineBuffers::incomingSharesBuffer_.pop_back();
                        delete incoming_aux;                    
                        continue;
                    }   
                ++i;
                
            }
            if(SharesListener::alive_ ==false)
            {
                pthread_exit(NULL);
            }
        }

    };
    
    
    
    SharesListener::SharesListener(Players::StandardPlayer * player,Utils::List<Players::StandardPlayer> * players)
    {
        if (SharesListener::standardShareDao_==NULL)
        {
            SharesListener::standardShareDao_=  new Daos::StandardShareDao(player,Utilities::Constants::UDP_SOCKET,Utilities::Constants::LISTENER_ALERTS);
        }
        if (SharesListener::player_==NULL) 
        {
            SharesListener::player_=player;    
        }
        if (SharesListener::players_==NULL) 
        {
            SharesListener::players_=players;
        }

    };
    
    
    int SharesListener::startListner()
    {
        //TODO: Verify how well this method works
        //clear engine buffer
        Buffers::EngineBuffers::incomingSharesBuffer_.clear();
        SharesListener::alive_=true;
        //setting sync share
        Shares::StandardShare * syncShare= new Shares::StandardShare();
        syncShare->setPlayerId(0);
        syncShare->setValue(player_->getPlayer());
        syncShare->setOperationId(-1);
        
        //initialize and start threat
        pthread_t threads[Utilities::Constants::LISTENER_THREADS];
        int rc;
        for(int i=0; i < Utilities::Constants::LISTENER_THREADS; i++ ){
            rc = pthread_create(&threads[i], NULL, 
                                SharesListener::listenerProcess, (void *)i);
            
            if (rc){
                std::cout << "Error:unable to create thread," << rc << std::endl;
                return Utilities::Constants::TRANS_FAILURE;
            }
        }
        
        std::cout<<"Synchronizing players..... --->Please Wait d*_*b \n";
        
        
        //sync process
        

        

        //TODO: change the use of the players list for the size of buffer
        while (true)
        {
            bool full= true;
            
            for (int i=0; i<SharesListener::players_->getLength(); i++)
            {
                
                standardShareDao_->sendShareToPlayer(syncShare, players_->get(i));
                
                usleep(100);
                
            }
            
            for(int i=0; i< players_->getLength();i++)
            {
              
                Shares::StandardShare* aux=NULL;

                aux= Buffers::EngineBuffers::syncBuffer_[i];
                

                if(aux==NULL)
                {
                    full=false;
                    break;
                }
            }
            if (full) {
                //precaution resend -- this is necessary to free processes that might be blocked 
                // by the waiting -- at this point we know everybody is listening. this way there is 
                //no possibility of a deadlock
                for (int j=0; j<100; j++) {
                    for (int i=0; i<players_->getLength(); i++) {
                    
                        SharesListener::standardShareDao_->sendShareToPlayer(syncShare, players_->get(i));
                        std::cout<<".";
                        usleep(10);
                    
                    }
                    
                    usleep(100);
                }
                std::cout<<"\n";
                break;
            }

        }        

        //end sync process
        usleep(10000000);
        std::cout<<"\n Synchronization done! That was fast isn't it? Thanks for Waiting ;) \n | | |\n v v v\n";
        

        
        return 1;
    };
    
    
    bool  SharesListener::isAlive()
    {
        return SharesListener::alive_;
    };
    
    int  SharesListener::stopListener()
    {
        SharesListener::alive_=false;
        return 1;
    };
    
    //Singleton Implementation
    SharesListener * SharesListener::getListener(Players::StandardPlayer * player,Utils::List<Players::StandardPlayer> * players)
    {
        if (SharesListener::listener_==NULL) 
        {
            SharesListener::listener_= new SharesListener(player,players);
        }
        return SharesListener::listener_;
    };
    
    
}

//TODO:This code could be optimized because to access so many static resources is not healthy
// with a list that is capable to reorder it's elements and to seek them using the player and operation and then erasing them.. that could be much more easier than this game of passing things around... 


//allows normal shares to be stored
//Buffers::EngineBuffers::incomingSharesBuffer_->push_back(aux->getPlayerId()-1,aux);
