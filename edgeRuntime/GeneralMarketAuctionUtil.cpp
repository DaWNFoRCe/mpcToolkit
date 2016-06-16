//
//  GeneralMarketAuctionUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 6/15/16.
//
//
//Generic Headers
#include <stdio.h>
#include <string>

//Custom Headers
#include "GeneralMarketAuctionUtil.h"
#include "FileUtil.h"
#include "ShareUtil.h"
#include "GeneralMarketBid.h"
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"


namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            Utils::List<Bids::GeneralMarketBid> * GeneralMarketAuctionUtil::readBids(std::string filename, int playerId, int totalBids, int suppliers)
            {
                Utils::List<std::string> *lines = ::Utilities::FileUtil::readFromTxt(filename, totalBids);
                Utils::Matrix<long> * values = ::Utilities::FileUtil::buildMatrix(lines, lines->getLength(), suppliers+4);
                Utils::Matrix<Shares::StandardShare> *matrix = ::Utilities::ShareUtil::buildMatrix(values, playerId);
                
                Utils::List<Bids::GeneralMarketBid> * bids = new Utils::List<Bids::GeneralMarketBid> (lines->getLength());
                for (int i =0; i< lines->getLength(); i++)
                {
                    Utils::List<Shares::StandardShare> * suppliers_list = new Utils::List<Shares::StandardShare>(suppliers);
                    for(int j =0 ; j< suppliers;j++)
                    {
                        suppliers_list->add(matrix->get(i, j+1));
                    }
                    
                    bids->add( new Bids::GeneralMarketBid(matrix->get(i, 0),suppliers_list,matrix->get(i, 11),matrix->get(i, 12),matrix->get(i, 13)));
                }
                lines->eraseAll();
                delete lines;
                values->eraseAll();
                delete values;
                delete matrix;
                //::Utilities::ShareUtil::destroyMatrix(matrix);
                return bids;
            };
        };
    };
};