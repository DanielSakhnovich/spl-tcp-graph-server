#pragma once

#include "std_lib_facilities.h"

class index_out_of_bounds_exception {};
using namespace std;


template<class T, int N>
class ring {
    int currIndex;
    T *elements;
 public:
    ring() :elements(new T[N]), currIndex(0){
    }

    T& operator [](int index){
        if(index < 0 || index >= N)
            throw index_out_of_bounds_exception();
        return elements[index];
    }
   ~ring(){
        delete [] elements ;
    } 

    void push_back( T data ){
        elements[this->currIndex] = data;
        this->currIndex = (this->currIndex + 1) % N; 
    }



};
template<class T, int N >
ostream& operator<<(ostream& os , ring <T,N>& x ){
    os << "[";
    for(int i = 0; i< N; i++){
            os<< x[i] << ", ";
    }
    os << "]";
    return os;
};
