//
//  ReducedBid.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 10/30/14.
//
//
//Generic Headers

//Custom Headers
#include "ReducedBid.h"
#include "StandardShare.h"
#include "IExchangable.h"
#include "ShamirSharesEngine.h"


namespace Applications
{
    namespace Auctions
    {
        namespace Bids
        {
            using namespace Shares;
            
            //standrad constructor to NULL
            ReducedBid::ReducedBid()
            {
                this->id_= NULL;
                this->price_=NULL;
                this->quantity_=NULL;

            };
            
            //initialization constructor
            ReducedBid::ReducedBid(Shares::StandardShare *id_, Shares::StandardShare * price, Shares::StandardShare * quantity)
            {
                this->id_= id_;
                this->price_=price;
                this->quantity_=quantity;

            };
            
            //get player id
            StandardShare * ReducedBid::getId()
            {
                return this->id_;
            };
            
            //set shared id
            void ReducedBid::setId(StandardShare * id_)
            {
                this->id_=id_;
            };
            
       
            //get player price
            StandardShare * ReducedBid::getPrice()
            {
                return this->price_;
            };
            
            //set shared price
            void ReducedBid::setPrice(StandardShare * price)
            {
                this->price_=price;
            };
            
            //get player quantity
            StandardShare * ReducedBid::getQuantity()
            {
                return this->quantity_;
            };
            
            //set shared quantity
            void ReducedBid::setQuantity(StandardShare * quantity)
            {
                this->quantity_=quantity;
            };
            
    
            
            ReducedBid *ReducedBid::clone()
            {
                return new ReducedBid(this->id_->clone(), this->price_->clone(), this->quantity_->clone());
            };
            
            int ReducedBid::secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine *engine)
            {
                ReducedBid * a_l= (ReducedBid *)a;
                ReducedBid * b_l= (ReducedBid *)b;
                
                Shares::StandardShare * difference;
                
                //exchange id
                difference = engine->multiplyTo(engine->substract(a_l->getId(), b_l->getId()), auth);
                a_l->setId(engine->substractTo(a_l->getId(), difference));
                b_l->setId(engine->addTo(b_l->getId(), difference));
                delete difference;
                
                //exchange quantity
                difference = engine->multiplyTo(engine->substract(a_l->getQuantity(), b_l->getQuantity()), auth);
                a_l->setQuantity(engine->substractTo(a_l->getQuantity(), difference));
                b_l->setQuantity(engine->addTo(b_l->getQuantity(), difference));
                delete difference;
                
                //exchange price
                difference = engine->multiplyTo(engine->substract(a_l->getPrice(), b_l->getPrice()), auth);
                a_l->setPrice(engine->substractTo(a_l->getPrice(), difference));
                b_l->setPrice(engine->addTo(b_l->getPrice(), difference));
                delete difference;

                return 1;
            };
            
            ReducedBid::~ReducedBid()
            {
                delete this->id_;
                delete this->quantity_;
                delete this->price_;

                
            };
            
            
        };
    };
};
