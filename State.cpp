#include "State.h"

Context::Context(char * cursor) : start(0), strls(false) {
   this->cursor = cursor;
   this->currentState = new OpenDTA();
}

void Context::exportToDB(char * filename)
{

}

void Context::advanceCursor(int c)
{
  cursor += c;

  for (start = cursor; cursor && *cursor != '>'; cursor++);
  
      if (cursor && *cursor == '>') {
        cursor++;
        memset(buffer, 0, sizeof(buffer));
        strncpy(buffer, start, cursor - start); /* buffer overflow */
        buffer[cursor-start]='\0';
      } 
}

void Context::advanceNoState()
{
    for (start = cursor; cursor && *cursor != '>'; cursor++);

    if (cursor && *cursor == '>') {
      cursor++;
      memset(buffer, 0, sizeof(buffer));
      strncpy(buffer, start, cursor - start); /* buffer overflow */
      buffer[cursor-start]='\0';
    } 
}


void * Context::advance() {

    for (start = cursor; cursor && *cursor != '>'; cursor++);
  
    if (cursor && *cursor == '>') {
      cursor++;
      memset(buffer, 0, sizeof(buffer));
      strncpy(buffer, start, cursor - start); /* buffer overflow */
      buffer[cursor-start]='\0';
    }

    while(currentState->check(buffer))
    {   
        currentState->process(*this);
        currentState = currentState->advanceState();
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

bool CloseDTA::process(Context & ctx) 
{
   ctx.advance();
   return true;
}

State * CloseDTA::advanceState() { 
   return NULL;
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
        case 119:
        case 118:
              ctx.hdr.fileRelease = R119;
              break;
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
  return new OpenVarTypes();
}

bool OpenMap::process(Context & ctx)
{
    #define MAP_COUNT 14
    char * ctxbuf = (char *)ctx.advance();
    int i, j;
   
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
        ctx.map.stata_map[map_names[i].c_str()] = GetLSF<uint64_t>(ctxbuf, 8);   
    
    ctx.advance();

    return true;

}

State * OpenVarTypes::advanceState()
{
  return new OpenVarNames();
}

bool OpenVarTypes::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();  
  int curr = 0;

  if (ctx.hdr.fileByteorder == LSF) {
      
     while (*ctxbuf != '<') {
          StataVariables * sta = new StataVariables();          
          sta->type = GetLSF<unsigned int>(ctxbuf, 2);
          ctx.vList.push_back(sta);
          ctxbuf += 2;
          curr++;
      }

      cout << "Total variables in dataset: " << curr << endl;

  } else {
      // not implemented yet
  }

  ctx.advance();
  return true;
}



State * OpenVarNames::advanceState()
{
  return new OpenSortList();
}

bool OpenVarNames::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();
  int sz = 0;
  int curr = 0;

  if (ctx.hdr.fileByteorder == LSF) {

      while (*ctxbuf != '<') {
     
          switch(ctx.hdr.fileRelease)
          { 
            // UTF-8
            case R119:
            case R118:
                      sz = strlen(ctxbuf);
                      /*wcslen((wchar_t *)ctxbuf);*/
                      ((StataVariables *)ctx.vList.at(curr))->varname.assign(&ctxbuf[0],sz);
                      ctxbuf += 129;
                      break;
            // ASCII 
            case R117:
                      sz = strlen(ctxbuf);
                      ((StataVariables *)ctx.vList.at(curr))->varname.assign(&ctxbuf[0],sz);
                      ctxbuf += 33;
                      break;
          }

          
          curr++;
      }
  } else {
      // not implemented yet
  }

  ctx.advance();

}



State * OpenSortList::advanceState()
{
  return new OpenFormats();
}

bool OpenSortList::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();    
  
  // We don't really need the sort list feature for the moment
  cout << "Ignoring sortlist" << endl;
  ctx.advance(); // </sortlist>

}

State * OpenFormats::advanceState()
{
  return new OpenValueLabelNames();
}

bool OpenFormats::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();    
  int curr = 0, sz = 0;

  if (ctx.hdr.fileByteorder == LSF) {

      while (*ctxbuf != '<') {  
          
          switch(ctx.hdr.fileRelease)
          { 
            // UTF-8
            case R119:
            case R118:
                //sz = wcslen((wchar_t *)ctxbuf);
                sz = strlen((char *)ctxbuf);                
                ((StataVariables *)ctx.vList.at(curr))->format.assign(&ctxbuf[0],sz);              
                cout << ((StataVariables *)ctx.vList.at(curr))->format << endl;                                
                ctxbuf += 57;
                break;

            // ASCII
            case R117:
                sz = strlen((char *)ctxbuf);
                ((StataVariables *)ctx.vList.at(curr))->format.assign(&ctxbuf[0],sz);  
                cout << ((StataVariables *)ctx.vList.at(curr))->format << endl;                
                ctxbuf += 49;
                break;
          }

          curr++;
      }

  } 
  else 
  {
      // not implemented yet
  }

  ctx.advance();

}

State * OpenValueLabelNames::advanceState()
{
  return new OpenVariableLabels();
}

bool OpenValueLabelNames::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();      
  int curr = 0, sz = 0;

  if (ctx.hdr.fileByteorder == LSF) {

      while (*ctxbuf != '<') {
        
          switch(ctx.hdr.fileRelease)
          { 
            // UTF-8
            case R119:
            case R118:
                //sz = wcslen((wchar_t *)ctxbuf);
                sz = strlen((char *)ctxbuf);
                ((StataVariables *)ctx.vList.at(curr))->vallbl.assign(&ctxbuf[0],sz);              
                ctxbuf += 129;
                break;

            // ASCII
            case R117:
                sz = strlen((char *)ctxbuf);
                ((StataVariables *)ctx.vList.at(curr))->vallbl.assign(&ctxbuf[0],sz);  
                ctxbuf += 33;
                break;
          }          
                 
          curr++;
      }
  } else {
      // not implemented yet
  }

  ctx.advance();
}

State * OpenVariableLabels::advanceState()
{
  return new OpenCharacteristics();
}

bool OpenVariableLabels::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();      
  int curr = 0, sz = 0;
  cout << "OpenVariableLabels" << endl;

  if (ctx.hdr.fileByteorder == LSF) {
    
    while (*ctxbuf && *ctxbuf != '<') {

      switch(ctx.hdr.fileRelease)
      {
          case R119:
          case R118:

            if (ctxbuf[0] != 0) {
              //sz = wcslen((wchar_t *)ctxbuf);
              sz = strlen((char *)ctxbuf);
              ((StataVariables *)ctx.vList.at(curr))->varlbl.assign(&ctxbuf[0],sz);  
              cout << ctx.vList.at(curr)->varname << " " << ctx.vList.at(curr)->varlbl << ctx.vList.at(curr)->format << endl;
            }
  
            ctxbuf += 321;
            break;

          case R117:
          
            if (ctxbuf[0] != 0) {
              sz = strlen((char *)ctxbuf);
              ((StataVariables *)ctx.vList.at(curr))->varlbl.assign(&ctxbuf[0],sz);  
              cout << ctx.vList.at(curr)->varname << " " << ctx.vList.at(curr)->varlbl << " " << ctx.vList.at(curr)->format << endl;
            }

            ctxbuf += 81;
            break;

          default:
            break;
      }

      curr++;
    }                                                  
  } 
  else 
  {
          // not implemented yet
  }

  ctx.advance();
}


State * OpenCharacteristics::advanceState()
{
  if (getHasCharacteristics())
    return new OpenCH();
  else
    return new OpenData(); 
}

bool OpenCharacteristics::process(Context & ctx)
{
    char * ctxbuf = (char *) ctx.getCursor();      
  
    if (ctxbuf[0] == '<' && ctxbuf[1] == '/')  
    {
       setHasCharacteristics(false);
       ctx.advance();
       ctx.advance();
    }
    else
    {
       setHasCharacteristics(true);
       ctx.advanceNoState();
    }    
}

State * OpenCH::advanceState()
{
   return new CloseCH();
}

bool OpenCH::process(Context & ctx)
{
    char * ctxbuf = ctx.getCursor();
    int count = 0;

    if (ctx.hdr.fileByteorder == LSF) {
      
      while (ctxbuf) { 
    
          if (ctxbuf[0] == '<' && 
              ctxbuf[1] == '/' && 
              ctxbuf[2] == 'c' && 
              ctxbuf[3] == 'h' && 
              ctxbuf[4] == '>' && 
              ctxbuf[5] == '<')
          {
            ctx.advanceCursor(count);
            ctxbuf = ctx.getCursor();
            return true; 
          }
          
          ctxbuf++;
          count++;
          
      }
    }
    else
    {

    }

    
}

State * CloseCH::advanceState()
{
  if (getHasCharacteristics())
    return new OpenCH();
  else
    return new OpenData();
}

bool CloseCH::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance(); 

  if (ctxbuf[0] == '<' && ctxbuf[1] == 'c' && ctxbuf[2] == 'h' && ctxbuf[3] == '>')
    setHasCharacteristics(true);
  else
  {
    setHasCharacteristics(false);
    ctx.advance();
  }
}

State * OpenData::advanceState()
{
  return new OpenSTRL();
}

bool OpenData::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();      
  int curr = 0, sz = 0;
  
  if (ctx.hdr.fileByteorder == LSF) {
      
        for (vector<StataVariables *>::iterator it = ctx.vList.begin();  *ctxbuf != '<' && it != ctx.vList.end(); ++it)
        {
          switch((*it)->type)
          {
              case ST_STRL:
                      cout << "STRL" << endl;
                      ctx.strls = true;
                      // handle STRLs

                      break;
              case ST_DOUBLE:
                      cout << "DOUBLE" << endl;
                      ctxbuf += 8;
                      break;
              case ST_FLOAT:
                      cout << "FLOAT" << endl;
                      ctxbuf += 4;
                      break;
              case ST_LONG:
                      cout << "LONG" << endl;
                      ctxbuf += 4;
                      break;
              case ST_INT:
                      cout << "INTEGER" << endl;
                      ctxbuf += 2;
                      break;
              case ST_BYTE:              
                      cout << "BYTE" << endl;
                      ctxbuf++;
                      break;
              default:
                      if ((*it)->type > 0 && (*it)->type <= 2045) {
                          cout << "STRING OF LENGTH " << (*it)->type << endl;
                          ctxbuf += (*it)->type;
                      }
                      else
                          cout << "UNKNOWN TYPE" << endl;
                      break;
              }
              
      }
        
    }
    else
    {
       // not implemented yet
    }

  ctx.advance();
}

State * OpenSTRL::advanceState()
{
  return new OpenValueLabel();
}

bool OpenSTRL::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();      

  if (!ctx.strls)
  {
    cout << "no strls, next" << endl;
    return ctx.advance() && true;
  }
  else
      {
        // not implemented
      }

      return false;
}

State * OpenValueLabel::advanceState()
{
  if (getHasLabels())
    return new OpenInnerValueLabel();
  else
    return new CloseValueLabel();

 
}

bool OpenValueLabel::process(Context & ctx)
{
  char * ctxbuf = (char *) ctx.advance();      
  
  if (ctxbuf[0] == '<' && ctxbuf[1] == '/')  
      setHasLabels(false);
  else
      setHasLabels(true);

}

State * CloseValueLabel::advanceState()
{
  return NULL;
}

bool CloseValueLabel::process(Context & ctx)
{
    ctx.advance();
}


State * OpenInnerValueLabel::advanceState()
{
  return new CloseDTA();
}

bool OpenInnerValueLabel::process(Context & ctx)
{
    char * ctxbuf = (char *) ctx.advance(); 
    char * txtorig = NULL;      
    int curr = 0, sz = 0, entries = 0, txtlen = 0, * offsets = NULL;
    StataValueLabel * svl = new StataValueLabel();

     if (ctx.hdr.fileByteorder == LSF) {
        
        while (*ctxbuf != '<') {
          
          switch(ctx.hdr.fileRelease)
          {
             
            case R119:
            case R118:

              break;

            case R117:  

                    ctxbuf += 4;
                    sz = strlen((char *)ctxbuf);
                    svl->labname.assign(&ctxbuf[0], sz);
                    cout << svl->labname << endl;
                    ctxbuf += 36;
                    entries = GetLSF<unsigned int>(ctxbuf, 4);
                    ctxbuf += 4;
                    txtlen = GetLSF<unsigned int>(ctxbuf, 4);
                    ctxbuf += 4;

                    offsets = new int[entries]; 

                    for (int i = 0; i < entries; i++, ctxbuf += 4)
                      offsets[i] = GetLSF<int>(ctxbuf, 4);

                    txtorig = ctxbuf + (entries * 4);
                    
                    for(int i = 0; i < entries; i++, ctxbuf += 4)
                    {
                        int _of = (i+1 < entries) ? offsets[i+1] - offsets[i] : txtlen - offsets[i];
                        svl->valuelabel[GetLSF<unsigned int>(ctxbuf, 4)].assign((char *)(&txtorig[offsets[i]]), _of );
                        cout << svl->valuelabel[GetLSF<unsigned int>(ctxbuf, 4)] << endl;
                    }

                    ctxbuf = txtorig + txtlen;
              break;

           }
        }
      }
      else
      {

      }
        

      ctx.advance();
}