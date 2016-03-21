//
//  LocationalAuction.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 10/30/14.
//
//

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "LocationalAuction.h"
#include "HalfLogExGates.h"
#include "BatcherMergeSort.h"
#include "ShamirSharesEngine.h"
#include "PushRelabel.h"
#include "StandardBid.h"
#include "ReducedBid.h"
#include "StandardShare.h"
#include "AuctionResponse.h"
#include "List.h"
#include "Matrix.h"
#include "BidUtil.h"
#include "ShareUtil.h"
#include "IExchangable.h"
#include "MathUtil.h"

//we asume to use 0 and n-1 as source and sink
namespace Applications
{
    namespace Auctions
    {
        using namespace Bids;
        
        Utils::List<StandardBid> * LocationalAuction::partition (Utils::List<StandardBid> * a, int *p)
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
                    Utilities::BidUtil::exchangeBids(a->get(i), a->get(j));
                    i++;
                }
            }
            //i++;
            *p= i;
            Utilities::BidUtil::exchangeBids(a->get(i), a->get(a->getLength()-1));
            return a;
        };
        
        Utils::Matrix<Shares::StandardShare> * LocationalAuction::findResidualNetwork( Utils::Matrix<Shares::StandardShare> * g, Utils::Matrix<Shares::StandardShare> *n)
        {
            Utils::Matrix<Shares::StandardShare> * result = new  Utils::Matrix<Shares::StandardShare> (g->getRows(),g->getColumns());
            for (int i =0; i<g->getRows(); i++)
            {
                for(int j =0; j< g->getColumns(); j++)
                {
                    result->set(i, j, this->engine_->substract(g->get(1,j), n->get(i, j)));
                }
            }
            return result;
        };
        
        LocationalAuction::LocationalAuction()
        {
            return;
        };
        
        LocationalAuction::LocationalAuction(Utils::List<StandardBid> * bids,Utils::Matrix<Shares::StandardShare> * network,             Utils::List<Shares::StandardShare> * markets, SmcEngines::ShamirSharesEngine * engine )
        {
            this->bids_= bids;
            this->network_=network;
            this->markets_=markets;
            this->engine_=engine;
            this->source_ =0;
            this->sink_= network_->getColumns();
        };
        
        //quick sorts the bids
        int LocationalAuction::quickSortBids(Utils::List<StandardBid> * bids)
        {
            
            if(1< bids->getLength())
            {
                int p= 0;


                Utils::List<StandardBid> * partitioned= this->partition(bids, &p);
                
                
                Utils::List<StandardBid> *left = new Utils::List<StandardBid>(p+1);
                Utils::List<StandardBid> *right = new Utils::List<StandardBid>(bids->getLength()+p-1);
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
                StandardBid * share_p= partitioned->get(p);
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
        
        //We assume that for our problem initial and final nodes were added bc of how the problem is mapped.
        Utils::Matrix<Shares::StandardShare> * LocationalAuction::maxFlowNetwork(Utils::Matrix<Shares::StandardShare> * network)
        {
            MaxFlow::PushRelabel *pushRelabel = new  MaxFlow::PushRelabel(network, network->getRows(), engine_);
            Utils::Matrix<Shares::StandardShare> *  result = pushRelabel->obtainMaxFlowStopCondition(0, network->getRows()-1);
            delete pushRelabel;
            return result;
        };
        
        
        Response::AuctionResponse * LocationalAuction::optimizeAuction()
        {
            std::cout<<"Process Started"<<"\n";
            //build default data
            Shares::StandardShare * zero = this->engine_->buildShare(0);
            Shares::StandardShare * minus_one = this->engine_->buildShare(-1);
            int s= this->source_+1;//first market
            int t = this->sink_-1;//last market

            //clone bids (duplicate bids quantity)
            Utils::List<StandardBid>* localBids = this->bids_->clone();
            
            //instantiation of solution array A and matrix flow "flow"
            Utils::Matrix<Shares::StandardShare> * flow = this->network_->clone();
            Utils::List<ReducedBid>* A = new Utils::List<ReducedBid>();
            Utils::Matrix<Shares::StandardShare> * all_c_market= new Utils::Matrix<Shares::StandardShare>(localBids->getLength(),this->markets_->getLength());
            std::cout<<"Starting Sorting"<<"\n";
            
            //sorting
            this->quickSortBids(localBids);
            
            std::cout<<"Sorting Finished - Starting  Network Definition"<<"\n";
            
            for(int i=0; i< localBids->getLength();i++)
            {
                Shares::StandardShare *supply_demand_c= this->engine_->greaterThanCatrinaModShares(localBids->get(i)->getQuantity(), zero);
                for(int j=0;j<this->markets_->getLength();j++ )
                {
                    Shares::StandardShare *aux_flow =flow->get(j+s,t);
                    
                    Shares::StandardShare *is_market_c = this->engine_->equalToToftShares(this->markets_->get(j), localBids->get(i)->getMarket());
                    all_c_market->set(i, j, is_market_c);
                    flow->set(j+s, t, this->engine_->addTo(this->engine_->multiplyTo(this->engine_->multiply(supply_demand_c, is_market_c), localBids->get(i)->getQuantity()), aux_flow));

                    delete aux_flow;
                }
                
                Shares::StandardShare *aux_quantity =localBids->get(i)->getQuantity();
                Shares::StandardShare *aux_negation_quantity = this->engine_->multiply(minus_one,aux_quantity);
                localBids->get(i)->setQuantity(this->engine_->assigmentOperationShares(supply_demand_c, aux_quantity, aux_negation_quantity));
                
                delete aux_quantity;
                delete aux_negation_quantity;
                delete supply_demand_c;
            }
            
            std::cout<<"Residual Graph Defined - Starting Delta "<<"\n";
            //obtain delta
            //this delta comes from all markets not from the sink and source assum
            Shares::StandardShare * delta = this->engine_->buildShare(0);
            for (int i=s; i<t; i++)
            {
          
                delta =engine_->addTo(delta, flow->get(i,t));
            }

            Shares::StandardShare * old_gap = delta->clone();
            
            std::cout<<"Delta Calculated - Initialization Finished: Starting Algorithm"<<"\n";
            
            int counter =0;
            //start of the algorithm
            while(localBids->getLength()>0)
            {


                //pop current bid analyzed
                StandardBid * aux = localBids->pop();
                

                //allocate the flow to the correspondent market -if NULL would accelerate the process the same on maxflow to just ignore vertex whe it does not exist
                for (int j =s; j<t; j++)
                {
                     //markets are build as 0 to n
                    Shares::StandardShare *quantity = this->engine_->assigmentOperationShares(all_c_market->get(counter,j-s), aux->getQuantity(), (long)0);
                    flow->set(0, j, this->engine_->addTo(flow->get(0, j), quantity));

                    delete quantity;
                    
                }

                Utils::Matrix<Shares::StandardShare> * temporal_flow  =this->maxFlowNetwork(flow); // it could be muted to an object with singleton so
                Shares::StandardShare *max_flow = this->engine_->buildShare(0);
                for(int j =s ; j<t; j++)
                {
                    max_flow= this->engine_->addTo(max_flow, temporal_flow->get(j, t));
                }
                ::Utilities::ShareUtil::destroyMatrix(temporal_flow);
                
                //if true aux allocated
                // ***no reshape graph
                Shares::StandardShare * current_gap= this->engine_->substract(delta, max_flow);
                
                Shares::StandardShare * gap_difference = this->engine_->substract(old_gap,current_gap);
                

                // ******* reshape graph
                for (int j =s; j< t; j++)
                {
                    Shares::StandardShare *market_flow = this->engine_->multiply(all_c_market->get(counter,j-s),aux->getQuantity());
                    Shares::StandardShare *gap_flow = this->engine_->multiply(all_c_market->get(counter,j-s),gap_difference);
                    
                    Shares::StandardShare *f_aux= flow->get(0, j);

                    Shares::StandardShare *o_flow = this->engine_->substract(flow->get(0, j), market_flow);
                    
                    flow->set(0,j,this->engine_->add(o_flow,gap_flow));
                    
                    
                    delete market_flow;
                    delete gap_flow;
                    delete o_flow;
                    delete f_aux;

                }

                //set A and destroy old share
                A->add(new ReducedBid(aux->getId()->clone(),aux->getPrice()->clone(), gap_difference->clone()));
                delete aux;
                
                //erase gap
                Shares::StandardShare * old_gap_aux= old_gap;
  
                old_gap=current_gap;
               
                delete max_flow;
                delete old_gap_aux;
                delete gap_difference;
                
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
            Response::AuctionResponse * response = new Response::AuctionResponse(flow,A);
            
            return response;
        };
    };
};