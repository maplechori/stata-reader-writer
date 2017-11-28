#include "State.h"

#ifndef __STATA_DB__
#define __STATA_DB__

enum DatabaseTypes {
    SQLITE = 0
};

class Context;

class Database
{
public:
  virtual bool connect(void *) = 0;
  virtual bool close() = 0;
  virtual bool write(Context & ctx) = 0;
  virtual ~Database() {};
};

#endif