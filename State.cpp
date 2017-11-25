#include "State.h"

Context::Context(char *cursor, int length) : start(0), strls(false)
{
  this->origin = cursor;
  this->cursor = cursor;
  this->length = length;
  this->currentState = new OpenDTA();
  this->buffer = new char[INITIAL_BUFFER];
  this->buffer_size = INITIAL_BUFFER;
}

Context::~Context()
{
  if (currentState)
    delete currentState;
  
  if (buffer)
    delete buffer;
}


int Context::exportToDB(char *filename)
{
 
}

void Context::advanceData(int c)
{
  int curr_length=0, curr_position = 0;
  char *prev_cursor = cursor;
 
  for (start = cursor; curr_position <= c; cursor++, curr_position++);
 
  curr_length = cursor - prev_cursor;

  if (curr_length > buffer_size)
    resizeBuffer(curr_length);

  memset(buffer, 0, buffer_size);
  strncpy(buffer, start, curr_length);

}

void Context::advanceNoState()
{
  int curr_length=0;
  int i = 0;


  for (start = cursor; cursor && (cursor - origin) < length && *cursor != '>'; cursor++);

  if (cursor && *cursor++ == '>')
  {
    curr_length = cursor - start;
        
    if (curr_length > buffer_size)
      resizeBuffer(curr_length);

    memset(buffer, 0, buffer_size);
    strncpy(buffer, start, curr_length);
  }

}

void Context::advanceCursor(int c)
{
  cursor += c;
  advanceNoState();
}


void * Context::advance()
{
  advanceNoState();

  while (currentState && currentState->check(buffer))
  {
      currentState->process(*this);

      if (currentState)
      {
        State * newState = currentState->advanceState();
        delete currentState;
        currentState = newState;
      }
  }
  
  return (void *)(start);
}

// OpenDTA State
bool OpenDTA::process(Context &ctx)
{
  ctx.advance();
  return true;
}

State *OpenDTA::advanceState()
{
  return new OpenHeader();
}

bool CloseDTA::process(Context &ctx)
{
  return false;
}

State *CloseDTA::advanceState()
{
  return NULL;
}

// OpenHeader State
bool OpenHeader::process(Context &ctx)
{
  ctx.advance();
  return true;
}

State *OpenHeader::advanceState()
{
  return new OpenRelease();
}

// OpenRelease State
State *OpenRelease::advanceState()
{
  return new OpenByteOrder();
}

bool OpenRelease::process(Context &ctx)
{
  string version = ctx.getChars(3);
 
  switch (strtol(version.c_str(), NULL, 10))
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
State *OpenByteOrder::advanceState()
{
  return new OpenK();
}

bool OpenByteOrder::process(Context &ctx)
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
State *OpenK::advanceState()
{
  return new OpenN();
}

bool OpenK::process(Context &ctx)
{

  if (ctx.hdr.fileByteorder == LSF)
  {
    char *ctxbuf = (char *)ctx.advance();

   

    switch (ctx.hdr.fileRelease)
    {
    // 4 byte
    case R119:
      if (ctxbuf[2] == '<' && ctxbuf[3] == '/')
        ctx.hdr.variables = GetLSF<uint16_t>(ctxbuf, 2);
      else 
        ctx.hdr.variables = GetLSF<uint32_t>(ctxbuf, 4);
      break;

    // 2 byte
    case R118:
    case R117:
      ctx.hdr.variables = GetLSF<uint16_t>(ctxbuf, 2);
      break;

    default:

      break;
    }
  }
  else
  {
    // MSF not implemented yet
  }

  ctx.advance();

  return true;
}

// OpenN State
State *OpenN::advanceState()
{
  return new OpenLabel();
}

bool OpenN::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();

  if (ctx.hdr.fileByteorder == LSF)
  {
    switch (ctx.hdr.fileRelease)
    {
    // 8 byte
    case R119:
    case R118:
      ctx.hdr.observations = GetLSF<uint64_t>(ctxbuf, 8);
      break;

    // 4 byte
    case R117:
      ctx.hdr.observations = GetLSF<uint32_t>(ctxbuf, 4);
      break;

    default:
      break;
    }
  }
  else
  {
    // MSF not implemented yet
  }


  ctx.advance();
  return true;
}

// OpenLabel State
State *OpenLabel::advanceState()
{
  return new OpenTimeStamp();
}

bool OpenLabel::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int label_count = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {
    switch (ctx.hdr.fileRelease)
    {
    case R119:
    case R118:
      label_count = GetLSF<int16_t>(ctxbuf, 2);
      ctx.hdr.datalabel.assign(&ctxbuf[2], label_count);
      break;

    case R117:
      label_count = GetLSF<int8_t>(ctxbuf, 1);
      ctx.hdr.datalabel.assign(&ctxbuf[1], label_count);
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

State *OpenTimeStamp::advanceState()
{
  return new CloseHeader();
}

bool OpenTimeStamp::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int label_count = 0;

  switch (ctx.hdr.fileRelease)
  {

  case R119:
  case R118:
  case R117:
    label_count = GetLSF<int8_t>(ctxbuf, 1);
    ctx.hdr.ts.assign(&ctxbuf[1], label_count);
    break;
  case R115:
  case R114:
  case R113:
  case R112:
    break;
  }

  ctx.advance();
  return true;
}

State *CloseHeader::advanceState()
{
  return new OpenMap();
}

bool CloseHeader::process(Context &ctx)
{
  ctx.advance();
  return true;
}

State *OpenMap::advanceState()
{
  return new OpenVarTypes();
}

bool OpenMap::process(Context &ctx)
{
#define MAP_COUNT 14
  char *ctxbuf = (char *)ctx.advance();
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
      "eof"};

  for (i = 0; i < MAP_COUNT; i++, ctxbuf += 8)
    ctx.map.stata_map[map_names[i].c_str()] = GetLSF<uint64_t>(ctxbuf, 8);

  ctx.advance();

  return true;
}

State *OpenVarTypes::advanceState()
{
  return new OpenVarNames();
}

bool OpenVarTypes::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int curr = 0;
  StataVariablesImpl<double> *sta_double;
  StataVariablesImpl<float> *sta_float;
  StataVariablesImpl<long> *sta_long;
  StataVariablesImpl<short> *sta_integer;
  StataVariablesImpl<int8_t> *sta_byte;
  StataVariablesImpl<string> *sta_char;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (*ctxbuf != '<')
    {
      int stataType = GetLSF<unsigned int>(ctxbuf, 2);

      switch (stataType)
      {
      case ST_STRL:
        cout << "STRL" << endl;

        // handle STRLs
        break;
      case ST_DOUBLE:
        sta_double = new StataVariablesImpl<double>();
        sta_double->type = stataType;
        ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_double));
        break;
      case ST_FLOAT:
        sta_float = new StataVariablesImpl<float>();
        sta_float->type = stataType;
        ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_float));
        break;
      case ST_LONG:
        sta_long = new StataVariablesImpl<long>();
        sta_long->type = stataType;
        ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_long));
        break;
      case ST_INT:
        sta_integer = new StataVariablesImpl<short>();
        sta_integer->type = stataType;
        ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_integer));
        break;
      case ST_BYTE:
        sta_byte = new StataVariablesImpl<int8_t>();
        sta_byte->type = stataType;
        ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_byte));
        break;
      default:
        if (stataType > 0 && stataType <= 2045)
        {
          sta_char = new StataVariablesImpl<string>();
          sta_char->type = stataType;
          ctx.vList.push_back(boost::shared_ptr<StataVariables>(sta_char));
        }
        else
          cout << "UNKNOWN TYPE " << hex << stataType << endl;
        break;
      }

      ctxbuf += 2;
      curr++;
    }
  }
  else
  {
    // not implemented yet
  }

  ctx.advance();
  return true;
}

State *OpenVarNames::advanceState()
{
  return new OpenSortList();
}

bool OpenVarNames::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int sz = 0;
  int curr = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (*ctxbuf != '<')
    {

      switch (ctx.hdr.fileRelease)
      {
      // UTF-8
      case R119:
      case R118:
        sz = strlen(ctxbuf);
        /*wcslen((wchar_t *)ctxbuf);*/
        (ctx.vList.at(curr))->varname.assign(&ctxbuf[0], sz);
        ctxbuf += 129;
        break;
      // ASCII
      case R117:
        sz = strlen(ctxbuf);
        (ctx.vList.at(curr))->varname.assign(&ctxbuf[0], sz);
        ctxbuf += 33;
        break;

      case R115:
      case R114:
      case R113:
      case R112:
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

State *OpenSortList::advanceState()
{
  return new OpenFormats();
}

bool OpenSortList::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  ctx.advance(); // </sortlist>
}

State *OpenFormats::advanceState()
{
  return new OpenValueLabelNames();
}

bool OpenFormats::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int curr = 0, sz = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (*ctxbuf != '<')
    {

      switch (ctx.hdr.fileRelease)
      {
      // UTF-8
      case R119:
      case R118:
        //sz = wcslen((wchar_t *)ctxbuf);
        sz = strlen((char *)ctxbuf);
        (ctx.vList.at(curr))->format.assign(&ctxbuf[0], sz);
        // cout << (ctx.vList.at(curr))->format << endl;
        ctxbuf += 57;
        break;

      // ASCII
      case R117:
        sz = strlen((char *)ctxbuf);
        (ctx.vList.at(curr))->format.assign(&ctxbuf[0], sz);
        //cout << (ctx.vList.at(curr))->format << endl;
        ctxbuf += 49;
        break;

      case R115:
      case R114:
      case R113:
      case R112:
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

State *OpenValueLabelNames::advanceState()
{
  return new OpenVariableLabels();
}

bool OpenValueLabelNames::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int curr = 0, sz = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (*ctxbuf != '<')
    {

      switch (ctx.hdr.fileRelease)
      {
      // UTF-8
      case R119:
      case R118:
        //sz = wcslen((wchar_t *)ctxbuf);
        sz = strlen((char *)ctxbuf);
        (ctx.vList.at(curr))->vallbl.assign(&ctxbuf[0], sz);
        ctxbuf += 129;
        break;

      // ASCII
      case R117:
        sz = strlen((char *)ctxbuf);
        (ctx.vList.at(curr))->vallbl.assign(&ctxbuf[0], sz);
        ctxbuf += 33;
        break;

      case R115:
      case R114:
      case R113:
      case R112:
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

State *OpenVariableLabels::advanceState()
{
  return new OpenCharacteristics();
}

bool OpenVariableLabels::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  int curr = 0, sz = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (*ctxbuf && *ctxbuf != '<')
    {

      switch (ctx.hdr.fileRelease)
      {
      case R119:
      case R118:

        if (ctxbuf[0] != 0)
        {
          //sz = wcslen((wchar_t *)ctxbuf);
          sz = strlen((char *)ctxbuf);
          (ctx.vList.at(curr))->varlbl.assign(&ctxbuf[0], sz);
        }

        ctxbuf += 321;
        break;

      case R117:

        if (ctxbuf[0] != 0)
        {
          sz = strlen((char *)ctxbuf);
          (ctx.vList.at(curr))->varlbl.assign(&ctxbuf[0], sz);
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

State *OpenCharacteristics::advanceState()
{
  if (getHasCharacteristics())
    return new OpenCH();
  else
    return new OpenData();
}

bool OpenCharacteristics::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.getCursor();

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

State *OpenCH::advanceState()
{
  return new CloseCH();
}

bool OpenCH::process(Context &ctx)
{
  char *ctxbuf = ctx.getCursor();
  int count = 0;

  if (ctx.hdr.fileByteorder == LSF)
  {

    while (ctxbuf)
    {

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

State *CloseCH::advanceState()
{
  if (getHasCharacteristics())
    return new OpenCH();
  else
    return new OpenData();
}

bool CloseCH::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();

  if (ctxbuf[0] == '<' && ctxbuf[1] == 'c' && ctxbuf[2] == 'h' && ctxbuf[3] == '>')
    setHasCharacteristics(true);
  else
  {
    setHasCharacteristics(false);
    ctx.advance();
  }
}

State *OpenData::advanceState()
{
  cout << "advancing to OpenSTRL()" << endl;
  return new OpenSTRL();
}

bool OpenData::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.getCursor(), *start_ctx = NULL;
  int curr = 0, sz = 0;

  start_ctx = ctxbuf;
  
  if (ctx.hdr.fileByteorder == LSF)
  {
    vector<boost::shared_ptr<StataVariables> >::iterator it;
    
    for (int j = 0; j < ctx.hdr.observations; j++)
    {
      
      for (it = ctx.vList.begin(); it != ctx.vList.end(); ++it)
      {

        switch ((*it)->type)
        {
        case ST_STRL:
          cout << "STRL" << endl;
          ctx.strls = true;
          // handle STRLs
          break;
        case ST_DOUBLE:
          //cout << "DOUBLE[" << (*it)->varname << "]" << GetLSF<double>(ctxbuf, 8) << " " << (*it)->format << " " << (*it)->varlbl << endl;
          static_cast<StataVariablesImpl<double> *>((&*it)->get())->setValue(GetLSF<double>(ctxbuf, 8));   
          ctxbuf += 8;
          break;
        case ST_FLOAT:
          //cout << "FLOAT[" << (*it)->varname << "]" << GetLSF<float>(ctxbuf, 4) << " " << (*it)->format << " " << (*it)->varlbl << endl;
          static_cast<StataVariablesImpl<float> *>((&*it)->get())->setValue(GetLSF<float>(ctxbuf, 4));                  
          ctxbuf += 4;
          break;
        case ST_LONG:
          //cout << "LONG[" << (*it)->varname << "]" << GetLSF<long>(ctxbuf, 4) << " " << (*it)->format << " " << (*it)->varlbl << endl;
          static_cast<StataVariablesImpl<long> *>((&*it)->get())->setValue(GetLSF<long>(ctxbuf, 4));        
          ctxbuf += 4;
          break;
        case ST_INT:
          //cout << "INTEGER[" << (*it)->varname << "]" << GetLSF<short>(ctxbuf, 2) << " " << (*it)->format << " " << (*it)->varlbl << endl;
          static_cast<StataVariablesImpl<short> *>((&*it)->get())->setValue(GetLSF<short>(ctxbuf, 2));
          ctxbuf += 2;
          break;
        case ST_BYTE:
          //cout << "BYTE[" << (*it)->varname << "]" << (short)GetLSF<int8_t>(ctxbuf, 1) << " " << (*it)->format << " " << (*it)->varlbl << endl;
          static_cast<StataVariablesImpl<int8_t> *>((&*it)->get())->setValue(GetLSF<int8_t>(ctxbuf, 1));
          ctxbuf++;
          break;
        default:
          if ((*it)->type > 0 && (*it)->type <= 2045)
          {
            static_cast<StataVariablesImpl<string> *>((&*it)->get())->setValue(GetLSF<string>(ctxbuf, (*it)->type));
            //cout << "STRING OF LENGTH[" << (*it)->type << "][" << (*it)->varname << "] " << (static_cast<StataVariablesImpl<string> *>((&*it)->get())->getValue()) << " " << (*it)->format << " " << (*it)->varlbl << endl;
            ctxbuf += (*it)->type;
          }
          else
            cout << "UNKNOWN TYPE" << endl;
          break;
        }
      }

      //
      //ctx.advance();
    }
  }
  else
  {
    // not implemented yet
  }
 
  //ctx.advanceCursor(ctxbuf-start_ctx);  
  ctx.advanceData(ctxbuf-start_ctx);
  ctx.advance();
  ctx.advance();  
  return true;
}

State *OpenSTRL::advanceState()
{
  return new OpenValueLabel();
}

bool OpenSTRL::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();

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

State *OpenValueLabel::advanceState()
{
  if (getHasLabels())
    return new OpenInnerValueLabel();
  else
    return new CloseValueLabel();
}

bool OpenValueLabel::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();

  if (ctxbuf[0] == '<' && ctxbuf[1] == '/')
    setHasLabels(false);
  else
    setHasLabels(true);
}

State *CloseValueLabel::advanceState()
{
  return NULL;
}

bool CloseValueLabel::process(Context &ctx)
{
  ctx.advance();
}

State *OpenInnerValueLabel::advanceState()
{
  if (getHasMoreLabels())
    return new OpenInnerValueLabel();
  else
    return new CloseValueLabel();
}

bool OpenInnerValueLabel::process(Context &ctx)
{
  char *ctxbuf = (char *)ctx.advance();
  char *txtorig = NULL;
  int curr = 0, sz = 0, totalSize = 0, entries = 0, txtlen = 0, *offsets = NULL;
  StataValueLabel *svl = new StataValueLabel();

  if (ctx.hdr.fileByteorder == LSF)
  {

    switch (ctx.hdr.fileRelease)
    {

    case R119:
    case R118:

      totalSize = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;
      sz = strlen((char *)ctxbuf);
      svl->labname.assign(&ctxbuf[0], sz);
      //cout << svl->labname << endl;
      ctxbuf += 132;
      entries = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;
      txtlen = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;

      offsets = new int[entries];

      for (int i = 0; i < entries; ctxbuf += 4, i++)
        offsets[i] = GetLSF<int32_t>(ctxbuf, 4);

      txtorig = ctxbuf + (entries * 4);

      for (int i = 0; i < entries; i++, ctxbuf += 4)
        svl->valuelabel[GetLSF<uint32_t>(ctxbuf, 4)].assign((char *)(&txtorig[offsets[i]]), strlen(&txtorig[offsets[i]]));

      ctxbuf += txtlen;
      break;

    case R117:
      totalSize = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;
      sz = strlen((char *)ctxbuf);
      svl->labname.assign(&ctxbuf[0], sz);
      //cout << svl->labname << endl;
      ctxbuf += 36;
      entries = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;
      txtlen = GetLSF<uint32_t>(ctxbuf, 4);
      ctxbuf += 4;

      offsets = new int[entries];

      for (int i = 0; i < entries; ctxbuf += 4, i++)
        offsets[i] = GetLSF<int32_t>(ctxbuf, 4);

      txtorig = ctxbuf + (entries * 4);

      for (int i = 0; i < entries; i++, ctxbuf += 4)
        svl->valuelabel[GetLSF<uint32_t>(ctxbuf, 4)].assign((char *)(&txtorig[offsets[i]]), strlen(&txtorig[offsets[i]]));

      ctxbuf += txtlen;
      break;

    case R115:
    case R114:
    case R113:
    case R112:
      break;
    }
  }
  else
  {
  }

  if (ctxbuf[0] == '<' && ctxbuf[1] == '/' && ctxbuf[2] == 'l' && ctxbuf[6] == '<' && ctxbuf[8] != 'v')
  {
    setHasMoreLabels(true);
    ctx.advance();
    ctx.advance();
  }
  else
  {
    setHasMoreLabels(false);
    ctx.advance();
  }

  return true;
}