//
//  GeneralMarketAuction.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/9/16.
//
//
//Custom Headers
#include "GeneralMarketAuction.h"
#include "HalfLogExGates.h"
#include "BatcherMergeSort.h"
#include "ShamirSharesEngine.h"
#include "PushRelabel.h"
#include "GeneralMarketBid.h"
#include "ReducedBid.h"
#include "GeneralMarketBid.h"
#include "GeneralAuctionResponse.h"
#include "List.h"
#include "Matrix.h"
#include "BidUtil.h"
#include "ShareUtil.h"
#include "IExchangable.h"
#include "MathUtil.h"

namespace Applications
{
    namespace Auctions
    {
        using namespace Bids;
        
        //Constructors
        GeneralMarketAuction::GeneralMarketAuction()
        {
            return;
        };
        
        GeneralMarketAuction::GeneralMarketAuction(Utils::List<GeneralMarketBid> * bids, Utils::List<Shares::StandardShare> * suppliers, SmcEngines::ShamirSharesEngine * engine)
        {
            this->bids_=bids;
            this->suppliers_=suppliers;
            this->engine_=engine;
            
        };
        
        //private methods
        Utils::List<GeneralMarketBid> * GeneralMarketAuction::partition (Utils::List<GeneralMarketBid> * a, int *p)
        {
            int i=0;
            for (int j=0; j<a->getLength()-1; j++)
            {
                //c= aj<=am : it is equivalent to c=am>aj
                Shares::StandardShare * c = this->engine_->greaterEqualThanCatrinaModShares( a->get(a->getLength()-1)->getPrice(),a->get(j)->getPrice());
                long c_open =  engine_->reconstructShare(c) ;
                c_open=c_open;
                delete c;
                if(c_open==1)
                {
                    a->get(i)->unsafeExchange(a->get(i), a->get(j));
                    i++;
                }
            }
            //i++;
            *p= i;
            a->get(i)->unsafeExchange(a->get(i), a->get(a->getLength()-1));
            return a;
        };

        
        //quick sorts the bids
        int GeneralMarketAuction::quickSortBids(Utils::List<GeneralMarketBid> * bids)
        {
            
            if(1< bids->getLength())
            {
                int p= 0;
                
                
                Utils::List<GeneralMarketBid> * partitioned= this->partition(bids, &p);
                
                
                Utils::List<GeneralMarketBid> *left = new Utils::List<GeneralMarketBid>(p+1);
                Utils::List<GeneralMarketBid> *right = new Utils::List<GeneralMarketBid>(bids->getLength()+p-1);
                for (int i =0; i< partitioned->getLength(); i++)
                {
                    
                    if (i<p)
                    {
                        left->add(partitioned->get(i));
                    }
                    else if (i>p)
                    {
                        right->add(partitioned->get(i));
                    }
                }
                GeneralMarketBid * share_p= partitioned->get(p);
                this->quickSortBids(left);
                this->quickSortBids(right);
                int length =bids->getLength();
                bids->clear();
                
                for (int i=0; i<length ; i++)
                {
                    if (i<left->getLength())
                    {
                        bids->add(left->get(i));
                        
                    }
                    else if (i==p)
                    {
                        bids->add(share_p);
                    }
                    else
                    {
                        bids->add(right->get(i-p-1));
                    }
                }
                //delete partitioned;
                delete left;
                delete right;
            }
            else
            {
                bids=bids;
            }
            return 1;
            
        };
        
        
        Response::GeneralAuctionResponse * GeneralMarketAuction::optimizeAuction()
        {
            std::cout<<"Process Started"<<"\n";
            //build default data
            Shares::StandardShare * zero = this->engine_->buildShare(0);
            Shares::StandardShare * minus_one = this->engine_->buildShare(-1);
            Shares::StandardShare *delta =zero->clone();
            Shares::StandardShare *acumulate_delta =zero->clone();
            Shares::StandardShare *clearance_price=zero->clone();
            Shares::StandardShare *clearance_volume=zero->clone();
            //clone bids (duplicate bids quantity)
            Utils::List<GeneralMarketBid>* localBids = this->bids_->clone();
            
            //instantiation of solution array A and matrix flow "flow"
            
            Utils::List<ReducedBid>* A = new Utils::List<ReducedBid>();
            Utils::List<Shares::StandardShare>* suppliers_capacity=new Utils::List<Shares::StandardShare>();
            Utils::List<Shares::StandardShare>* suppliers_demand=new Utils::List<Shares::StandardShare>();
            for (int i =0; i< this->suppliers_->getLength(); i++)
            {
                suppliers_capacity->add(zero->clone());
                suppliers_demand->add(zero->clone());
            }
            std::cout<<"Starting Sorting"<<"\n";
            
            //sorting
            this->quickSortBids(localBids);
            std::cout<<"Finishes Sorting - Starts Delta Calculation"<<"\n";
            //delta calculation
            for (int i =0; i< localBids->getLength(); i++) {
                Shares::StandardShare * aux = this->engine_->multiply(localBids->get(i)->getDemand(),localBids->get(i)->getQuantity());
                delta= this->engine_->addTo(delta, aux);
                delete aux;
            }
            
            std::cout<<"Delta Calcultation Finished, Delta of:"<<engine_->presentShare(delta)<<" - Starting  Auction"<<"\n";
            int  counter =0;

            //start of the algorithm
            while(localBids->getLength()>0)
            {
                
            std::cout<<"Bid: "<< counter+1<<"\n";
                //pop current bid analyzed
                GeneralMarketBid * aux = localBids->pop();
                

                Shares::StandardShare * c= this->engine_->lessThanCatrinaModShares(acumulate_delta, delta);
                
                //Secure Operation ->Clearance Price
                Shares::StandardShare * substraction =this->engine_->substract(aux->getPrice(), clearance_price);
                Shares::StandardShare * multiplication =this->engine_->multiplyTo(this->engine_->multiplyTo(this->engine_->substractScalar(1, aux->getDemand()), c),substraction );
                
                clearance_price = this->engine_->addTo(clearance_price, multiplication);
                
                delete multiplication;
                delete substraction;
                
                //Secure Operation ->Volume Price
                multiplication=this->engine_->multiplyTo(this->engine_->multiplyTo(this->engine_->substractScalar(1, aux->getDemand()), c), aux->getQuantity());
                
                clearance_volume = this->engine_->addTo(clearance_volume, multiplication);
                
                delete multiplication;
                
                
                for (int i =0; i< this->suppliers_->getLength(); i++)
                {
                    Shares::StandardShare *previous_capacity = suppliers_capacity->get(i);
                    Shares::StandardShare *previous_demand = suppliers_demand->get(i);
                    
                    suppliers_capacity->set(i, this->engine_->add(this->engine_->multiply(this->engine_->multiply(this->engine_->multiply(this->engine_->substract(1, aux->getDemand()),aux->getSupplier()->get(i)), c), aux->getQuantity()), previous_capacity));
                    
                    suppliers_demand->set(i,this->engine_->add(this->engine_->multiply(this->engine_->multiply(this->engine_->multiply(this->engine_->substract(1, c), aux->getDemand()), aux->getSupplier()->get(i)), aux->getQuantity()), previous_demand));
                    
                    delete previous_capacity;
                    delete previous_demand;
                }
                
                
                acumulate_delta =this->engine_->add(acumulate_delta, aux->getQuantity());
                
                //response
                A->add(new ReducedBid(aux->getId()->clone(),aux->getPrice()->clone(), this->engine_->multiplyTo(this->engine_->sxor(aux->getDemand(), c),aux->getQuantity()))); //this->engine_->multiply(engine_->multiplyTo(engine_->substract(1, aux->getDemand()),this->engine_->substract(1,c)), aux->getQuantity()))
                
                std::cout <<" end new iteration: " <<"\n";
                
                counter++;
            }
            
            //random permutation
            std::cout<<"Starting Random Permutation"<<"\n";
            
            int round2Power = ::Utilities::MathUtil::pow2roundup(A->getLength());
            std::cout<< "roundPower: "<< round2Power <<"\n";
            Utils::List<Bids::IExchangable> * permutation = new Utils::List<Bids::IExchangable> (round2Power);
            for (int i=0; i< round2Power; i++)
            {
                if(i < A->getLength())
                {
                    permutation->add(A->get(i));
                }
                else
                {
                    permutation->add(new ReducedBid(zero->clone(),zero->clone(),zero->clone()));
                }
            }
            
            Permutations::BatcherMergeSort * permutator = new Permutations::BatcherMergeSort(this->engine_);
            
            Utils::List<Bids::IExchangable> * nPermutation =  permutator->permute(permutation);

            std::cout<<"Start Bids Allocation \n";
            //delete A;
            A->clear();
            for (int i=0; i< nPermutation->getLength(); i++)
            {
                A->add((Bids::ReducedBid*)nPermutation->get(i));
            }
            
            
            delete permutator;
            delete permutation;
            delete zero;
            delete minus_one;

            
            
            Response::GeneralAuctionResponse * response = new Response::GeneralAuctionResponse(clearance_price,clearance_volume,suppliers_capacity,A);
                                                                                            
            
            return response;
        };

        
        
    };
};