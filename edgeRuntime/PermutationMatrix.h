//
//  PermutationMatrix.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/8/15.
//
//

#ifndef __edgeRuntime__PermutationMatrix__
#define __edgeRuntime__PermutationMatrix__

#include "ShamirSharesEngine.h"
#include "List.h"
#include "StandardPlayer.h"
#include "StandardShare.h"

#include <stdio.h>
namespace Applications
{
    namespace Permutations
    {
        class PermutationMatrix
        {
        private:
            static Utils::List<Shares::StandardShare> * sharesList;
            
        public:
            static Shares::StandardShare * multiThreading(Utils::List<Players::StandardPlayer> *players, int player);
            
            static void * addition (void *e);
            
            
        };
    };
};

#endif /* defined(__edgeRuntime__PermutationMatrix__) */
