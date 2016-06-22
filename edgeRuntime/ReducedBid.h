//
//  ReducedBid.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/21/15.
//
//

#ifndef __edgeRuntime__ReducedBid__
#define __edgeRuntime__ReducedBid__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "StandardShare.h"
#include "IExchangable.h"
#include "ShamirSharesEngine.h"

namespace Applications
{
    namespace Auctions
    {
        namespace Bids
        {
            class ReducedBid: public IExchangable
            {
                
            private:
                
                Shares::StandardShare * id_;
                Shares::StandardShare * price_;
                Shares::StandardShare * quantity_;

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
                ReducedBid();
                
                /**
                 * @brief Parametrize Constructor
                 * @details  Initialize all estate variables with default values
                 
                 * @exception NA
                 * @todo
                 <ul>
                 
                 <li>Build Exceptions for non compatible data
                 </ul>
                 */
                ReducedBid(Shares::StandardShare *id_, Shares::StandardShare * price, Shares::StandardShare * quantity);
                
                
                /**
                 @brief Simple Getter
                 @return  unique id share of the bid
                 */
                Shares::StandardShare  * getId();
                
                /**
                 @brief Simple Setter
                 @param unique id share of the bid
                 */
                void setId(Shares::StandardShare * id_);
               
                /**
                 @brief Simple Getter
                 @return  price in share form of the bid
                 */
                Shares::StandardShare  * getPrice();
                
                /**
                 @brief Simple Setter
                 @param price in share of the bid
                 */
                void setPrice(Shares::StandardShare * price);
                
                /**
                 @brief Simple Getter
                 @return  quantity in share form of the bid
                 */
                Shares::StandardShare  * getQuantity();
                
                /**
                 @brief Simple Setter
                 @param quantity in share of the bid
                 */
                void setQuantity(Shares::StandardShare * quantity);
                
                
                ReducedBid *clone();
                
                int secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine * engine);
                
                /**
                 @brief Destroyer. Eliminates all  Shares on the bid. If the Shares depend on other references it is recommended to create copies for the Bids.
                 */
                ~ReducedBid();
            };
        };
    };
};
#endif /* defined(__edgeRuntime__ReducedBid__) */
