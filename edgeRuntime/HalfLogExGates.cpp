//
//  HalfLogExGates.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/21/15.
//
//

#include "HalfLogExGates.h"
#include "StandardBid.h"
#include "StandardShare.h"

namespace Applications
{
    namespace Permutations
    {
        //private members

        
        //public members
        HalfLogExGates::HalfLogExGates()
        {
            this->engine_=NULL;
        };
        HalfLogExGates::HalfLogExGates(SmcEngines::ShamirSharesEngine * engine)
        {
            this->engine_=engine;
        };
        
        Utils::List<Auctions::Bids::IExchangable> * HalfLogExGates::permute(Utils::List<Auctions::Bids::IExchangable> *list)
        {
          if(list->getLength()<=1)
          {
              Utils::List<Auctions::Bids::IExchangable> *  localList = new Utils::List<Auctions::Bids::IExchangable>(1);
              localList->add(list->get(0));
              return  localList;
          }
            int currentLenght = list->getLength()/2;
            Utils::List<Auctions::Bids::IExchangable> *left = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            Utils::List<Auctions::Bids::IExchangable> *right = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            for (int i=0; i<currentLenght; i++)
            {
                left->add(list->get(i));
                right->add(list->get(i+currentLenght));
            }
            
            Utils::List<Auctions::Bids::IExchangable> * newLeft =this->permute(left);
            Utils::List<Auctions::Bids::IExchangable> * newRight= this->permute(right);
            
            Utils::List<Auctions::Bids::IExchangable> * response= new  Utils::List<Auctions::Bids::IExchangable> (list->getLength());
            
            for (int i=0; i< currentLenght; i++)
            {
                Shares::StandardShare * bit =this->engine_->generateDagmardRandomBit();
                left->get(i)->secureExchange(bit, newLeft->get(i), newRight->get(i), this->engine_);
                
                //reconstruct vector
                response->set(i, newLeft->get(i));
                response->set(currentLenght+i, newRight->get(i));
                
                delete bit; 
            }
            
            delete  left;
            delete right;
            delete newLeft;
            delete newRight;
           
            return response;
        };
    };
};