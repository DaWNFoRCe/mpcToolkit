//
//  GeneralMarketBid.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/9/16.
//
//

#ifndef GeneralMarketBid_h
#define GeneralMarketBid_h

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
            class GeneralMarketBid:public IExchangable
            {
                
            private:
                
                Shares::StandardShare * id_;
                Utils::List<Shares::StandardShare> * supplier_;
                Shares::StandardShare * price_;
                Shares::StandardShare * quantity_;
                Shares::StandardShare * demand_;
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
                GeneralMarketBid();
                
                /**
                 * @brief Parametrize Constructor
                 * @details  Initialize all estate variables with default values
                 
                 * @exception NA
                 * @todo
                 <ul>
                 
                 <li>Build Exceptions for non compatible data
                 </ul>
                 */
                GeneralMarketBid(Shares::StandardShare *id_,  Utils::List<Shares::StandardShare> * supplier, Shares::StandardShare * price, Shares::StandardShare * quantity, Shares::StandardShare * demand);
                
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
                Utils::List<Shares::StandardShare> *  getSupplier();
                
                /**
                 @brief Simple Setter
                 @param origin market in share of the bid
                 */
                void setSupplier(Utils::List<Shares::StandardShare> * supplier);
                
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
                
                /**
                 @brief Simple Getter
                 @return  demand in share form of the bid
                 */
                Shares::StandardShare  * getDemand();
                
                /**
                 @brief Simple Setter
                 @param demand in share of the bid
                 */
                void setDemand(Shares::StandardShare * demand);
                
                
                GeneralMarketBid * clone();
                
                int secureExchange(Shares::StandardShare * auth, IExchangable* a, IExchangable *b, SmcEngines::ShamirSharesEngine * engine);
                
                int unsafeExchange( IExchangable* a, IExchangable *b);
                ~GeneralMarketBid();
                
            };
        };
    };
};

#endif /* GeneralMarketBid_h */

