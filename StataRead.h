#ifndef __STATAREAD_H__
#define __STATAREAD_H__

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <boost/regex.hpp>
#include <boost/variant.hpp>
#include <boost/program_options.hpp>
#include <list>
#include <boost/asio.hpp>
#include "StataHeader.h"
#include "StataVariables.h"


enum state {
    OPEN_HEADER,
    OPEN_RELEASE,
    OPEN_BYTEORDER,
    OPEN_K,
    OPEN_N,
    OPEN_LBL,
    OPEN_TS,
    OPEN_MAP,
    OPEN_VARTYPES,
    OPEN_VARNAMES,
    OPEN_SORTLIST,
    OPEN_FORMATS,
    OPEN_VALLBL,
    OPEN_VARLBL,
    OPEN_CHARACTERISTICS,
    OPEN_DATA,
    CLOSE_HEADER


};

#define XML_OPEN_FILE "<stata_dta>"
#define XML_CLOSE_FILE "</stata_dta"

#define XML_OPEN_HEADER "<header>"
#define XML_CLOSE_HEADER "</header>"

#define XML_OPEN_RELEASE "<release>"
#define XML_CLOSE_RELEASE "</release>"

#define XML_OPEN_BYTEORDER "<byteorder>"
#define XML_CLOSE_BYTEORDER "</byteorder>"

#define XML_OPEN_VARIABLES_COUNT "<K>"
#define XML_CLOSE_VARIABLES_COUNT "</K>" 

#define XML_OPEN_OBSERVATIONS_COUNT "<N>"
#define XML_CLOSE_OBSERVATIONS_COUNT "</N>"

#define XML_OPEN_LABEL "<label>"
#define XML_CLOSE_LABEL "</label>"

#define XML_OPEN_TIMESTAMP "<timestamp>"
#define XML_CLOSE_TIMESTAMP "</timestamp>"

#define XML_OPEN_MAP "<map>"
#define XML_CLOSE_MAP "</map>"

#define XML_LSF "LSF"
#define XML_MSF "MSF"



#endif


