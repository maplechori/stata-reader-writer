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
bool OpenHeader::process(Context & ctx)
{
     ctx.advance();
     return true;
}

State * OpenHeader::advanceState()
{
  return new OpenRelease();
}

// OpenRelease State
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
State * OpenByteOrder::advanceState() 
{
  return new OpenK();
}

bool OpenByteOrder::process(Context & ctx) 
{
      string byteOrder = ctx.getChars(3);

      cout << byteOrder << " Val" << endl;

      if (!strcasecmp(byteOrder.c_str(), XML_LSF))
        ctx.hdr.fileByteorder = MSF;
      else
        ctx.hdr.fileByteorder = LSF;
     
      ctx.advance(); // ORDER 
      ctx.advance(); // </byteorder>
      
      cout << "byteOrder: " << ctx.hdr.fileByteorder << endl;

      return true;
}

// OpenK State
State * OpenK::advanceState()
{
  return NULL;
}

bool OpenK::process(Context & ctx)
{
  return true;
}
