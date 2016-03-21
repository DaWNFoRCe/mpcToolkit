//
//  LocationalAuctionUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/25/14.
//
//
//Generic Headers
#include <stdio.h>
#include <string>

//Custom Headers
#include "LocationalAuctionUtil.h"
#include "FileUtil.h"
#include "ShareUtil.h"
#include "StandardBid.h"
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"


namespace Applications
{
    namespace Auctions
    {
        namespace Utilities
        {
            Utils::Matrix<Shares::StandardShare> * LocationalAuctionUtil::readNetwork(std::string filename, int playerId, int vertices)
            {
                
                Utils::List<std::string> *lines = ::Utilities::FileUtil::readFromTxt(filename, vertices);
                Utils::Matrix<long> * values = ::Utilities::FileUtil::buildMatrix(lines, lines->getLength(), lines->getLength());
                Utils::Matrix<Shares::StandardShare> *matrix = ::Utilities::ShareUtil::buildMatrix(values, playerId);
                delete lines;
                values->eraseAll();
                delete values;
                
                return matrix;
                
            };
            
            Utils::List<Bids::StandardBid> * LocationalAuctionUtil::readBids(std::string filename, int playerId, int totalBids)
            {
                Utils::List<std::string> *lines = ::Utilities::FileUtil::readFromTxt(filename, totalBids);
                Utils::Matrix<long> * values = ::Utilities::FileUtil::buildMatrix(lines, lines->getLength(), 4);
                Utils::Matrix<Shares::StandardShare> *matrix = ::Utilities::ShareUtil::buildMatrix(values, playerId);
                
                Utils::List<Bids::StandardBid> * bids = new Utils::List<Bids::StandardBid> (lines->getLength());
                for (int i =0; i< lines->getLength(); i++)
                {
                    bids->add( new Bids::StandardBid(matrix->get(i, 0),matrix->get(i, 1),matrix->get(i, 2),matrix->get(i, 3)));
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
