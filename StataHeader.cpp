/* 
 * File:   StataHeader.cpp
 * Author: adrianmo
 * 
 * Created on May 19, 2014, 4:41 PM
 */

#include "StataHeader.h"

StataHeader::StataHeader(): ts(0), datalabel(0) {
    
    
}

StataHeader::StataHeader(const StataHeader& orig) {
}

StataHeader::~StataHeader() {
    
    if (ts)
        delete ts;

    if (datalabel)
        delete datalabel;
}

