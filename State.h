#ifndef __STATE__H__
#define __STATE__H__

#include <vector>
#include <string.h>
#include <memory>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include "StateBitops.h"
#include "StataMap.h"
#include "StataValueLabel.h"
#include "StataDB.h"

using namespace std;

class Context;

#define CHECK_TAG(tag) ((!strcasecmp(buffer, tag)) ? true : false)
#define INITIAL_BUFFER 1024

class State
{
public:
  virtual State *advanceState() = 0;
  virtual bool check(char *) = 0;
  virtual bool process(Context &ctx) = 0;
  virtual ~State() {};
};

class Context
{

public:
  Context(char *cursor, int length);
  virtual ~Context();
  void *advance();
  StataHeader hdr;
  StataMap map;
  State *currentState;
  bool strls;
  char *origin;
  vector< boost::shared_ptr<StataVariables> > vList;
  vector < vector < char *  > > vData;
  char *getCursor() { return cursor; }
  void clearCursor() { cursor = NULL; }
  void advanceNoState();
  void advanceCursor(int c);
  void advanceData(int c);  

  string getChars(int count)
  {
    string tmp;
    tmp.assign(cursor, count);
    cursor += count;
    return tmp;
  };

  void resizeBuffer(int sz)
  {
      delete [] buffer;
      buffer_size = sz + 1;
      buffer = new char[buffer_size];
  };

  int exportToDB(DatabaseTypes db_type, void * params);

private:
  char *buffer;
  int buffer_size;
  char *cursor;
  char *start;
  int length;
};

class OpenDTA : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_FILE); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenDTA() {};
};

class CloseDTA : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_CLOSE_FILE); }
  State *advanceState();
  bool process(Context &ctx);
  ~CloseDTA() {};
};

class OpenHeader : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_HEADER); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenHeader() {};
};

class CloseHeader : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_CLOSE_HEADER); }
  State *advanceState();
  bool process(Context &ctx);
  ~CloseHeader() {};
};

class OpenRelease : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_RELEASE); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenRelease() {};
};

class CloseRelease : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_CLOSE_RELEASE); }
  State *advanceState();
  bool process(Context &ctx);
  ~CloseRelease() {};
};

class OpenByteOrder : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_BYTEORDER); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenByteOrder() {};
};

class OpenK : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VARIABLES_COUNT); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenK() {};
};

class OpenN : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_OBSERVATIONS_COUNT); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenN() {};
};

class OpenLabel : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_LABEL); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenLabel() {};
};

class OpenCH : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_CH); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenCH() {};
};

class CloseCH : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_CLOSE_CH); }
  State *advanceState();
  bool process(Context &ctx);
  void setHasCharacteristics(bool v) { hasCharacteristics = v; };
  bool getHasCharacteristics() { return hasCharacteristics; };
  ~CloseCH() {};
private:
  bool hasCharacteristics;
};

class OpenTimeStamp : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_TIMESTAMP); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenTimeStamp() {};
};

class OpenMap : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_MAP); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenMap() {};
};

class OpenVarTypes : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VARIABLE_TYPES); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenVarTypes() {};
};

class OpenVarNames : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VARNAMES); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenVarNames() {};
};

class OpenSortList : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_SORTLIST); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenSortList() {};
};

class OpenFormats : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_FORMATS); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenFormats() {};
};

class OpenValueLabelNames : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VALUE_LABEL_NAMES); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenValueLabelNames() {};
};

class OpenVariableLabels : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VARIABLE_LABELS); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenVariableLabels() {};
};

class OpenCharacteristics : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_CHARACTERISTICS); }
  State *advanceState();
  bool process(Context &ctx);
  void setHasCharacteristics(bool v) { hasCharacteristics = v; };
  bool getHasCharacteristics() { return hasCharacteristics; };
  ~OpenCharacteristics() {};
private:
  bool hasCharacteristics;
};

class OpenData : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_DATA); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenData() {};
};

class OpenSTRL : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_STRLS); }
  State *advanceState();
  bool process(Context &ctx);
  ~OpenSTRL() {};
};

class OpenValueLabel : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_VALUE_LABELS); }
  State *advanceState();
  bool process(Context &ctx);
  void setHasLabels(bool v) { hasLabels = v; };
  bool getHasLabels() { return hasLabels; };
  ~OpenValueLabel() {};
private:
  bool hasLabels;
};

class CloseValueLabel : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_CLOSE_VALUE_LABELS); }
  State *advanceState();
  bool process(Context &ctx);
  ~CloseValueLabel() {};
};

class OpenInnerValueLabel : public State
{
  bool check(char *buffer) { return CHECK_TAG(XML_OPEN_INNER_VALUE_LABELS); }
  State *advanceState();
  bool process(Context &ctx);
  void setHasMoreLabels(bool v) { hasMoreLabels = v; };
  bool getHasMoreLabels() { return hasMoreLabels; };
  ~OpenInnerValueLabel() {};
private:
  bool hasMoreLabels;
};

#endif