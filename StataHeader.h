/* 
 * File:   StataHeader.h
 * Author: adrianmo
 *
 */

#ifndef STATAHEADER_H
#define	STATAHEADER_H

#define ST_STRL   32768
#define ST_DOUBLE 65526
#define ST_FLOAT  65527
#define ST_LONG   65528
#define ST_INT    65529
#define ST_BYTE   65530

enum release { R117, R115, R114,  R113, R112 };
enum byteOrder { LSF , MSF };

class StataHeader  {
public:
    StataHeader();
    StataHeader(const StataHeader& orig);
    
    int variables;
    int observations;
    char *ts;
    char *datalabel;
    enum release fileRelease;
    enum byteOrder fileByteorder;
    
    virtual ~StataHeader();
private:

};

#endif	/* STATAHEADER_H */

