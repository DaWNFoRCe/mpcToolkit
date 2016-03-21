//
//  IExchangable.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/20/15.
//
//

#ifndef __edgeRuntime__IExchangable__
#define __edgeRuntime__IExchangable__

//Generic Header
#include <stdio.h>

//Custom Header
#include "StandardShare.h"
#include "ShamirSharesEngine.h"


namespace Applications
{
    namespace Auctions
    {
        namespace Bids
        {
            class IExchangable
            {
            public:
                IExchangable(){};
                
                virtual int secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine *engine)=0;
                
                virtual ~IExchangable(){};
            };
        };
    };
};
#endif /* defined(__edgeRuntime__IExchangable__) */
