//
//  GeneralMarketAuctionUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/15/16.
//
//

#ifndef GeneralMarketAuctionUtil_h
#define GeneralMarketAuctionUtil_h
//Generic Headers
#include <stdio.h>
#include <string>

//Custom Headers
#include "GeneralMarketBid.h"
#include "StandardShare.h"
#include "List.h"
namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            class GeneralMarketAuctionUtil
            {
            public:
                

                static Utils::List<Bids::GeneralMarketBid> * readBids(std::string filename, int playerId, int totalBids, int suppliers);
                
            };
        };
    };
};

#endif /* GeneralMarketAuctionUtil_h */
