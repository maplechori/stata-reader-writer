/* 
 * File:   StataVariables.h
 * Author: adrianmo
 *
 * Created on May 28, 2014, 10:50 AM
 */

#ifndef STATAVARIABLES_H
#define	STATAVARIABLES_H

#include <string>


class StataVariables {
public:
    StataVariables();
    StataVariables(const StataVariables& orig);
    virtual ~StataVariables();
    std::string varname;
    int type;
    std::string format;
    std::string vallbl;
    std::string varlbl;
    
private:

};

#endif	/* STATAVARIABLES_H */

