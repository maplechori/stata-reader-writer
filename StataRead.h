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
#include <boost/test/unit_test.hpp>

#include <list>
#include <boost/asio.hpp>
#include "StataHeader.h"
#include "StataVariables.h"

#define XML_OPEN_FILE "<stata_dta>"
#define XML_CLOSE_FILE "</stata_dta>"

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

#define XML_OPEN_VARIABLE_TYPES "<variable_types>"
#define XML_CLOSE_VARIABLE_TYPES "</variable_types>"

#define XML_OPEN_VARNAMES "<varnames>"
#define XML_CLOSE_VARNAMES "</varnames>"

#define XML_OPEN_SORTLIST "<sortlist>"
#define XML_CLOSE_SORTLIST "</sortlist>"

#define XML_OPEN_FORMATS "<formats>"
#define XML_CLOSE_FORMATS "</formats>"

#define XML_OPEN_VALUE_LABEL_NAMES "<value_label_names>"
#define XML_CLOSE_VALUE_LABEL_NAMES "</value_label_names>"

#define XML_OPEN_VARIABLE_LABELS "<variable_labels>"
#define XML_CLOSE_VARIABLE_LABELS "</variable_labels>"

#define XML_OPEN_CHARACTERISTICS "<characteristics>"
#define XML_CLOSE_CHARACTERISTICS "</characteristics>"

#define XML_OPEN_DATA "<data>"
#define XML_CLOSE_DATA "</data>"

#define XML_OPEN_STRLS "<strls>"
#define XML_CLOSE_STRLS "</strls>"

#define XML_OPEN_VALUE_LABELS "<value_labels>"
#define XML_CLOSE_VALUE_LABELS "</value_labels>"

#define XML_OPEN_INNER_VALUE_LABELS "<lbl>"
#define XML_CLOSE_INNER_VALUE_LABELS "</lbl>"

#endif


