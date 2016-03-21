//
//  BidsUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/5/14.
//
//
//Generic Headers

//Custom Headers
#include "BidUtil.h"
#include "ShareUtil.h"
#include "StandardShare.h"
#include "ReducedBid.h"
#include "List.h"

namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            using namespace ::Utilities;
            int BidUtil::pointerExchange (Bids::StandardBid ** a, Bids::StandardBid ** b)
            {
                //Bids::StandardBid * aux= *a;
                //*a=*b;
                //*b=aux;
                //std::swap((*a)->getId(), *b);
                return 1;
                
            };
            
            int BidUtil::exchangeBids(Bids::StandardBid * a, Bids::StandardBid *b)
            {
                //BidUtil::pointerExchange(&a, &b);
                Shares::StandardShare * aux= a->getId();
                a->setId(b->getId());
                b->setId(aux);
                
                aux= a->getMarket();
                a->setMarket(b->getMarket());
                b->setMarket(aux);
                
                aux= a->getPrice();
                a->setPrice(b->getPrice());
                b->setPrice(aux);
                
                aux= a->getQuantity();
                a->setQuantity(b->getQuantity());
                b->setQuantity(aux);
                
                return 1;
            };
            
            int BidUtil::destroyList(Utils::List<Bids::StandardBid> *list)
            {
                for (int i =0; i< list->getLength(); i++)
                {
                    Bids::StandardBid *aux = list->get(i);
                    list->set(i, NULL);
                    delete aux;
                }
                delete list;
                return 1;
            };
            
            int BidUtil::destroyList(Utils::List<Bids::ReducedBid> *list)
            {
                for (int i =0; i< list->getLength(); i++)
                {
                    Bids::ReducedBid *aux = list->get(i);
                    list->set(i, NULL);
                    delete aux;
                }
                delete list;
                return 1;
            };
        };
    };
};