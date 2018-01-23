
//
//  main.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 31/10/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//


//Generic Headers
#include <iostream>
//#include <String>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

//#include <mach/mach.h>
//#include <mach/mach_time.h>

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
    int localAddress[]= {10,33,137,40};
    int externalAddress[]={127,0,0,1}; //{192,168,1,5};//{130,104,59,120};//{130,104,59,120};
    //int localAddress[]= {127,0,0,1};
    std::cout<< "The current player is: "<< player << " and his secret is: "<< secret<<"\n";
    
    Players::StandardPlayer * p1=new Players::StandardPlayer(1,3001,localHome);
    Players::StandardPlayer * p2=new Players::StandardPlayer(2,3002,localHome);
    
    Players::StandardPlayer * p3=new Players::StandardPlayer(3,3003,localHome);
    
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
    
    
    for (int j=0; j<1; j++)
    {
        //times for key generation
        //time measurement initialization   
        std::cout<<"Start Key generation \n";
        std::clock_t begin = std::clock();         

        for (int i=0; i<1000; i++)
        {
            

            Shares::StandardShare * key = engine->generateShareRandomNumber();
            long g = 1;
            
            long l_alpha=1; 
            if(player==1)
            {
                l_alpha=1;
            }
            else if (player==2)
            {
                l_alpha=1;
            }
            else
            {
                l_alpha=1;
            }

            g= pow(g,l_alpha*key->getValue());
            Utils::List<Shares::StandardShare> * p_key_array = engine->shareValue(g);
            Shares::StandardShare * p_key = engine->multiply(engine->multiply(p_key_array->get(0),p_key_array->get(1)),p_key_array->get(2));
            long p_p_key =engine->presentShare(p_key);

            delete p_key_array;
            delete p_key;
           
        }

        std::clock_t end = std::clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;             
        std::cout<<"Time in Seconds: "<< elapsed_secs<<"\n"; 
         
        //times for repair
        //time measurement initialization   
        std::cout<<"Start Repair \n";
        begin = std::clock();         
        for (int i=0;i<1000;i++)
        {
            delete shares;
            shares=engine->shareValue(secret);
            prod= engine->substract(engine->multiply(shares->get(0), long(2)), (shares->get(1)));//engine->lessEqualThanCatrinaModShares(shares->get(0), shares->get(1));
        }
        end = std::clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;             
        std::cout<<"Time in Seconds: "<< elapsed_secs<<"\n"; 
         
        std::cout<<"present new reconstruction: "<< engine->presentShare(prod)<<"\n";              

        std::cout<<"Comparitions: \n"<<engine->totalCom<<"\n";
        std::cout <<"Operations: "<<Buffers::EngineBuffers::operationCounter_<<"\n";
    }
    
    
    return 1;
    
};
