#ifndef __STATEBITOPS_H__
#define __STATEBITOPS_H__

#include "StateBitops.h"


template <> 
string GetLSF<string>(char * ctxbuf, int size)
{
    string _st;
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