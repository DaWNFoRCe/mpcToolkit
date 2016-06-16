//
//  GeneralMarketBid.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/14/16.
//
//

#include <stdio.h>

//Custom Headers
#include "StandardShare.h"
#include "IExchangable.h"
#include "ShamirSharesEngine.h"
#include "GeneralMarketAuction.h"

namespace Applications
{
    namespace Auctions
    {
        namespace Bids
        {
            using namespace Shares;
            GeneralMarketBid::GeneralMarketBid()
            {
                this->id_= NULL;
                this->supplier_=NULL;
                this->price_=NULL;
                this->quantity_=NULL;
                this->demand_=NULL;
                
            };
            
            //initialization constructor
            GeneralMarketBid::GeneralMarketBid(Shares::StandardShare *id_,  Utils::List<Shares::StandardShare> * supplier, Shares::StandardShare * price, Shares::StandardShare * quantity, Shares::StandardShare * demand)
            {
                this->id_= id_;
                this->demand_=demand;
                this->price_=price;
                this->quantity_=quantity;
                this->supplier_=supplier;
                
            };
            
            
            
            //get player id
            StandardShare * GeneralMarketBid::getId()
            {
                return this->id_;
            };
            
            //set shared id
            void GeneralMarketBid::setId(StandardShare * id_)
            {
                this->id_=id_;
            };
            
            //get player market
            Utils::List<Shares::StandardShare> * GeneralMarketBid::getSupplier()
            {
                return this->supplier_;
            };
            
            //set shared market
            void GeneralMarketBid::setSupplier(Utils::List<Shares::StandardShare> * supplier)
            {
                this->supplier_=supplier;
            };
            
            //get player price
            StandardShare * GeneralMarketBid::getPrice()
            {
                return this->price_;
            };
            
            //set shared price
            void GeneralMarketBid::setPrice(StandardShare * price)
            {
                this->price_=price;
            };
            
            //get player quantity
            StandardShare * GeneralMarketBid::getQuantity()
            {
                return this->quantity_;
            };
            
            //set shared quantity
            void GeneralMarketBid::setQuantity(StandardShare * quantity)
            {
                this->quantity_=quantity;
            };
            
            //get player quantity
            StandardShare * GeneralMarketBid::getDemand()
            {
                return this->demand_;
            };
            
            //set shared quantity
            void GeneralMarketBid::setDemand(StandardShare * demand)
            {
                this->demand_=demand;
            };
            
            
            
            GeneralMarketBid *GeneralMarketBid::clone()
            {
                return new GeneralMarketBid(this->id_->clone(), this->supplier_->clone(), this->price_->clone(), this->quantity_->clone(), this->demand_->clone());
            };
            
            int GeneralMarketBid::secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine *engine)
            {
                GeneralMarketBid * a_l= (GeneralMarketBid *)a;
                GeneralMarketBid * b_l= (GeneralMarketBid *)b;
                
                Shares::StandardShare * difference;
                
                //exchange Id
                difference = engine->multiplyTo(engine->substract(a_l->getId(), b_l->getId()), auth);
                a_l->setId(engine->substractTo(a_l->getId(), difference));
                b_l->setId(engine->addTo(b_l->getId(), difference));
                delete difference;
                
                
                //exchange Supplier
                for (int i=0; i<a_l->getSupplier()->getLength(); i++)
                {
                    StandardShare *a_l_value =a_l->getSupplier()->get(i);
                    StandardShare *b_l_value =b_l->getSupplier()->get(i);
                    difference = engine->multiplyTo(engine->substract(a_l_value, b_l_value), auth);
                    a_l->getSupplier()->set(i, engine->substractTo(a_l_value, difference));
                    b_l->getSupplier()->set(i, engine->substractTo(b_l_value, difference));
                    delete difference;
                }
                
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
                
                //exchange Demand
                difference = engine->multiplyTo(engine->substract(a_l->getDemand(), b_l->getDemand()), auth);
                a_l->setPrice(engine->substractTo(a_l->getDemand(), difference));
                b_l->setPrice(engine->addTo(b_l->getDemand(), difference));
                delete difference;

                
                return 1;
            };
            
            
            int GeneralMarketBid::unsafeExchange(IExchangable* a, IExchangable *b)
            {
                GeneralMarketBid * a_l = (GeneralMarketBid*)a;
                GeneralMarketBid * b_l = (GeneralMarketBid*)b;
                Shares::StandardShare * aux= a_l->getId();
                a_l->setId(b_l->getId());
                b_l->setId(aux);
                
                aux= a_l->getDemand();
                a_l->setDemand(b_l->getDemand());
                b_l->setDemand(aux);
                
                aux= a_l->getPrice();
                a_l->setPrice(b_l->getPrice());
                b_l->setPrice(aux);
                
                aux= a_l->getQuantity();
                a_l->setQuantity(b_l->getQuantity());
                b_l->setQuantity(aux);
                
                Utils::List<Shares::StandardShare> * aux_list= a_l->getSupplier();
                a_l->setSupplier(b_l->getSupplier());
                b_l->setSupplier(aux_list);
                
                return 1;
            };
            
            GeneralMarketBid::~GeneralMarketBid()
            {
                delete this->id_;
                delete this->supplier_;
                delete this->quantity_;
                delete this->price_;
                
                
            };
        };
    };
};