//
//  PermutationMatrix.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/8/15.
//
//

#include <iostream>
#include <unistd.h>

#include "ShamirSharesEngine.h"
#include "List.h"
#include "StandardPlayer.h"
#include "StandardShare.h"

#include "PermutationMatrix.h"

namespace Applications
{
    namespace Permutations
    {
        Utils::List<Shares::StandardShare> * PermutationMatrix::sharesList =NULL;
        
        Shares::StandardShare * PermutationMatrix::multiThreading(Utils::List<Players::StandardPlayer> *players, int player)
        {
            PermutationMatrix::sharesList = new Utils::List<Shares::StandardShare>();
            
            SmcEngines::ShamirSharesEngine *engine = new SmcEngines::ShamirSharesEngine(players->get(player-1),players,true);
            Utils::List<Players::StandardPlayer> * players2 = new Utils::List<Players::StandardPlayer> (3);
            
            for (int i =0; i<3; i++)
            {
                players2->add(new Players::StandardPlayer( players->get(i)->getPlayer(), players->get(i)->getPort()+1000, players->get(i)->getIpAdress()));
            }
            
            SmcEngines::ShamirSharesEngine *engine2 = new SmcEngines::ShamirSharesEngine(players->get(player-1),players2,true);

            pthread_t threads[2];
            int rc;
            rc =
            pthread_create(&threads[0], NULL,
                           PermutationMatrix::addition, (void *)engine);
            
        };
        
        void  * PermutationMatrix::addition (void *e)
        {
            
            SmcEngines::ShamirSharesEngine *engine = (SmcEngines::ShamirSharesEngine *)e;
            int port =engine->getPlayer()->getPort();
            Shares::StandardShare * share=NULL;
            if (port>3999) {
                share = engine->buildShare(5);
            }
            else
            {
               share = engine->buildShare(7);
            }
                
            PermutationMatrix::sharesList->set(0, engine->add( share, share));
            std::cout<<" Thread: "<< engine->presentShare(share)<<"\n";
            return NULL;
        };
            

    };
};