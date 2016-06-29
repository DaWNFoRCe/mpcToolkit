//
//  List.h
//  edgeRuntime
//
//  Created by Abdelrahaman Aly on 22/11/13.
//

#ifndef edgeRuntime_List_h
#define edgeRuntime_List_h

//Generic Headers
#include <iostream>
#include <vector>

//Custom Headers
#include "Constants.h"
namespace Utils
{
    template <class T> class List
    {
    private:
        
        // Data container
        std::vector<T*>  vect_;
        //size of the vector in memory
        int size_;
        //current toal number of added elements
        int length_;
        
        //Initialize container
        std::vector<T*>  initializeVector(int size, std::vector<T*>  vect )
        {
            for (int i=0;i<size;i++)
            {
                vect[i]=NULL;
            }
            return vect;
        }
        
    public:
        //Default Constructor
        List()
        {
            this->vect_.resize(Utilities::Constants::MAX_LIST_SIZE);
            this->vect_=this->initializeVector(Utilities::Constants::MAX_LIST_SIZE,this->vect_);
            this->size_=Utilities::Constants::MAX_LIST_SIZE;
            this->length_=0;
	    //List((Utilities::Constants::MAX_LIST_SIZE);	
            return;
        };
        
        //Sized List Constructor
        List(int size)
        {
            //TODO:lookup on how new works with //
            std::vector<T*>  local(size);
            this->vect_ =local;
	    //this->vect_.resize(size);
            this->vect_=this->initializeVector(size,this->vect_);
            this->length_=0;
            this->size_=size;
            return;            
        };
        
        //returns the number of elements that the list contains.
        int getLength()
        {
            return this->length_;
        };
        //return the number of initialize positions -not the current size
        int getSize(){
            return this->size_;
        };
        
        
        
        //Adds an Element to the Container
        int add(T * element) //TODO: check the return type
        {
            
            this->vect_[this->length_]=element;
            this->length_++;
            return 1;
        };
        
        //Eleminates an element from The Container
        int eliminateLast()
        {
            this->vetc_.pop_back();
            this->length_--;
        };
        
        //Gets the element on the index
        T * get(int index)
        {
            return this->vect_[index];
        };
        
        //Sets a value on a given position
        int set (int index, T * element)
        {
            this->vect_[index]= element;
            
            if (this->length_< index+1) {
                this->length_=index+1;
            }
            return 1;
        };
        
        // erase all the list
        int clear()
        {
            for(int i=0;i<this->length_;i++)
            {
                if (this->vect_[i]!=NULL)
                {
                    this->vect_[i]=NULL;
                }
                
                
            }
            //this->vect_=this->initializeVector(this->size_,this->vect_);
            
            this->length_=0;
            
        };
        //pops an element from a list
        T * pop()
        {
            T * aux = this->vect_[0];
            this->vect_[0]=NULL;
            
            this->vect_.erase(this->vect_.begin());
            this->length_=this->length_-1;
            return aux;
        };
        
        //Generates an exact copy of the list
        List<T> * clone()
        {
            List<T> * list = new List<T>(this->getLength());
            
            for (int i=0; i<this->getLength(); i++)
            {
                list->add(this->get(i)->clone());
            }
            return list;
        };

        int eraseAll()
        {
            T *aux;
            for (int i=0; i <size_;i++)
            // for( typename std::vector<T*>::iterator it =vect_.begin(); it!= vect_.end(); ++it)
            {
                aux = vect_[i];
                vect_[i]=NULL;
                delete aux;
            }
            length_=0;
        };
        
    };
}


#endif
