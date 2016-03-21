//
//  BellmanFord.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/02/14.
//
//
//local headers
#include "BellmanFord.h"
#include "StandardShare.h"
#include "ShamirSharesEngine.h"
#include "ShareUtil.h"
#include "List.h"
#include "EngineBuffers.h"
namespace Applications
{
   namespace ShortestPath
   {

       //The INF value is tight to the bit sizes  used and on the application requirements.
       const int Applications::ShortestPath::BellmanFord::INF=99;
       
       //method implementation
       Utils::List<Shares::StandardShare> * Applications::ShortestPath::BellmanFord::obtainSecureShortestPath(SmcEngines::ShamirSharesEngine * engine, Utils::List<Utils::List<Shares::StandardShare> > * G)
       {
           
           //instantiate variables
           int nodes = G->getLength();
           int start =0;
           
           //TODO: Build a set method on the list implementation to replace this use of the double pointer which is unestable
           Shares::StandardShare ** d= new Shares::StandardShare *[nodes] ; //double pointer array of disntances
           Shares::StandardShare ** f= new Shares::StandardShare *[nodes] ; //double pointer array of  father nodes
           
           Utils::List<Shares::StandardShare> * results= new Utils::List<Shares::StandardShare>(nodes);// list to be returned as answer
           
           //put default values in place for lists
           for (int i=0; i<nodes; i++)
           {
               d[i]= NULL;
               f[i]=engine->buildShare(1);
           }
           d[start] = engine->buildShare(0);
           
           //BellmanFord MPC Implementation
           for (int i =0; i< nodes; i++)
           {
               for (int u =0; u<nodes; u++) //this only works with a matrix representation that hides the structure
               {
                   int edges = G->get(i)->getLength();
                   for (int v=0; v<edges; v++)
                   {
                       Shares::StandardShare * comp;
                       Shares::StandardShare * aux= engine->addShares(d[u], G->get(u)->get(v));
                       if (d[v]==NULL)
                       {
                           comp=engine->buildShare(1);
                           d[v]=engine->buildShare(BellmanFord::INF);
                       }
                       else
                       {
                           comp= engine->greaterEqualThanCatrinaModShares(d[v], aux);
                       }
                       
                       Shares::StandardShare * aux_before=d[v];
                       d[v]= engine->assigmentOperationShares(comp, aux, aux_before);

                       //TODO: build an overload of the assigment method such that it receives integers in one of the values and does a wrap with the build method
                       Shares::StandardShare * s_u=engine->buildShare(u);
                       Shares::StandardShare * aux_f =                        f[v];
                       f[v]= engine->assigmentOperationShares(comp,s_u , f[v]);
                       delete aux_f;
                       delete s_u;
                       delete aux_before;
                       delete aux;
                       delete comp;
                   }
               }
           }
           //Migration of types from array to list.
           for (int i=0; i<nodes; i++)
           {
               results->add(f[i]);
           }
           for (int i=0; i<nodes; i++)
           {
               delete d[i];

           }
           return results;
       };
   };
};