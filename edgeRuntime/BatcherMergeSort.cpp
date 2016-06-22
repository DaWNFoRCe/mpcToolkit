//
//  BatcherMergeSort.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/30/15.
//
//

#include "BatcherMergeSort.h"
#include "StandardBid.h"
#include "StandardShare.h"

namespace Applications
{
    namespace Permutations
    {
        //private members
        
        
        //public members
        BatcherMergeSort::BatcherMergeSort()
        {
            this->engine_=NULL;
        };
        BatcherMergeSort::BatcherMergeSort(SmcEngines::ShamirSharesEngine * engine)
        {
            this->engine_=engine;
        };
        
        Utils::List<Auctions::Bids::IExchangable> * BatcherMergeSort::permute(Utils::List<Auctions::Bids::IExchangable> *list)
        {
            if(list->getLength()<=1)
            {
                //Utils::List<Auctions::Bids::IExchangable> *  localList = new Utils::List<Auctions::Bids::IExchangable>(1);
                //localList->add(list->get(0));
                //return  localList;
                return list;
            }
            int currentLenght = list->getLength()/2;
            Utils::List<Auctions::Bids::IExchangable> *left = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            Utils::List<Auctions::Bids::IExchangable> *right = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            
            for(int i=0; i<currentLenght; i++)
            {
                left->add(list->get(i));
                right->add(list->get(currentLenght+i));
            }
            
            Utils::List<Auctions::Bids::IExchangable> * r_left= this->permute(left);
            Utils::List<Auctions::Bids::IExchangable> *r_right=this->permute(right);
//            Utils::List<Auctions::Bids::IExchangable> *response =new                 Utils::List<Auctions::Bids::IExchangable>(list->getLength());
//            for (int i =0; i< currentLenght; i++)
//            {
//                response->set(i,left->get(i));
//                response->set(i+currentLenght, right->get(i));
                
//            };
            Utils::List<Auctions::Bids::IExchangable> *r_response= this->merge(list);
            delete left;
            delete right;
            //delete r_left;
            //delete r_right;
            //delete response;
            
            return r_response;
            
            
        };
        
        Utils::List<Auctions::Bids::IExchangable> * BatcherMergeSort::merge(Utils::List<Auctions::Bids::IExchangable> *list)
        {
            
            if(list->getLength()<=1)
            {
              //  Utils::List<Auctions::Bids::IExchangable> *  localList = new Utils::List<Auctions::Bids::IExchangable>(1);
              //  localList->add(list->get(0));
              //  return  localList;
                return list;
            }
           
            int currentLenght = list->getLength()/2;
            Utils::List<Auctions::Bids::IExchangable> *even = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            Utils::List<Auctions::Bids::IExchangable> *odd = new Utils::List<Auctions::Bids::IExchangable>(currentLenght);
            
            for(int i=0; i<list->getLength(); i=i+2)
            {
                even->add(list->get(i));
                odd->add(list->get(i+1));
            }
            
            Utils::List<Auctions::Bids::IExchangable> * r_even= this->merge(even);
            Utils::List<Auctions::Bids::IExchangable> *r_odd=this->merge(odd);
            //Utils::List<Auctions::Bids::IExchangable> *response =new Utils::List<Auctions::Bids::IExchangable>(list->getLength());
            
            for (int i =0; i< currentLenght; i++)
            {
                Auctions::Bids::IExchangable *left = r_even->get(i);
                Auctions::Bids::IExchangable *right = r_odd->get(i);
                Shares::StandardShare *bit =this->engine_->generateDagmardRandomBit();
                
                left->secureExchange(bit, left, right, this->engine_);
                
                //response->set(i, left);
                //response->set(i+currentLenght, right);
                delete bit;
            }
            
            delete even;
            delete odd;
            //delete  r_even;
            //delete r_odd;
            
            return list;
            //return response;
        };
    };
};