/* 
 * File:   StataHeader.cpp
 * Author: adrianmo
 * 
 * Created on May 19, 2014, 4:41 PM
 */

#include "StataHeader.h"

StataHeader::StataHeader() {
    
    ts = 0x0;
    
    
}

StataHeader::StataHeader(const StataHeader& orig) {
}

StataHeader::~StataHeader() {
    
    if (ts)
        delete ts;
}

