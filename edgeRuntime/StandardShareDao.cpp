//
//  StandardShareDao.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 05/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <string>

//Custom Heathers
#include "StandardShareDao.h"
#include "SocketPool.h"
#include "StandardShare.h"
#include "StandardPlayer.h"
#include "UdpSocket.h"
#include "Constants.h"
#include "IStandardSocket.h"

#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>

namespace Daos 
{
    
    Daos::StandardShareDao::StandardShareDao(Players::StandardPlayer *player,int socketType, bool alerts){
     
        if (socketType==1){

            this->inSocket_= (Sockets::UdpSocket* )Pools::SocketPool::getInSocket();
            this->outSocket_= (Sockets::UdpSocket* )Pools::SocketPool::getOutSocket();
            //this->socket_= new Sockets::UdpSocket(player);
        }
        this->alerts_=alerts;
    };
    
    std::string StandardShareDao::zToString(const ZZ_p &z) {
        std::stringstream buffer;
        buffer << z;
        return buffer.str();
    };
    
    Shares::StandardShare * StandardShareDao::obtainShareFromPlayer(Players::StandardPlayer * player)
    {
        char * message = this->inSocket_->receiveFrom(player,this->alerts_);


        return StandardShareDao::buildDto(message);
    };
    
    int StandardShareDao::sendShareToPlayer(Shares::StandardShare * share, Players::StandardPlayer * player)
    {
        char * serializedData= this->serializeStandardShare(share);
        return this->outSocket_->sendTo(player,serializedData,this->alerts_);
    };
    
    
     char  * StandardShareDao::serializeStandardShare(Shares::StandardShare * share)
    {
        
        char  shareId[1024] ="";
        char  shareValue[1024]="";
        char  operationId[1024]="";
        //Cast to string the values of the share
        snprintf(shareId,sizeof shareId, "%d",share->getPlayerId());
        if(share->getOperationId()==-1)
        {
            snprintf(shareValue,sizeof shareValue,"%ld", share->getValue());
        }
        else
        {
            std::stringstream buffer;
            buffer << share->getValuep();
            //shareValue=strdup(buffer.str().c_str());
            //shareValue = NTL::conv<const char *>(share->getValuep());
            snprintf(shareValue,sizeof shareValue,"%s", buffer.str().c_str());
        }
        snprintf(operationId,sizeof operationId,"%d", share->getOperationId());
        
        //Concat the values of the share to be send
        char  serializedShare[1024]="";//= shareId;
        strcat(serializedShare,shareId);
        //delete shareId;
        strcat(serializedShare,Utilities::Constants::SERIALIZATION_SEPARATOR);
        strcat(serializedShare,shareValue);
        strcat(serializedShare,Utilities::Constants::SERIALIZATION_SEPARATOR);
        strcat(serializedShare,operationId);
        strcat(serializedShare,Utilities::Constants::SERIALIZATION_SEPARATOR);
        strcat(serializedShare,"\0");
        //strcat(serializedShare,"endl");
        //strcat(serializedShare,Utilities::Constants::SERIALIZATION_SEPARATOR);

        
        return serializedShare;
    }
    
    
     Shares::StandardShare * StandardShareDao::buildDto(char * message)
    {
	//std::cout<<"Look this might be the problem "<<message <<"\n";//<< " "<< splitedData[0]<<" "<<splitedData[1]<<" "<<splitedData[2] <<"the end\n";
        Shares::StandardShare * share = new Shares::StandardShare();
        std::string splitedData[3];//TODO define this as a constant parameter for StandardShareDao
        
        //TODO to put this into a method is 
        char  aux_ [1024];
	strcpy(aux_,message);
        char * aux = strtok (aux_,Utilities::Constants::SERIALIZATION_SEPARATOR);
        int i=0;
	//std::cout<<"output: "<< message<<"\n";
        //std::cout<<Utilities::Constants::SERIALIZATION_SEPARATOR<< "Look this might be the problem "<<aux <<" "<< i<<"\n";
        while (aux != NULL)
        {
	//std::cout<<"Look this might be the problem "<<aux <<" "<< i<<"\n";
            splitedData[i]=aux;
            i++;
            aux = strtok (NULL,Utilities::Constants::SERIALIZATION_SEPARATOR);
        }
       // share->setBits(NULL);
	
        share->setPlayerId(std::atoi(splitedData[0].c_str()));
        //if(splitedData[1].size()>1) std::cout<<"Look this might be the problem "<<splitedData[1]<<"the end\n";
        share->setValue(std::atol(splitedData[1].c_str()));// this is a long sized int, it can't be translated with atoi 
        share->setOperationId(std::atoi(splitedData[2].c_str()));
	//std::cout<<"so now we are just here "<<share->getValue()<<" soo \n";
       	if (share->getOperationId()!=0 && share->getValue()!=0 && share->getPlayerId()!=0)
	{
		//std::cout<<"funk"<<"\n";
		share->setValuep(conv<ZZ_p>(conv<ZZ>(splitedData[1].c_str())));
	}
	//std::cout<<"doesnt get here"<<"\n";
        if (i==0)
	 {
            return NULL;
        }
        return share;
    
    };
    bool StandardShareDao::isAlerts()
    {
        return alerts_;
    };
    
    void StandardShareDao::setAlerts(bool alerts)
    {
        this->alerts_=alerts;
    };
    


}
