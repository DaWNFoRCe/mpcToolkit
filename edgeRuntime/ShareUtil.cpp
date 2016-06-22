//
//  ShareUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 1/28/14.
//

//Generic Headers
#include <memory.h>

//Custom Headers
#include "ShareUtil.h"
#include "StandardShare.h"
#include "List.h"
#include "Matrix.h"

namespace Utilities
{
    //Method Implementation of ShareUtils
    
    //build method of shares
    Shares::StandardShare * ShareUtil::wrapStandardShare(int playerId,long value)
    {
       Shares::StandardShare *  share= new Shares::StandardShare();

        share->setPlayerId(playerId);
        share->setValue(value);
        return share;
    };
    
    //swaps a for b
    int ShareUtil::exchangeShares(Shares::StandardShare ** a, Shares::StandardShare **b)
    {
        Shares::StandardShare * aux= *a;
        *a=*b;
        *b=aux;
        return 1;
    };    
    
    //build mothod for list of shares
    Utils::List<Shares::StandardShare> * ShareUtil::wrapStandardShareList(int  playerId,int *values, int size)
    {
        Utils::List<Shares::StandardShare> * list= new Utils::List<Shares::StandardShare> (size);
        for (int i=0; i<size; i++)
        {
            //invoques build method.
            list->add(ShareUtil::wrapStandardShare(playerId, values[i]));
        }
        return list;
        
    };
    
    //destroy list of shares
    int ShareUtil::destroyList( Utils::List<Shares::StandardShare> * list)
    {
        if (list==NULL) {
            return 1;
        }
        
        for (int i=0; i< list->getLength(); i++)
        {
            if(list->get(i) !=NULL)
            {
                Shares::StandardShare * aux = list->get(i);
                list->set(i, NULL);
                delete aux;
            }
            
        }
        delete list;
        
        return 1;
    };
    
    //destroy list of shares
    int ShareUtil::destroyMatrix( Utils::Matrix<Shares::StandardShare> * matrix)
    {
        if (matrix==NULL)
        {
            return 1;
        };
        
        for (int i=0; i< matrix->getRows(); i++)
        {
            for (int j=0; j< matrix->getColumns(); j++)
            {
                if(matrix->get(i,j) !=NULL)
                {
                    Shares::StandardShare * aux = matrix->get(i,j);
                    matrix->set(i,j, NULL);
                    delete aux;
                }
            }
        }
        delete matrix;
        
        return 1;
    };
    

    Utils::Matrix<Shares::StandardShare> *  ShareUtil::buildMatrix( Utils::Matrix<long> * matrix, int playerId)
    {
        Utils::Matrix<Shares::StandardShare> * s_matrix = new     Utils::Matrix<Shares::StandardShare>(matrix->getRows(), matrix->getColumns());
        for (int i=0; i< matrix->getRows(); i++)
        {
            for (int j=0; j<matrix->getColumns(); j++)
            {
                s_matrix->set(i,j , ShareUtil::wrapStandardShare(playerId, *matrix->get(i,j)));
            }
        }
        return s_matrix;
    };
    
}
