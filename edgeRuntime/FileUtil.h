//
//  FileUtil.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/24/14.
//
//

#ifndef __edgeRuntime__FileUtil__
#define __edgeRuntime__FileUtil__

//Generic Headers
#include <stdio.h>

//Custom Headers
#include "List.h"
#include "Matrix.h"
namespace Utilities
{
    class FileUtil
    {
    public:
        static Utils::List<std::string> * readFromTxt( std::string filename, int totalLines);
        
        static Utils::Matrix<long> * buildMatrix(Utils::List<std::string> * list, int rows, int columns);
        
        
    };
};
#endif /* defined(__edgeRuntime__FileUtil__) */
