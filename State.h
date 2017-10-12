
#include <vector>
#include <string.h>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include "StateBitops.h"
#include "StataMap.h"


using namespace std;

class Context;

#define CHECK_TAG(tag) ((!strcasecmp(buffer, tag)) ? true : false) 


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
     void * advance();
     StataHeader hdr;
     StataMap map;
     State * currentState;
     vector<StataVariables *> vList;

     string getChars(int count) 
     {  
         string tmp; 
         tmp.assign(cursor, count); 
         cursor += count; 
         return tmp;  
     };
   
   private:
     char buffer[4096];
     char * cursor;
     char * start;
};


class OpenDTA : public State {
      bool check(char * buffer) { return CHECK_TAG(XML_OPEN_FILE); }
      State * advanceState();
      bool process(Context & ctx);
};

class OpenHeader : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_HEADER); } 
       State * advanceState(); 
       bool process(Context & ctx);
};


class OpenRelease : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_RELEASE); }
       State * advanceState();
       bool process(Context & ctx);
};

class CloseRelese : public State {
       bool check(char * buffer ) { return CHECK_TAG(XML_CLOSE_RELEASE); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenByteOrder : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_BYTEORDER); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenK : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VARIABLES_COUNT); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenN : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_OBSERVATIONS_COUNT); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenLabel : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_LABEL); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenTimeStamp : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_TIMESTAMP); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenMap : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_MAP); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVarTypes : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VARIABLE_TYPES); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVarNames : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VARNAMES); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenSortList : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_SORTLIST); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenFormats : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_FORMATS); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenValueLabelNames : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VALUE_LABEL_NAMES); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenVariableLabels : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VARIABLE_LABELS); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenCharacteristics : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_CHARACTERISTICS); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenData : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_DATA); }
       State * advanceState();
       bool process(Context & ctx);
};

class OpenValueLabel : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VALUE_LABELS); }
  State * advanceState();
  bool process(Context & ctx);
};

