#include "StataHeader.h"
#include <bitset>
#include <iostream>
#include <string>
#include <cstdio>

template <typename T> 
T GetLSF(char * ctxbuf, int size)
{
    int i, j;
    T currValue = 0;

    for (i=0, j=0; i < size; i++, j += 8)
        currValue  |= ((ctxbuf[i] << j) & (0xFF << j));
        

    return currValue;

}

template <> 
string GetLSF<string>(char * ctxbuf, int size);


template <> 
float GetLSF<float>(char * ctxbuf, int size);


template <> 
double GetLSF<double>(char * ctxbuf, int size);