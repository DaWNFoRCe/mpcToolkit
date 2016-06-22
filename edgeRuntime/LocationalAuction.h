//
//  LocationalAuction.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 10/30/14.
//
//

#ifndef __edgeRuntime__LocationalAuction__
#define __edgeRuntime__LocationalAuction__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "ShamirSharesEngine.h"
#include "PushRelabel.h"
#include "StandardBid.h"
#include "StandardShare.h"
#include "AuctionResponse.h"

#include "List.h"
#include "Matrix.h"

namespace Applications
{
    namespace Auctions
    {
        using namespace Bids;
        class LocationalAuction
        {
        private:
            Utils::Matrix<Shares::StandardShare> * network_=NULL;
            Utils::List<Shares::StandardShare> * markets_=NULL;
            Utils::List<StandardBid> * bids_=NULL;
            SmcEngines::ShamirSharesEngine * engine_=NULL;
            Utils::List<StandardBid> * partition (Utils::List<StandardBid> * a, int * p);
            Utils::Matrix<Shares::StandardShare> * findResidualNetwork( Utils::Matrix<Shares::StandardShare> * g,            Utils::Matrix<Shares::StandardShare> *n);
            int source_=0;
            int sink_=0;
            
        public:
            LocationalAuction();
            LocationalAuction(Utils::List<StandardBid> * bids, Utils::Matrix<Shares::StandardShare> * network,             Utils::List<Shares::StandardShare> * markets, SmcEngines::ShamirSharesEngine * engine);
            Utils::List<StandardBid> * permuteBids(Utils::List<StandardBid> * bids);
            int quickSortBids(Utils::List<StandardBid> * bids);
            Utils::Matrix<Shares::StandardShare> * maxFlowNetwork(Utils::Matrix<Shares::StandardShare> * network);
            Response::AuctionResponse * optimizeAuction();
            
        };
    };
};

#endif /* defined(__edgeRuntime__LocationalAuction__) */
