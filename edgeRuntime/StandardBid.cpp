//
//  StandardBid.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 10/30/14.
//
//
//Generic Headers

//Custom Headers
#include "StandardBid.h"
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
            StandardBid::StandardBid()
            {
                this->id_= NULL;
                this->market_=NULL;
                this->price_=NULL;
                this->quantity_=NULL;

            };
            
            //initialization constructor
            StandardBid::StandardBid(Shares::StandardShare *id_, Shares::StandardShare * market, Shares::StandardShare * price, Shares::StandardShare * quantity)
            {
                this->id_= id_;
                this->market_=market;
                this->price_=price;
                this->quantity_=quantity;

            };
            
 
            
            //get player id
            StandardShare * StandardBid::getId()
            {
                return this->id_;
            };
            
            //set shared id
            void StandardBid::setId(StandardShare * id_)
            {
                this->id_=id_;
            };

            //get player market
            StandardShare * StandardBid::getMarket()
            {
                return this->market_;
            };
            
            //set shared market
            void StandardBid::setMarket(StandardShare * market)
            {
                this->market_=market;
            };
            
            //get player price
            StandardShare * StandardBid::getPrice()
            {
                return this->price_;
            };
            
            //set shared price
            void StandardBid::setPrice(StandardShare * price)
            {
                this->price_=price;
            };
            
            //get player quantity
            StandardShare * StandardBid::getQuantity()
            {
                return this->quantity_;
            };
            
            //set shared quantity
            void StandardBid::setQuantity(StandardShare * quantity)
            {
                this->quantity_=quantity;
            };
            

                        
            StandardBid *StandardBid::clone()
            {
                return new StandardBid(this->id_->clone(), this->market_->clone(), this->price_->clone(), this->quantity_->clone());
            };
            
            
            
            int StandardBid::secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine *engine)
            {
                StandardBid * a_l= (StandardBid *)a;
                StandardBid * b_l= (StandardBid *)b;

                Shares::StandardShare * difference;
                
                //exchange Id
                difference = engine->multiplyTo(engine->substract(a_l->getId(), b_l->getId()), auth);
                a_l->setId(engine->substractTo(a_l->getId(), difference));
                b_l->setId(engine->addTo(b_l->getId(), difference));
                delete difference;
                
                
                //exchange Market
                difference = engine->multiplyTo(engine->substract(a_l->getMarket(), b_l->getMarket()), auth);
                a_l->setMarket(engine->substractTo(a_l->getMarket(), difference));
                b_l->setMarket(engine->addTo(b_l->getMarket(), difference));
                delete difference;
                

                //exchange Quantity
                difference = engine->multiplyTo(engine->substract(a_l->getQuantity(), b_l->getQuantity()), auth);
                a_l->setQuantity(engine->substractTo(a_l->getQuantity(), difference));
                b_l->setQuantity(engine->addTo(b_l->getQuantity(), difference));
                delete difference;
                
                //exchange Price
                difference = engine->multiplyTo(engine->substract(a_l->getPrice(), b_l->getPrice()), auth);
                a_l->setPrice(engine->substractTo(a_l->getPrice(), difference));
                b_l->setPrice(engine->addTo(b_l->getPrice(), difference));
                delete difference;
                
                return 1;
            };
            
            int StandardBid::unsafeExchange( IExchangable* a, IExchangable *b)
            {
                return 1;
            };
            
            StandardBid::~StandardBid()
            {
                delete this->id_;
                delete this->market_;
                delete this->quantity_;
                delete this->price_;

                
            };
            

        };
    };
};
