//
//  BidsUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/5/14.
//
//

#ifndef __edgeRuntime__BidsUtil__
#define __edgeRuntime__BidsUtil__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "BidUtil.h"
#include "StandardBid.h"
#include "ReducedBid.h"
#include "List.h"

namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            class BidUtil
            {
            private:
                static int pointerExchange (Bids::StandardBid ** a, Bids::StandardBid ** b);
            public:
                static int exchangeBids(Bids::StandardBid * a, Bids::StandardBid *b);
                //Create interface Destroyable
                static int destroyList(Utils::List<Bids::StandardBid> *list);
                static int destroyList(Utils::List<Bids::ReducedBid> *list);
            };
        };
    };
};

#endif /* defined(__edgeRuntime__BidsUtil__) */
