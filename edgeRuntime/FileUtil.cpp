//
//  FileUtil.cpp
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/24/14.
//
//
//Generic Header
#include <fstream>
#include <string>
#include <sstream>

//Custom Header
#include "FileUtil.h"
#include "List.h"
#include "Matrix.h"

namespace Utilities
{
    Utils::List<std::string> * FileUtil::readFromTxt( std::string filename, int totalLines)
    {
        Utils::List<std::string> *lines = new Utils::List<std::string>(totalLines);
        std::ifstream infile (filename.data());
        std::string line="";
        
        while (std::getline(infile, line))
        {
            std::string * aux = new std::string(line);
            lines->add(aux);
        }
        return lines;
    };
    
    Utils::Matrix<long> * FileUtil::buildMatrix(Utils::List<std::string> * list, int rows, int columns)
    {
        Utils::Matrix<long> * matrix = new Utils::Matrix<long> (rows,columns);
        
        for( int i =0; i< rows;i++ )
        {
            std::string line =*list->get(i);
            std::istringstream iss(line);
            int j=0;
            //for(int j=0; j< columns;j++)
            for (int number; iss >> number;)
            {
                //std::cout<<number;
                long *aux=new long (number);
                //iss >> aux;
                matrix->set(i, j++, aux);
                
            }
        }
        return matrix;
    };
    
};
