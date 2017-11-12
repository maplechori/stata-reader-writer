#include "StataHeader.h"
#include <bitset>
#include <iostream>
#include <string>

template <typename T> 
T GetLSF(char * ctxbuf, int size)
{
    int i, j;
    T currValue = 0;
    
    if (size > 2)
    {
        currValue = *(T *)(ctxbuf);
        //cout << "[size: " << currValue << " ] " << endl;
    }
    else
    {

        for (i=0, j=0; i < size; i++, j += 8)
        {
            currValue |= ((ctxbuf[i] << j) & (0xFF << j));
           // cout << "[" << i << "] " << currValue << endl;
        }
    }

    
    return currValue;

}

template <> 
string GetLSF<string>(char * ctxbuf, int size);


template <> 
float GetLSF<float>(char * ctxbuf, int size);


template <> 
double GetLSF<double>(char * ctxbuf, int size);