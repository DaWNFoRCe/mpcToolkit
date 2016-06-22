//
//  PushRelabel.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/7/14.
//
//

//Generic Headers

//Custom Headers
#include "PushRelabel.h"

#include "StandardShare.h"
#include "ShamirSharesEngine.h"
#include "Matrix.h"
#include "ShareUtil.h"
namespace Applications
{
    namespace MaxFlow
    {
        //The INF value is tight to the bit sizes  used and on the application requirements.
        const int PushRelabel::INF=9999999;
        const int PushRelabel::LIMIT=99;
        //Default contructor
        PushRelabel::PushRelabel()
        {
            this->g_=NULL;
            this->engine_=NULL;
            this->vertices_=0;

        };
        
        //Parametrized contructor
        PushRelabel::PushRelabel( Utils::Matrix<Shares::StandardShare> * g, int vertices, SmcEngines::ShamirSharesEngine * engine)
        {
            this->vertices_=vertices;
            this->g_=g;
            this->engine_=engine;
        };
        
        //Simple Getter
        Utils::Matrix<Shares::StandardShare> * PushRelabel::getG()
        {
            return this->g_;
        };
        
        //Simple Setter
        void PushRelabel::setG(Utils::Matrix<Shares::StandardShare> * g)
        {
            this->g_=g;
        };
        
        //Simple Getter
        int PushRelabel::getVertices()
        {
            return this->vertices_;
        };
        
        //Simple Setter
        void PushRelabel::setVertices(int vertices)
        {
            this->vertices_=vertices;
        };
        
        
        //Simple Getter
        SmcEngines::ShamirSharesEngine *  PushRelabel::getEngine()
        {
            return this->engine_;
        };
        
        //Simple Setter
        void PushRelabel::setEngine(SmcEngines::ShamirSharesEngine * engine)
        {
            this->engine_=engine;
        };
        
        //pushes either the excess of the node or its residual capacity
        int PushRelabel::push( int u, int v, Utils::Matrix<Shares::StandardShare> * F,Shares::StandardShare * rc, Utils::List<Shares::StandardShare> * excess, Shares::StandardShare * push_direction)
        {
            Shares::StandardShare * send = this->engine_->assigmentOperationShares(push_direction, excess->get(u), rc);

            F->set(u,v,this->engine_->addTo(F->get(u,v),send));
            F->set(v,u,this->engine_->substractTo(F->get(v,u),send));

            excess->set(u,this->engine_->substractTo(excess->get(u),send));
            excess->set(v, this->engine_->addTo( excess->get(v),send));

            
            delete send;
            return 1;
        };
        
        int PushRelabel::discharge (Utils::List<int> * list, Utils::Matrix<Shares::StandardShare > * G, Utils::Matrix<Shares::StandardShare > * F, Utils::List<Shares::StandardShare> * height ,Utils::List<Shares::StandardShare> * excess)
        {
            Shares::StandardShare * s_INF = this->engine_->buildShare(PushRelabel::INF);
            Shares::StandardShare * s_zero = this->engine_->buildShare(0);
            
            for (int i=0; i<list->getLength(); i++) //replace by the elements on the list
            {
                int u = *list->get(i);
                Shares::StandardShare * min_height = s_INF->clone();
                for(int v=0; v< this->vertices_;v++)
                {
                  if(G->get(u, v)!=NULL)
                  {
                    Shares::StandardShare * rc = engine_->substract(G->get(u,v), F->get(u,v));

                    Shares::StandardShare * rc_comparision= this->engine_->lessThanCatrinaModShares(  s_zero,rc);
                    Shares::StandardShare * is_height= this->engine_->lessThanCatrinaModShares(height->get(v), height->get(u));
                    rc= this->engine_->multiplyTo(rc, is_height);
                    Shares::StandardShare * push_direction= this->engine_->lessThanCatrinaModShares(excess->get(u), rc);
                    this->push(u, v, F, rc, excess, push_direction);
                    Shares::StandardShare * c= this->engine_->lessThanCatrinaModShares(height->get(v), min_height);
                    c= this->engine_->multiplyTo(c, rc_comparision);
                    Shares::StandardShare * augm_height= this->engine_->add(height->get(v), 1);
                    Shares::StandardShare * temp_min_height= this->engine_->assigmentOperationShares(c, augm_height, min_height);
                    
                    delete min_height;
                    delete rc;
                    delete rc_comparision;
                    delete is_height;
                    delete push_direction;
                    delete c;
                    delete augm_height;

                    min_height= temp_min_height;
                  }
                    
                };
                Shares::StandardShare * aux = height->get(u);
                height->set(u,min_height);
                delete aux;
            }
            
            delete s_zero;
            delete s_INF;
            return 1;
        };
        
        Utils::Matrix<Shares::StandardShare > * PushRelabel::obtainMaxFlow(int source, int sink, int limit, bool stop)
        {
            
            //auxilliar values definitions
            Shares::StandardShare *s_zero = this->engine_->buildShare(0);
            
            Utils::Matrix<Shares::StandardShare > * F = new Utils::Matrix<Shares::StandardShare >(this->vertices_,this->vertices_);

            Utils::List<Shares::StandardShare> *  height =new Utils::List<Shares::StandardShare> ();
          
            Utils::List<Shares::StandardShare> *  excess= new Utils::List<Shares::StandardShare> ();

            Utils::List<int> * list = new Utils::List<int> ();
            
            //initialization process
            for (int i=0; i<this->vertices_; i++)
            {
                for(int j=0; j< this->vertices_; j++)
                {
                    F->set(i,j,s_zero->clone());
                    //if (g_->get(i, j)!=NULL)
                    //{
                    //    F->set(i,j,s_zero->clone());
                    //}
                    //else
                    //{
                    //    F->set(i, j, NULL);
                    //}

                };
                height->add(s_zero->clone());
                excess->add( s_zero->clone());
                if (i!= source  && i!= sink)
                {
                    list->add(new int(i));
                }
            }
            height->set(source,this->engine_->buildShare(this->vertices_)); //longest path from source to sink is less than n long
            excess->set(source,this->engine_->buildShare(PushRelabel::INF)); //send as much flow as possible to neighbours of source
            
            for(int v=0; v< this->vertices_; v++)
            {
                if (this->g_->get(source,v) != NULL)
                {
                    Shares::StandardShare * push_direction = engine_->lessThanCatrinaModShares(excess->get(source),this->g_->get(source,v));
                    this->push(source, v, F, g_->get(source,v), excess, push_direction);
                    delete push_direction;
                }
            };

            //preflow - push
            for (int i =0; i< limit; i++)
            {
                std::cout<<"Iteration: " <<i <<"\n";
                this->discharge(list, this->g_, F, height, excess);

                
                //stopping condition
                if (stop)
                {
                    Shares::StandardShare * totalExcess = this->engine_->buildShare(0);
                    for (int  j=source+1; j<sink; j++)
                    {
                        totalExcess=this->engine_->addTo(totalExcess, excess->get(j));
                    }
                    Shares::StandardShare * c = this->engine_->equalToToftShares(totalExcess, s_zero );

                    delete totalExcess;
                    long s_c =this->engine_->reconstructShare(c);
                    if (s_c)
                    {
                        delete c;
                        break;
                    }

                    delete c;
                }
            }
            
            delete s_zero;
            for(int i=0; i< list->getLength();i++)
            {
                delete list->get(i);
            }
            
            delete list;
            ::Utilities::ShareUtil::destroyList(height);
            ::Utilities::ShareUtil::destroyList(excess);
            
            return F;

        };
        
        Utils::Matrix<Shares::StandardShare> * PushRelabel::obtainMaxFlowFullRounds(int source, int sink)
        {
            int totalIterations=  (4*pow(this->vertices_,2)-10*this->vertices_+4);
            return this->obtainMaxFlow(source, sink, totalIterations, false);
        };
        
        
        Utils::Matrix<Shares::StandardShare> * PushRelabel::obtainMaxFlowStopCondition(int source, int sink)
        {
            return this->obtainMaxFlow(source, sink, PushRelabel::LIMIT, true );
        };
        
    };
};