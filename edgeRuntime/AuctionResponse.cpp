//
//  AuctionResponse.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/21/14.
//
//
//Generic Headers

//Custom Headers
#include "AuctionResponse.h"
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
            //default constructor
            AuctionResponse::AuctionResponse()
            {
                this->flow_=NULL;
                this->bids_=NULL;
                return;
            };
            
            //initializer constructor
            AuctionResponse::AuctionResponse(Utils::Matrix<Shares::StandardShare> * flow , Utils::List<Bids::ReducedBid> * bids)
            {
                this->flow_=flow;
                this->bids_=bids;
                return;
            };
            
            //simple getter
            Utils::Matrix<Shares::StandardShare> * AuctionResponse::getFlow()
            {
                return this->flow_;
            };
            
            //simple setter
            void AuctionResponse::setFlow(Utils::Matrix<Shares::StandardShare> * flow)
            {
                this->flow_=flow;
            };
            
            //simple getter
            Utils::List<Bids::ReducedBid> * AuctionResponse::getBids()
            {
                return bids_;
            };
            
            //simple setter
            void AuctionResponse::setBids(Utils::List<Bids::ReducedBid> * bids)
            {
                this->bids_=bids;
            };

    };
};
};

