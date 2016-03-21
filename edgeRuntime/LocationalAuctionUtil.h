//
//  LocationalAuctionUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/25/14.
//
//

#ifndef __edgeRuntime__LocationalAuctionUtil__
#define __edgeRuntime__LocationalAuctionUtil__

//Generic Headers
#include <stdio.h>
#include <string>

//Custom Headers
#include "StandardBid.h"
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"

namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            class LocationalAuctionUtil
            {
            public:
                
                static Utils::Matrix<Shares::StandardShare> * readNetwork(std::string filename, int playerId, int vertices);
                static Utils::List<Bids::StandardBid> * readBids(std::string filename, int playerId, int totalBids);
                
            };
        };
    };
};
#endif /* defined(__edgeRuntime__LocationalAuctionUtil__) */
