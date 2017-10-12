#include "StataHeader.h"

template <typename T> 
T GetLSF(char * ctxbuf, int size)
{
    int i, j;
    T currValue = 0;

    for (i=0, j=0; i < size; i++, j += 8)
        currValue |= ((ctxbuf[i] >> j) & 0xFF);
 
    
    return currValue;
}