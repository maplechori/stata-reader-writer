/* 
 * File:   StataHeader.h
 * Author: adrianmo
 *
 */

#ifndef STATAHEADER_H
#define	STATAHEADER_H

#include <string>
#include <stdint.h>
#include <map>
#include <sstream>

using namespace std;

#define ST_STRL   32768
#define ST_DOUBLE 65526
#define ST_FLOAT  65527
#define ST_LONG   65528
#define ST_INT    65529
#define ST_BYTE   65530

enum release { R119, R118, R117, R115, R114, R113, R112 };
enum byteOrder { LSF , MSF };

class StataHeader  {

public:
    StataHeader();
    StataHeader(const StataHeader& orig);
    
    uint32_t variables;
    uint64_t observations;
    std::string ts;
    std::string datalabel;
    enum release fileRelease;
    enum byteOrder fileByteorder;
    virtual ~StataHeader();
    string showHeader();
private:

};

#endif	/* STATAHEADER_H */

