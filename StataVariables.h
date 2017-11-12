/* 
 * File:   StataVariables.h
 * Author: adrianmo
 *
 * Created on May 28, 2014, 10:50 AM
 */

#ifndef STATAVARIABLES_H
#define STATAVARIABLES_H

#include <string>
#include <iostream>

using namespace std;

class StataVariables
{
  public:
    std::string varname;
    int type;
    std::string format;
    std::string vallbl;
    std::string varlbl;
};

template <class T>
class StataVariablesImpl : public StataVariables
{
  public:
    StataVariablesImpl(){};
    StataVariablesImpl(const StataVariablesImpl &orig){};
    virtual ~StataVariablesImpl() { cout << "cleaning" << endl; };
    T getValue() { return value; };
    void setValue(T t) { value = t; }

  private:
    T value;
};

#endif /* STATAVARIABLES_H */
