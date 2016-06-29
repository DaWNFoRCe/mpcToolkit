//
//  GeneralAuctionResponse.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/17/16.
//
//

#ifndef GeneralAuctionResponse_h
#define GeneralAuctionResponse_h
//Custom Headers
#include "Matrix.h"
#include "List.h"
#include "ReducedBid.h"
#include "StandardShare.h"
namespace Applications
{
    namespace Auctions
    {
        namespace Response
        {
            class GeneralAuctionResponse
            {
            private:
                Utils::List<Shares::StandardShare> * suppliersCapacity_;
                Utils::List<Shares::StandardShare> * suppliersDemand_;
                Utils::List<Bids::ReducedBid> * bids_;
                Shares::StandardShare * clearance_volume_;
                Shares::StandardShare * clearance_price_;
            public:
                
                GeneralAuctionResponse();
                GeneralAuctionResponse(Shares::StandardShare * clearance_price ,Shares::StandardShare * clearance_volume,Utils::List<Shares::StandardShare> * suppliersDemand, Utils::List<Shares::StandardShare> * suppliersCapacity, Utils::List<Bids::ReducedBid> * bids);
                
                Utils::List<Bids::ReducedBid> * getBids();
                void setBids(Utils::List<Bids::ReducedBid> * bids);
                
                Utils::List<Shares::StandardShare> * getSuppliersDemand();
                void setSuppiersDemand(Utils::List<Shares::StandardShare> * suppliers);
                
                Utils::List<Shares::StandardShare> * getSuppliersCapacity();
                void setSuppiersCapacity(Utils::List<Shares::StandardShare> * suppliers);
                
                Shares::StandardShare * getClearanceVolume();
                void setClearanceVolume(Shares::StandardShare * clearanceVolume);
                
                Shares::StandardShare * getClearancePrice();
                void setClearancePrice(Shares::StandardShare * clearancePrice);

    
            };
        };
    };
};
#endif /* GeneralAuctionResponse_h */
