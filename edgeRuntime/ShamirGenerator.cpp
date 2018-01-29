//
//  ShamirGenerator.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 20/11/13.
//  Copyright (c) 2013 Abdelrahaman Aly. All rights reserved.
//

//Generic Headers
#include <iostream>

//Library Headers
#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>

//Custom Headers
#include "List.h"
#include "Constants.h"
#include "StandardPlayer.h"
#include "StandardShare.h"
#include "ShamirGenerator.h"

using namespace NTL;
namespace ShareGenerators
{
    //Method implementation of ShamirGenerator
    
    //Constructor
    ShareGenerators::ShamirGenerator::ShamirGenerator(Players::StandardPlayer * player, long p, bool mode_big_p, ZZ big_p)
    {
        //Initialize the Random number generator's seed using the system clocks
        srand((unsigned)time(0));
        //Initialize variables
        player_=player;
        this->p_=p;
        this->mode_big_p_ = mode_big_p;
        this->big_p_=big_p;
    };
    
    //Destructor
    ShareGenerators::ShamirGenerator::~ShamirGenerator(){
    
    };
    
    //generate the shares
    int ShamirGenerator::generateShares(long secret, int players, Utils::List<Shares::StandardShare> *list)
    {
        //Gets the slope for the equation randomically
        ZZ_p slope;
        if (!this->mode_big_p_)
        {
          slope= conv<ZZ_p>(rand()% this->p_-1);
        }
        else
        {
            ZZ_p slope= conv<ZZ_p>(RandomBnd(conv<ZZ>(this->big_p_)));
        }
        //TODO: The process to obtain the shares should be based on the T-1 paradigm.
        // hence the construction of the shares will depend on the number of players. there is a formula to do that to build polinomials of n deegree.
        for (ZZ i=conv<ZZ>(0); i< players;i++)
        {
            //creates the share TODO: take the process to the constructor
            Shares::StandardShare * aux =new Shares::StandardShare();
            
            aux->setPlayerId(player_->getPlayer());
            aux->setValuep(slope*(conv<ZZ_p>(i)+ conv<ZZ_p>(1)) +conv<ZZ_p>(secret));
            aux->setValue(conv<long>(slope*(conv<ZZ_p>(i)+ conv<ZZ_p>(1)) +conv<ZZ_p>(secret)));
            list->add(aux);
            
        } 
       
        return 1;
            
    };
    //generates the multiplication Regenerations
     int ShamirGenerator::multiplicationRegeneration(Shares::StandardShare * share, int players,Utils::List<Shares::StandardShare> * list)
    {
        //TODO: Add modulus operation that is why is here
        //TODO: modify this to make it authomatic in the number of players maybe slower though
        ZZ_p result=3*(conv<ZZ_p>(list->get(0)->getValue())) -3*conv<ZZ_p>((list->get(1)->getValue()))+conv<ZZ_p>((list->get(2)->getValue()));
        long value= conv<long>(result);
        share->setValue(value);
        return  1;
    };

}
