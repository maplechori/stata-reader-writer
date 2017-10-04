#include "State.h"

Context::Context(char * cursor) : start(0) {
   this->cursor = cursor;
   this->currentState = new OpenHeader();

   while(1)
     this->advance();
}

void Context::advance() {

    for (start = cursor; cursor && *cursor != '>'; cursor++);

    if (cursor && *cursor == '>') {
      cursor++;
      strncpy(buffer, start, cursor - start);
      buffer[cursor-start]='\0';
    }


    if (this->currentState->check(this->buffer))
        this->currentState = this->currentState->advanceState();
     
}


bool OpenHeader::check(char * buffer) {
  
     if (!strcasecmp(buffer, XML_HEADER))
     {
          cout << "Seems we found a STATA header" << endl;
          return true;
     }

     return false;
}

State * OpenHeader::advanceState() {
  return new OpenRelease();
}


bool OpenRelease::check(char * buffer)
{
     if (!strcasecmp(buffer, XML_RELEASE))
     {
        cout << "Release" << endl;
        return true;
 
     }

     return false;
}


