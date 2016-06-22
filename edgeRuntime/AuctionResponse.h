//
//  AuctionResponse.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/21/14.
//
//
//Generic Headers

//Custom Headers
#include "Matrix.h"
#include "List.h"
#include "ReducedBid.h"
#include "StandardShare.h"

#ifndef __edgeRuntime__AuctionResponse__
#define __edgeRuntime__AuctionResponse__

#include <stdio.h>
namespace Applications
{
    namespace Auctions
    {
        namespace Response
        {
            class AuctionResponse
            {
            private:
                Utils::Matrix<Shares::StandardShare> * flow_;
                Utils::List<Bids::ReducedBid> * bids_;
            public:
                
                AuctionResponse();
                AuctionResponse(Utils::Matrix<Shares::StandardShare> * flow , Utils::List<Bids::ReducedBid> * bids);
                Utils::Matrix<Shares::StandardShare> * getFlow();
                void setFlow(Utils::Matrix<Shares::StandardShare> * flow);

                Utils::List<Bids::ReducedBid> * getBids();
                void setBids(Utils::List<Bids::ReducedBid> * bids);
                
            };
        };
    };
};

#endif /* defined(__edgeRuntime__AuctionResponse__) */
