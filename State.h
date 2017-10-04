
#include <vector>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include <string.h>

using namespace std;
class Context;


class State
{
   public: 
     virtual State * advanceState() = 0;
     virtual bool check(char *) = 0;
     virtual bool process(Context & ctx) = 0;
};

class Context {
   
   public: 
     Context(char * cursor);
     void advance();
   
   private:
     char buffer[4096];
     char * cursor;
     char * start;
     vector<StataVariables *> vList;
     StataHeader hdr;
     State * currentState;
     
};

class OpenHeader : public State {
       bool check(char *);
       State * advanceState();   
       bool process(Context & ctx);
};


class OpenRelease : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenByteOrder : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenK : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenN : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenLabel : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenTimeStamp : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenMap : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVarTypes : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVarNames : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenSortList : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenFormats : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenValueLabels : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVariableLabels : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenCharacteristics : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};

class OpenData : public State {
       bool check(char *);
       State * advanceState();
       bool process(Context & ctx);
};


