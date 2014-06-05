/* 
 * File:   StataHeader.h
 * Author: adrianmo
 *
 * Created on May 19, 2014, 4:41 PM
 */

#ifndef STATAHEADER_H
#define	STATAHEADER_H

enum release { R117, R115, R114,  R113, R112 };
enum byteOrder { LSF , MSF };

class StataHeader  {
public:
    StataHeader();
    StataHeader(const StataHeader& orig);
    
    int variables;
    int observations;
    char *ts;
    enum release fileRelease;
    enum byteOrder fileByteorder;
    
    virtual ~StataHeader();
private:

};

#endif	/* STATAHEADER_H */

