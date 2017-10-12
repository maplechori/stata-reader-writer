/* 
 * File:   StataMap.h
 * Author: adrianmo
 *
 * Created on May 28, 2014, 5:35 PM
 */

#ifndef __STATAMAP_H__
#define	__STATAMAP_H__

#include <string>
#include "StataHeader.h"

using namespace std;

class StataMap {
public:
    StataMap();
    StataMap(const StataMap& orig);
    virtual ~StataMap();
    map<string, int> stata_map;
    byteOrder order;

};

#endif	/* STATAMAP_H */

