
#include <vector>
#include <string.h>
#include <sqlite3.h>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include "StateBitops.h"
#include "StataMap.h"
#include "StataValueLabel.h"

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
     bool strls;
     vector<StataVariables *> vList;
     char * getbuffer() { return &buffer[0]; }

     string getChars(int count) 
     {  
         string tmp; 
         tmp.assign(cursor, count); 
         cursor += count; 
         return tmp;  
     };

     void exportToDB(char * filename);
   
   private:
     sqlite3 *db;
     char buffer[64000];
     char * cursor;
     char * start;
};


class OpenDTA : public State {
      bool check(char * buffer) { return CHECK_TAG(XML_OPEN_FILE); }
      State * advanceState();
      bool process(Context & ctx);
};


class CloseDTA : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_OPEN_FILE); }
  State * advanceState();
  bool process(Context & ctx);
};

class OpenHeader : public State {
       bool check(char * buffer) { return CHECK_TAG(XML_OPEN_HEADER); } 
       State * advanceState(); 
       bool process(Context & ctx);
};

class CloseHeader : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_CLOSE_HEADER); } 
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

class OpenSTRL : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_OPEN_STRLS); }
  State * advanceState();
  bool process(Context & ctx);
};

class OpenValueLabel : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_OPEN_VALUE_LABELS); }
  State * advanceState();
  bool process(Context & ctx);
  void setHasLabels(bool v) { hasLabels = v; };
  bool getHasLabels() { return hasLabels; };
  private:
    bool hasLabels;
};

class CloseValueLabel : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_CLOSE_VALUE_LABELS); }
  State * advanceState();
  bool process(Context & ctx);
};



class OpenInnerValueLabel : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_OPEN_INNER_VALUE_LABELS); }
  State * advanceState();
  bool process(Context & ctx);
};


class CloseInnerValueLabel : public State {
  bool check(char * buffer) { return CHECK_TAG(XML_CLOSE_INNER_VALUE_LABELS); }
  State * advanceState();
  bool process(Context & ctx);
};
