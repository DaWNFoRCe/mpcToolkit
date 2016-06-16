
//
//  main.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 31/10/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//


//Generic Headers
#include <iostream>
#include <String>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>

#include <mach/mach.h>
#include <mach/mach_time.h>

//Library Headers
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>
#include <NTL/ZZ_pX.h>
#include <NTL/ZZ_pE.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>

//Custom Headers
#include "StandardShare.h"
#include "Constants.h"
#include "StandardPlayer.h"
#include "ShamirSharesEngine.h"
#include "SocketPool.h"
#include "EngineBuffers.h"
#include "GeneralMarketBid.h"
#include "ReducedBid.h"
#include "AuctionResponse.h"
#include "GeneralMarketAuction.h"
#include "PushRelabel.h"
#include "BellmanFord.h"
#include "List.h"
#include "Matrix.h"
#include "MathUtil.h"
#include "ShareUtil.h"
#include "GeneralMarketAuctionUtil.h"
#include "PermutationMatrix.h"
#include "ShareUtil.h"

NTL_CLIENT
using namespace Applications::Auctions;

/**
 @mainpage Library code  "Edge Runtime".
 *Is a collection of libraries that implements a vast array of secure functionality. From comparison Methods and equality tests \cite Toft13 and \cite DKNT06 to the calculus of the inverse and xor gates.
 *
 *The framework relies on a synchronic communicational framework that parallelized communication services to implement Shamir secret sharing schemes, and GMW multiplications. Hoghly optimized for the 3 players case.
 *
 *Our security model protects data against non-adaptive honest-but-curious adversaries, and we assume secure transmission channels.
 * To start the main class contains an example on how it can be used.
 */

//Main Example
int main(int argc, const char ** argv)
{
    std::cout << "******************** Bellman Ford in C++ ********************"<<"\n";
    cout<<"#############################################################"<<endl;
	cout<<"#                    _                                      #"<<endl;  
	cout<<"#                  -=\\`\\                                    #"<<endl;  
	cout<<"#              |\\ ____\\_\\__                                 #"<<endl;  
	cout<<"#            -=\\c`\"\"\"\"\"\"\" \"`)                               #"<<endl;  
	cout<<"#               `~~~~~/ /~~`\                                #"<<endl;  
	cout<<"#                 -==/ /                                    #"<<endl;  
	cout<<"#                   '-'                                     #"<<endl;  
	cout<<"#                  _  _                                     #"<<endl;  
	cout<<"#                 ( `   )_                                  #"<<endl;  
	cout<<"#                (    )    `)                               #"<<endl;  
	cout<<"#              (_   (_ .  _) _)                             #"<<endl;  
	cout<<"#                                             _             #"<<endl;  
	cout<<"#                                            (  )           #"<<endl;  
	cout<<"#             _ .                         ( `  ) . )        #"<<endl;  
	cout<<"#           (  _ )_                      (_, _(  ,_)_)      #"<<endl;  
	cout<<"#         (_  _(_ ,)                                        #"<<endl;  
	cout<<"#############################################################"<<endl;
    std::cout << "brought you by: CORE -->{*.-}"<<"\n";
    std::cout << "Welcome to the GIT World Arithmetic Modulus!! Comp with 2 Local" <<"\n";
    

    Utils::List<Players::StandardPlayer> *list = new Utils::List<Players::StandardPlayer>(3);


    int player;
    int secret;
    if (argc==1) {
        
         player = atoi("1");
         secret = atoi("5");
    }else {
        
        player = atoi(argv[1]);
        secret = atoi(argv[2]);
    }
    int localHome[]= {127,0,0,1};
    int localAddress[]= {127,0,0,1};
    int externalAddress[]={127,0,0,1}; //{192,168,1,5};//{130,104,59,120};//{130,104,59,120};
    //int localAddress[]= {127,0,0,1};
    std::cout<< "The current player is: "<< player << " and his secret is: "<< secret<<"\n";
    
    Players::StandardPlayer * p1=new Players::StandardPlayer(1,3001,externalAddress);
    Players::StandardPlayer * p2=new Players::StandardPlayer(2,3003,externalAddress);
    
    Players::StandardPlayer * p3=new Players::StandardPlayer(3,3002,localAddress);
    
    list->add( p1);
    list->add( p2);
    list->add( p3);
    Pools::SocketPool::configureByDefault(list->get(player-1));
    std::cout<< "the exit port is:" << list->get(player-1)->getPort()<<"\n";
    //Experimentation with Multithreading TODO: Migrate operation counter
    //Applications::Permutations::PermutationMatrix::multiThreading(list, player);
    SmcEngines::ShamirSharesEngine *engine = new SmcEngines::ShamirSharesEngine(list->get(player-1),list,true);
    Utils::List<Shares::StandardShare> *shares;
    
    
    //getchar();
    //first value sharing
    shares=engine->shareValue(secret);


    
    //Algorithm variable declaration
    Shares::StandardShare * prod=NULL;

    

    //markets


   
    //bids
    int totalBids=20;
    int suppliers =10;
    Utils::List<Bids::ReducedBid> *rbids;
    Utils::List<Bids::GeneralMarketBid> * bids; //= new     Utils::List<Bids::StandardBid>();
    char* filename;
    asprintf(&filename, "%s%i%s", "bids-", player,".txt");
    bids =Applications::Auctions::Utilities::GeneralMarketAuctionUtil::readBids(filename, player, totalBids, suppliers);
    //bids->add(new Bids::StandardBid(engine->buildShare(0), markets->get(1)->clone(), engine->buildShare(50), engine->buildShare(10)));
    //bids->add(new Bids::StandardBid(engine->buildShare(1), markets->get(0)->clone(), engine->buildShare(30), engine->buildShare(10)));
    //bids->add(new Bids::StandardBid(engine->buildShare(2), markets->get(0)->clone(), engine->buildShare(20), engine->buildShare(15)));
    
    
  
    for (int j=0; j<1; j++)
    {
        //time measurement initialization
 
       // Shares::StandardShare * aux = engine->buildShare(0);
       // Shares::StandardShare * aux_m = engine->buildShare(0);
        //algorithm process
        
        //std::cout<<"Comparitions: \n"<<engine->totalCom<<"\n";
        double tInitial=  mach_absolute_time();

//        for (int i=4; i<=256; i=i*2)
        for (int i=1; i<=1; i++)
        {
            
            GeneralMarketAuction * auction = new GeneralMarketAuction(bids, bids->get(0)->getSupplier(),engine);
            //prod= engine->lessEqualThanCatrinaModShares(shares->get(0), shares->get(1));
            rbids=  auction->optimizeAuction();
            for(int i=0;i<rbids->getLength();i++)
            {
                std::cout<<"position: "<<i+1<<" bid: " << engine->presentShare(rbids->get(i)->getId())<<" price: "<<engine->presentShare(rbids->get(i)->getPrice()) << " volume: " <<engine->presentShare(rbids->get(i)->getQuantity()) <<"\n";
            }
            prod = shares->get(1);
        /*

            std::cout<<"result: "<< engine->presentShare(prod) <<"\n";
            Response::AuctionResponse * auctionResponse= auction->optimizeAuction();
            rbids = auctionResponse->getBids();
            prod = engine->buildShare(0);
            std::cout<<"Start Share Presentation: \n";
            for(int k=0; k< rbids->getLength();k++)
            {
                long used =engine->presentShare(rbids->get(k)->getQuantity());
                std::cout<<"op: "<< engine->presentShare(rbids->get(k)->getId()) <<" Usage: "<< used  <<" Price: "<< engine->presentShare(rbids->get(k)->getPrice()) <<"\n";
                if(used!=0)
                {
            //::Utilities::ShareUtil::destroyList(engine->obtainMostSignificativeBit(bits));
            //prod =engine->bitwiseLessThanEqualSecureScalar(bits,0);

            //prod= engine->lessEqualThanCatrinaModShares(aux, aux_m);
                   prod = engine->addTo(prod,engine->multiply(rbids->get(k)->getPrice(),used));
                }
            }

          */


          
                      }
            double tFinal =mach_absolute_time();
            
            
            static double timeConvert = 0.0;
            if ( timeConvert == 0.0 )
            {
                mach_timebase_info_data_t timeBase;
                (void)mach_timebase_info( &timeBase );
                timeConvert = (double)timeBase.numer /
                (double)timeBase.denom /
                1000000000.0;
            }
            
            std::cout <<(tFinal-tInitial)*timeConvert <<"\n";
            std::cout <<engine->getTime() * timeConvert <<"\n";

            //std::cout<<"Comparitions: \n"<<engine->totalCom<<"\n";
            std::cout <<Buffers::EngineBuffers::operationCounter_<<"\n";
            
        }
    
        //time measurement process
        //gettimeofday(&tFinal, NULL);
        //int tmElp=(tFinal.tv_sec - tInitial.tv_sec) * 1000.0;
        //tmElp += (tFinal.tv_usec - tInitial.tv_usec) / 1000.0;
        //std::cout<<"total time elapsed: " <<tmElp <<"\n";
  
    
    //Process to Open Shares
    Utils::List<Shares::StandardShare> * localOpenShares= new  Utils::List<Shares::StandardShare>(1);
    Utils::List<Utils::List<Shares::StandardShare> >* openShares=NULL;
    std::vector<long> response;
    localOpenShares->add(prod);//*(totalRan)
    //std::cout<<"values: "<<greatThanToft->getValue()<<" "<<greatThanToft->getOperationId()<<" "<<greatThanToft->getPlayerId()<<"\n";
    for (int i=0; i<1; i++) {
        openShares=engine->syncGlobal(localOpenShares);
        response = engine->reconstructShares(localOpenShares);//*(ranBin)
        response = engine->reconstructShares(localOpenShares);
    }
  
    //printing information process
    //std::cout<< "total transmission operations performed: "<< Buffers::EngineBuffers::operationCounter_<<"\n";
    for (int i=0; i<openShares->get(0)->getLength(); i++)
    {
        std::cout<< "The Open Share from player: "<< i+1<<" is: "<<  openShares->get(0)->get(i)->getValue()  << "\n";
        
    };
    //single response line:
    std::cout<<"Response: on Multiplication: "<<engine->presentShare(prod)<<"\n";
                                                              
    return 1;
    
};


















