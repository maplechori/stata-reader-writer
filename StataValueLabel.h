/* 
 * File:   StataVariables.h
 * Author: adrianmo
 *
 * Created on Oct 22, 2017, 5:22 AM
 */

#ifndef STATAVALUELABELS_H
#define	STATAVALUELABELS_H

#include <string>
#include <map>


class StataValueLabel {

public:
         StataValueLabel();
         ~StataValueLabel();
        std::string labname;
        std::map<int, std::string> valuelabel;

};



#endif	/* STATAVALUELABELS_H */