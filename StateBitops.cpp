#ifndef __STATEBITOPS_H__
#define __STATEBITOPS_H__

#include "StateBitops.h"
#include <string.h>

template <> 
string GetLSF<string>(char * ctxbuf, int size)
{
    string _st;
    char * c = ctxbuf;
    int inner_size = 0;

    while(c && *c && inner_size < size)
    {
        inner_size++;
        c++;
    }

    if (inner_size < size)
        memset(&ctxbuf[inner_size], 0, size - inner_size);

    _st.assign(ctxbuf, size);
    return _st;
}


template <> 
double GetLSF<double>(char * ctxbuf, int size)
{
    return *(double *)(ctxbuf);
}


template <> 
float GetLSF<float>(char * ctxbuf, int size)
{
    return *(float *)(ctxbuf);
}

#endif