/* 
 * File:   StataMap.h
 * Author: adrianmo
 *
 * Created on May 28, 2014, 5:35 PM
 */

#ifndef STATAMAP_H
#define	STATAMAP_H
#include <string>
#include "StataHeader.h"

using namespace std;

class StataMap {
public:
    StataMap();
    StataMap(const StataMap& orig);
    virtual ~StataMap();
private:
    std::string mapName;
    int position;
    byteOrder order;

};

#endif	/* STATAMAP_H */

