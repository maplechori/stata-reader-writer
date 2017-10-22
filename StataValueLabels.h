/* 
 * File:   StataVariables.h
 * Author: adrianmo
 *
 * Created on Oct 22, 2017, 5:22 AM
 */

#ifndef STATAVALUELABELS_H
#define	STATAVALUELABELS_H

#include <string>

class ValueLabelTable {

};


class StataValueLabel {
    public:
         StataValueLabel();
         ~StataValueLabel();

    private:
        int lenvtable;
        std::string labname;
        int padding;
        



};



#endif	/* STATAVALUELABELS_H */