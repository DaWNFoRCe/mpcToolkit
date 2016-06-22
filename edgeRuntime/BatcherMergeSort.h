//
//  BatcherMergeSort.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/30/15.
//
//

#ifndef __edgeRuntime__BatcherMergeSort__
#define __edgeRuntime__BatcherMergeSort__

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
        class BatcherMergeSort
        {
        private:
            SmcEngines::ShamirSharesEngine * engine_ = NULL;
        public:
            BatcherMergeSort();
            BatcherMergeSort(SmcEngines::ShamirSharesEngine * engine);
            Utils::List<Auctions::Bids::IExchangable> *permute(Utils::List<Auctions::Bids::IExchangable> *list);
            Utils::List<Auctions::Bids::IExchangable> *merge(Utils::List<Auctions::Bids::IExchangable> *list);
        };
    };
};
#endif /* defined(__edgeRuntime__BatcherMergeSort__) */
