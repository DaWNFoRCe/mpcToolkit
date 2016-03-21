//
//  BellmanFord.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/02/14.
//
//

#ifndef edgeRuntime_BellmanFord_h
#define edgeRuntime_BellmanFord_h

#include "StandardShare.h"
#include "ShamirSharesEngine.h"

#include "List.h"

namespace Applications
{
    namespace ShortestPath
    {
        /**
         * @class BellmanFord
         * @ingroup Applications.Shortestpath
         * @brief
         * The following class is meant to implement \cite ACMPV13 Bellman Ford Algorithm. It allows the use
         * BellmanFord in MPC environment. It uses the structure of the Framework, allowing the use of engine
         * operations along  the different tasks of the algorithm. It provides a single static method to acces
         * the functionality.
         * @details Class does not need to be instantiated and developed as Utility Class.
         * @author Abdelrahaman Aly
         * @date  2014/02/11
         * @version 0.0.1.15
         */
        class BellmanFord
        {
        private:
            //Sufficiently large value, smaller enough such that no field overflow happens but still bigger than any calculated value
            static const int INF;
            
        public:
            /**
             * @brief Calculates the Shortest Path of G.
             * @details Uses the instance of the engine to obtain securely the shortest path of the Graph in case it exist.  if not returns an array full with the first element of the secuence. The method uses the List implementation of the Framework. Non Secure Operations are Executed using  NTL support. 
             * @param engine instance of the engine return by the application. 
             * @param G graph instance, in the form of native Utils::List. Made of lists.
             * @return Return the shortest path of G if there is one.
             * @retval shortestPath Returns the following: 
                       <ul>
                            <li>[1,...,1]= no feasible shortest path
                            <li>[a,...,b]= feasible shortest path
                       </ul>
             * @note It only returns the first shortest path found amongst all the possibilities, no optimization is present.
             * @exception NA
             * @todo
             <ul>
                <li>Build Exceptions for Applications
                <li>Improve engine implementation 
                <li>The current method is in charge also to build the List to be returned. It should be delegated to an specialized procedure in an utility class
                <li> Build a Configuration method or to define in the Constants Class the value of the INF Parameter
             making a different typing management system.
             </ul>
            */
            static Utils::List<Shares::StandardShare> * obtainSecureShortestPath(SmcEngines::ShamirSharesEngine * engine, Utils::List<Utils::List<Shares::StandardShare> > * G);

            
        };
        
    };
}

#endif
