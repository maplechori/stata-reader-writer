/* 
 * File:   StataBase.h
 * Author: adrianmo
 *
 * Created on June 1, 2014, 5:11 PM
 */



#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <boost/regex.hpp>
#include <boost/variant.hpp>
#include <list>
#include <netinet/in.h>
#include <boost/program_options.hpp> 
#include "StataHeader.h"
#include "StataVariables.h"



#ifndef STATABASE_H
#define	STATABASE_H

class StataBase {
public:
    StataBase();
    void advance();
    StataBase(const StataBase& orig);
    virtual ~StataBase();
private:
    char *c;
    char *start;
    char buffer[4096];
    unsigned short ops;

};

#endif	/* STATABASE_H */

