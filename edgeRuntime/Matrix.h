//
//  Matrix.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 11/12/14.
//
//

#ifndef __edgeRuntime__Matrix__
#define __edgeRuntime__Matrix__

//Generic Headers
#include <stdio.h>
#include <vector>

//Custom Headers
#include "List.h"

namespace Utils
{
    template <class T> class Matrix
    {
    private:
        List<List<T> > * matrix_;
        int rows_;
        int cols_;
    public:
        Matrix(int rows, int cols)
        {
            //std::vector<std::vector <T*> > matrix;
            this->rows_=rows;
            this->cols_=cols;
            matrix_= new List<List<T> >(rows);
            for (int i=0; i< this->rows_; i++)
            {
                matrix_->add(new List<T>(cols));
                for (int j=0;j<this->cols_; j++)
                {
                    matrix_->get(i)->add(NULL);
                }
            }
//            matrix_=matrix;
        };
        
        T * get(int row, int column)
        {
            return matrix_->get(row)->get(column);
        };
        

        void set(int row, int column, T * data)
        {
            matrix_->get(row)->set(column,data);
        };
        
        int getRows()
        {
            return this->rows_;
        };
        
        int getColumns()
        {
            return this->cols_;
        };
        
        Matrix <T> * clone()
        {
            Matrix<T> * copy = new Matrix<T>(this->rows_, this->cols_);
            for(int i =0; i< this->rows_; i++)
            {
                for(int j =0; j<this->cols_; j++)
                {
                    if( this->get(i,j)!= NULL)
                    {
                        copy->set(i,j, this->get(i,j)->clone());
                    }
                }
            }
            return copy;
        };
        

        int eraseAll()
        {
            for(int i =0; i<this->rows_;i++)
            {
                for (int j=0; j< this->cols_;j++)
                {
                    T *aux =this->get(i, j);
                    this->set(i,j, NULL);
                    delete aux;
                }
            }
            return 1;
        };
        
        ~Matrix()
        {
            for(int i =0; i<this->rows_;i++)
            {
                delete this->matrix_->get(i);
                this->matrix_->set(i, NULL);
            }
            delete matrix_;
        };
        
    };
};

#endif /* defined(__edgeRuntime__Matrix__) */
