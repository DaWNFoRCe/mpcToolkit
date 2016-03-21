//
//  PushRelabel.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/7/14.
//
//

#ifndef __edgeRuntime__PushRelabel__
#define __edgeRuntime__PushRelabel__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "ShamirSharesEngine.h"
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"

namespace Applications
{
    namespace MaxFlow
    {
        class PushRelabel
        {
        private:
            
            SmcEngines::ShamirSharesEngine * engine_;
            Utils::Matrix<Shares::StandardShare> * g_;
            int vertices_;
            //Sufficiently large value, smaller enough such that no field overflow happens but still bigger than any calculated value
            static const int INF;
            static const int LIMIT;
            
        public:
            
            /**
             * @brief Default class constructor
             * @details  Initialize all estate variables with default values
             
             * @exception NA
             * @todo
             <ul>
             
             <li>Build Exceptions for non compatible data
             </ul>
             */
            PushRelabel();
            
            /**
             * @brief Parametrize Constructor
             * @details  Initialize all estate variables with default values
             
             * @exception NA
             * @todo
             <ul>
             
             <li>Build Exceptions for non compatible data
             </ul>
             */
            PushRelabel(Utils::Matrix<Shares::StandardShare> * g, int vertices, SmcEngines::ShamirSharesEngine * engine);
            
            /**
             @brief Simple Getter
             @return  Graph Matrix in share form representing the graph
             */
            Utils::Matrix<Shares::StandardShare> * getG();
            
            /**
             @brief Simple Setter
             @param Graph  Matrix in share  form representing the graph
             */
            void setG(Utils::Matrix<Shares::StandardShare> * g);
            
            /**
             @brief Simple Getter
             @return  Graph Matrix in share form representing the graph
             */
            int getVertices();
            
            /**
             @brief Simple Setter
             @param Graph  Matrix in share  form representing the graph
             */
            void setVertices(int vertices);
            
            /**
             @brief Simple Getter
             @return  Returns Engine being used
             */
            SmcEngines::ShamirSharesEngine  * getEngine();
            
            /**
             @brief Simple Setter
             @param Graph in share of the bid
             */
            void setEngine(SmcEngines::ShamirSharesEngine * engine);
            
            /**
             * @brief Method to Push Flow and balance vertex
             * @details Follows the direcciont assigned and pushes flow from the vertex
             * @param u node origin
             * @param v node destiny
             * @param F flow matrix
             * @param rc residual capacity of the edge
             * @param excess current excess of the nodes needed for (u and v)
             * @param push_direction condition whether or not flow needs to be allocated or push
             * @exception NA
             * @todo
             <ul>
             <li>Build Exceptions for non compatible data
             </ul>
             */
            int push( int u, int v, Utils::Matrix<Shares::StandardShare> * F, Shares::StandardShare * rc, Utils::List<Shares::StandardShare> * excess, Shares::StandardShare * push_direction);

            /**
             * @brief Default class constructor
             * @details  Initialize all estate variables with default values
             * param list
             * @exception NA
             * @todo
             <ul>
             
             <li>Build Exceptions for non compatible data
             </ul>
             */
            int discharge (Utils::List<int> * list, Utils::Matrix<Shares::StandardShare> * G, Utils::Matrix<Shares::StandardShare> * F, Utils::List<Shares::StandardShare> * height , Utils::List<Shares::StandardShare> * excess );

            
            Utils::Matrix<Shares::StandardShare> * obtainMaxFlow(int source, int sink, int limit, bool stop);
            Utils::Matrix<Shares::StandardShare> * obtainMaxFlowFullRounds(int source, int sink);
            Utils::Matrix<Shares::StandardShare> * obtainMaxFlowStopCondition(int source, int sink);
            
            
        };
    };
};
#endif /* defined(__edgeRuntime__PushRelabel__) */
