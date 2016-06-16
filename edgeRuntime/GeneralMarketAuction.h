//
//  GeneralMarketAuction.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/9/16.
//
//

#ifndef GeneralMarketAuction_h
#define GeneralMarketAuction_h
//Generic Headers
#include <stdio.h>

//Custom Headers
#include "ShamirSharesEngine.h"
#include "PushRelabel.h"
#include "GeneralMarketBid.h"
#include "StandardShare.h"
#include "AuctionResponse.h"

#include "List.h"
#include "Matrix.h"

namespace Applications
{
    namespace Auctions
    {
        using namespace Bids;
        class GeneralMarketAuction
        {
        private:
            
            
            Utils::List<GeneralMarketBid> * bids_=NULL;
            SmcEngines::ShamirSharesEngine * engine_=NULL;
            Utils::List<Shares::StandardShare> * suppliers_=NULL;
            Utils::List<GeneralMarketBid> * partition (Utils::List<GeneralMarketBid> * a, int * p);
            
        public:
            GeneralMarketAuction();
            GeneralMarketAuction(Utils::List<GeneralMarketBid> * bids, Utils::List<Shares::StandardShare> * suppliers, SmcEngines::ShamirSharesEngine * engine);
            Utils::List<GeneralMarketBid> * permuteBids(Utils::List<GeneralMarketBid> * bids);
            int quickSortBids(Utils::List<GeneralMarketBid> * bids);
            Utils::List<ReducedBid> * optimizeAuction();
            
        };
    };
};

#endif /* GeneralMarketAuction_h */
