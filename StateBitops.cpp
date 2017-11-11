#ifndef __STATEBITOPS_H__
#define __STATEBITOPS_H__

#include "StateBitops.h"


template <> 
string GetLSF<string>(char * ctxbuf, int size)
{
    string _st;
    _st.assign(ctxbuf, size);
    cout << _st << endl;
    return _st;
}


#endif