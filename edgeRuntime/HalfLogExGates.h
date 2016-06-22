//
//  HalfLogExGates.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/21/15.
//
//

#ifndef __edgeRuntime__HalfLogExGates__
#define __edgeRuntime__HalfLogExGates__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "ShamirSharesEngine.h"
#include "IExchangable.h"

#include "List.h"


namespace Applications
{
    namespace Permutations
    {
        class HalfLogExGates
        {
        private:
             SmcEngines::ShamirSharesEngine * engine_ = NULL;
        public:
            HalfLogExGates();
            HalfLogExGates(SmcEngines::ShamirSharesEngine * engine);
            Utils::List<Auctions::Bids::IExchangable> *permute(Utils::List<Auctions::Bids::IExchangable> *list);
            
        };
    };
};

#endif /* defined(__edgeRuntime__HalfLogExGates__) */
