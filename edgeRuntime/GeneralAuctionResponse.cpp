//
//  GeneralAuctionResponse.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/17/16.
//
//

#include "GeneralAuctionResponse.h"
namespace Applications
{
    namespace Auctions
    {
        namespace Response
        {
            GeneralAuctionResponse::GeneralAuctionResponse()
            {
                this->suppliersCapacity_=NULL;
                this->suppliersDemand_=NULL;
                this->bids_=NULL;
                this->clearance_volume_=NULL;
                this->clearance_price_=NULL;
                return;
            };
            
            GeneralAuctionResponse::GeneralAuctionResponse(Shares::StandardShare * clearance_price ,Shares::StandardShare * clearance_volume,Utils::List<Shares::StandardShare> * suppliersDemand, Utils::List<Shares::StandardShare> * suppliersCapacity, Utils::List<Bids::ReducedBid> * bids)
            {
                this->suppliersCapacity_=suppliersCapacity;
                this->suppliersDemand_=suppliersDemand;
                this->bids_=bids;
                this->clearance_volume_=clearance_volume;
                this->clearance_price_=clearance_price;
                return;
            };
            
            
            Utils::List<Bids::ReducedBid> * GeneralAuctionResponse::getBids()
            {
                return this->bids_;
            };
            
            void GeneralAuctionResponse::setBids(Utils::List<Bids::ReducedBid> * bids)
            {
                this->bids_=bids;
            };
            
            Utils::List<Shares::StandardShare> * GeneralAuctionResponse::getSuppliersDemand()
            {
                return this->suppliersDemand_;
            };
            
            void GeneralAuctionResponse::setSuppiersDemand(Utils::List<Shares::StandardShare> * suppliers)
            {
                this->suppliersDemand_=suppliers;
            };
            
            Utils::List<Shares::StandardShare> * GeneralAuctionResponse::getSuppliersCapacity()
            {
                return this->suppliersCapacity_;
            };
            
            void GeneralAuctionResponse::setSuppiersCapacity(Utils::List<Shares::StandardShare> * suppliers)
            {
                this->suppliersCapacity_=suppliers;
            };
            
            Shares::StandardShare * GeneralAuctionResponse::getClearanceVolume()
            {
                return this->clearance_volume_;
            };
            
            void GeneralAuctionResponse::setClearanceVolume(Shares::StandardShare * clearanceVolume)
            {
                this->clearance_volume_=clearanceVolume;
            };
            
            Shares::StandardShare * GeneralAuctionResponse::getClearancePrice()
            {
                return this->clearance_price_;
            };
            
            void GeneralAuctionResponse::setClearancePrice(Shares::StandardShare * clearancePrice)
            {
                this->clearance_price_=clearancePrice;
            };

        };
        
    };
};