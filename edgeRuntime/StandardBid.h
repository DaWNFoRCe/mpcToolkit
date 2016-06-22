//
//  StandardBid.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 10/30/14.
//
//

#ifndef __edgeRuntime__StandardBid__
#define __edgeRuntime__StandardBid__

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
            class StandardBid: public IExchangable
            {
                
            private:
        
                Shares::StandardShare * id_;
                Shares::StandardShare * market_;
                Shares::StandardShare * price_;
                Shares::StandardShare * quantity_;
                //state variables for extended use - do not use if is not the correct configuration

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
                StandardBid();
                
                /**
                 * @brief Parametrize Constructor
                 * @details  Initialize all estate variables with default values
                 
                 * @exception NA
                 * @todo
                 <ul>
                 
                 <li>Build Exceptions for non compatible data
                 </ul>
                 */
                StandardBid(Shares::StandardShare *id_, Shares::StandardShare * market, Shares::StandardShare * price, Shares::StandardShare * quantity);
                
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
                 @return  origin market in share form of the bid
                 */
                Shares::StandardShare  * getMarket();
                
                /**
                 @brief Simple Setter
                 @param origin market in share of the bid
                 */
                void setMarket(Shares::StandardShare * market);
                
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
                
                                
                StandardBid *clone();
                
                int secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine * engine);
                
                /**
                 @brief Destroyer. Eliminates all  Shares on the bid. If the Shares depend on other references it is recommended to create copies for the Bids.
                 */
                 ~StandardBid();
            };
        };
    };
};
#endif /* defined(__edgeRuntime__StandardBid__) */
