#include "State.h"

Context::Context(char * cursor) : start(0) {
   this->cursor = cursor;
   this->currentState = new OpenDTA();
}

void Context::advance() {

    for (start = cursor; cursor && *cursor != '>'; cursor++);

    if (cursor && *cursor == '>') {
      cursor++;
      strncpy(buffer, start, cursor - start);
      buffer[cursor-start]='\0';
    }

    while(this->currentState->check(this->buffer))
    {   
        currentState->process(*this);
        this->currentState = this->currentState->advanceState();
    }
}

// OpenDTA State

bool OpenDTA::process(Context & ctx) 
{
   ctx.advance();
   return true;
}


State * OpenDTA::advanceState() {
    return new OpenHeader();
}

// OpenHeader State 
bool OpenHeader::check(char * buffer) {
  
     if (!strcasecmp(buffer, XML_OPEN_HEADER))
          return true;

     return false;
}

bool OpenHeader::process(Context & ctx)
{
     ctx.advance();
     return true;
}

State * OpenHeader::advanceState() {
      return new OpenRelease();
}

// OpenRelease State
bool OpenRelease::check(char * buffer)
{
     if (!strcasecmp(buffer, XML_OPEN_RELEASE))
     {
        cout << "Release" << endl;
        return true;
     }

     return false;
}

State * OpenRelease::advanceState() {
  return new OpenByteOrder();
}

bool OpenRelease::process(Context & ctx)
{

     string version = ctx.getChars(3);

     switch(strtol(version.c_str(), NULL, 10))
     {
	 case 117: 
               ctx.hdr.fileRelease = R117;
               break;

         default:
               ctx.hdr.fileRelease = R117;
               break;

     }
     
     ctx.advance(); // CONTENT 
     ctx.advance(); // </release>
     return true;
}

// OpenByteOrder State

bool OpenByteOrder::check(char * buffer) 
{
     if (!strcasecmp(buffer, XML_OPEN_BYTEORDER))
     {
       cout << "byteOrder" << endl;      
       return true;
     }

    return false;
}

State * OpenByteOrder::advanceState() 
{
  cout << "here" << endl;
  return NULL;
}

bool OpenByteOrder::process(Context & ctx) 
{
        return true;
}
