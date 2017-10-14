#include "State.h"

Context::Context(char * cursor) : start(0) {
   this->cursor = cursor;
   this->currentState = new OpenDTA();
}

void * Context::advance() {

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

    return (void *)(start);
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
     cout << version << endl;
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
 
      if (!strcasecmp(byteOrder.c_str(), XML_LSF))
        ctx.hdr.fileByteorder = LSF;
      else
        ctx.hdr.fileByteorder = MSF;
     
      ctx.advance(); // ORDER 
      ctx.advance(); // </byteorder>
      
      return true;
}

// OpenK State
State * OpenK::advanceState()
{
  return new OpenN();
}

bool OpenK::process(Context & ctx)
{
    
    if (ctx.hdr.fileByteorder == LSF) {
        char * ctxbuf = (char *) ctx.advance();
       
        switch(ctx.hdr.fileRelease)
        {
          // 4 byte
          case R119:
          case R118:
                ctx.hdr.variables = GetLSF<int>(ctxbuf, 4);
                break;

          // 2 byte
          case R117:
               ctx.hdr.variables = GetLSF<int>(ctxbuf, 2); 
               break;

          default: 

                break;
        }

    } else {
         // MSF not implemented yet
    } 

    ctx.advance();

    return true;
}

// OpenN State
State * OpenN::advanceState()
{
  return new OpenLabel();
}

bool OpenN::process(Context & ctx)
{
    char * ctxbuf = (char *) ctx.advance();
    
    if (ctx.hdr.fileByteorder == LSF) 
    {
      switch(ctx.hdr.fileRelease)
      {
        // 8 byte
        case R119:
        case R118:
              ctx.hdr.observations = GetLSF<uint64_t>(ctxbuf, 8);
              break;
        
        // 4 byte
        case R117:
            ctx.hdr.observations = GetLSF<int>(ctxbuf, 4); 
            break;

        default: 
             break;
       } 
    }
    else 
    {
        // MSF not implemented yet
    }
    
    cout << "Observations: " << ctx.hdr.observations << endl; 
    ctx.advance();
    return true;
}

// OpenLabel State
State * OpenLabel::advanceState()
{
  return new OpenTimeStamp();
}

bool OpenLabel::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();
  int label_count = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {
    switch(ctx.hdr.fileRelease)
    {
      case R119:
      case R118:
          label_count = GetLSF<int>(ctxbuf, 2); 
          ctx.hdr.datalabel.assign(&ctxbuf[2],label_count);
          break;
      
      case R117:
          label_count = GetLSF<int>(ctxbuf, 1);
          ctx.hdr.datalabel.assign(&ctxbuf[1],label_count);
          break;

      default: 
           break;
     } 
  }
  else
  {
      // not implemented yet
  }
  
  cout << "Label Count: " << label_count << " " << ctx.hdr.datalabel << endl;
  ctx.advance();

  return true;
}

State * OpenTimeStamp::advanceState()
{
  return new CloseHeader();
}

bool OpenTimeStamp::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();
  int label_count = 0;

  switch(ctx.hdr.fileRelease)
  {

    case R119:
    case R118:
    case R117:
        label_count = GetLSF<int>(ctxbuf, 1); 
        ctx.hdr.ts.assign(&ctxbuf[1],label_count);
        break;
  }

  cout << "timeStamp: " << ctx.hdr.ts << endl;
  ctx.advance();
  return true;
}

State * CloseHeader::advanceState()
{
  return new OpenMap();
}

bool CloseHeader::process(Context & ctx)
{
  ctx.advance();
  return true;
}

State * OpenMap::advanceState()
{
  return new OpenMap();
}

bool OpenMap::process(Context & ctx)
{
    #define MAP_COUNT 14
    char * ctxbuf = (char *)ctx.advance();
    int i, j;
    cout << "here" << endl;
   
    string map_names[] = {
        "stata_data_start", 
        "map",
        "variable_types",
        "varnames",
        "sortlist",
        "formats",
        "value_label_names",
        "variable_labels",
        "characteristics",
        "data",
        "strls",
        "value_labels",
        "stata_data_end",
        "eof"
    };

    for (i = 0; i < MAP_COUNT; i++, ctxbuf += 8)
    {
        ctx.map.stata_map[map_names[i].c_str()] = GetLSF<uint64_t>(ctxbuf, 8);   
        cout << map_names[i].c_str() << " " << ctx.map.stata_map[map_names[i].c_str()] << endl;
    }
    
    return true;

}