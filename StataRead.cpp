#include "StataRead.h"

using namespace boost;
using namespace std;
using std::cout;

static char *start = NULL;
static char buffer[4096];
static char *c = NULL;


static void advance() {

    for (start = c; c && *c != '>'; c++);

    if (c && *c == '>') {
        c++;
        strncpy(buffer, start, c - start);
        buffer[c - start] = '\0';
    }
}


int stata_read(char * c) 
{
    vector<StataVariables *> vList;
    StataHeader hdr;
    state currentState = OPEN_HEADER;


    if (!*c || *c != '<') {
        cout << "Unsupported file format" << endl;
        return 0;
    }

    advance();

    while (1) {

        advance();

        switch (currentState) {

            case OPEN_HEADER:
                if (!strcasecmp(buffer, XML_OPEN_HEADER)) {
                    cout << "Seems we found a STATA header" << endl;
                    currentState = OPEN_RELEASE;
                }
                break;

            case OPEN_RELEASE:

                if (!strcasecmp(buffer, XML_OPEN_RELEASE)) {
                    strncpy(buffer, c, 3);
                    buffer[3] = 0;
                    cout << "Release: " << buffer << endl;

                    hdr.fileRelease = R117;
                    currentState = OPEN_BYTEORDER;
                    advance();
                }
                break;

            case OPEN_BYTEORDER:
                if (!strcasecmp(buffer, XML_OPEN_BYTEORDER)) {
                    strncpy(buffer, c, 3);
                    buffer[3] = 0;
                    cout << "ByteOrder: " << buffer << endl;

                    if (!strcasecmp(buffer, XML_LSF))
                        hdr.fileByteorder = LSF;
                    else
                        hdr.fileByteorder = MSF;

                    currentState = OPEN_K;
                    advance();
                }

                break;

            case OPEN_K:

                if (!strcasecmp(buffer, XML_OPEN_VARIABLES_COUNT)) {
                    if (hdr.fileByteorder == LSF) {
                        short * x = (short *) c;
                        hdr.variables = *x;
                        cout << "Variables: " << hdr.variables << endl;
                    } else {
                        // not implemented yet
                    }
                    currentState = OPEN_N;
                    advance();

                }
                break;

            case OPEN_N:
                if (!strcasecmp(buffer, XML_OPEN_OBSERVATIONS_COUNT)) {
                    if (hdr.fileByteorder == LSF) {
                        int * x = (int *) c;
                        hdr.observations = *x;
                        cout << "Observations: " << hdr.observations << endl;
                    } else {
                        // not implemented yet
                    }
                    currentState = OPEN_LBL;
                    advance();
                }

                break;
            case OPEN_LBL:

                if (!strcasecmp(buffer, XML_OPEN_LABEL))
                {
                    if (hdr.fileByteorder == LSF)
                    {
                        unsigned char x = *(unsigned char *)c;
                        c++;
                        cout << "Datalabel Size: " << static_cast<int>(x) << endl;

                        hdr.datalabel = new char[x + 1];
                        hdr.datalabel = c; // , x);
                        //hdr.datalabel[x]=0;
                        cout << hdr.datalabel << endl;

                    }
                    else
                    {
                        // not implemented yet
                    }

                    currentState = OPEN_TS;
                    advance();

                }

                break;


            case OPEN_TS:

                if (!strcasecmp(buffer, XML_OPEN_TIMESTAMP)) {
                    char *t = c;
                    int i = 0;

                    unsigned char x = *(unsigned char*) c++;

                    while (*c != '<')
                    {
                        c++;
                        i++;
                    }

                    hdr.ts.assign(t + 1, i);
                    cout << "Size: " << static_cast<int> (x) << " Timestamp: " << hdr.ts << endl;
                    currentState = OPEN_MAP;
                    advance();

                }
                break;

            case OPEN_MAP:
                if (!strcasecmp(buffer, XML_OPEN_MAP)) {
                    // Map Size is 14 * 8 bytes
                    c += (14 * 8);
                }

                currentState = OPEN_VARTYPES;
                advance();

                break;

            case OPEN_VARTYPES:
                if (!strcasecmp(buffer, "<variable_types>")) {
                    int curr = 0;
                    if (hdr.fileByteorder == LSF) {
                        while (*c != '<') {
                            unsigned short * x = (unsigned short *) c;
                            StataVariables * sta = new StataVariables();
                            sta->type = static_cast<unsigned short> (*x);

                            vList.push_back(sta);
                            c += 2;
                            curr++;
                        }
                    } else {
                        // not implemented yet
                    }

                    currentState = OPEN_VARNAMES;
                    advance();


                }

                break;
           case OPEN_VARNAMES:
                if (!strcasecmp(buffer, "<varnames>")) {
                    int curr = 0;
                    if (hdr.fileByteorder == LSF) {

                        while (*c != '<') {
                            char buf[32];
                            strncpy(buf, c, 32);
                            cout << "VAR: " << buf << endl;
                            vList.at(curr)->varname = buf;
                            c += 33;
                            curr++;
                        }
                    } else {
                        // not implemented yet
                    }



                    advance();
                    currentState = OPEN_SORTLIST;

                }

                break;

            case OPEN_SORTLIST:
                if (!strcasecmp(buffer, "<sortlist>")) {

                    while(*c != '<')
                          c++;

                    // Ignore the sort list
                    currentState = OPEN_FORMATS;
                    advance();
                }

                break;
           case OPEN_FORMATS:

                if (!strcasecmp(buffer, "<formats>")) {
                    int curr = 0;
                    if (hdr.fileByteorder == LSF) {

                        while (*c != '<') {
                            char buf[50];
                            strncpy(buf, c, 50);

                            vList.at(curr)->format = buf;

                            c += 49;
                            curr++;
                        }
                    } else {
                        // not implemented yet
                    }



                    advance();
                    currentState = OPEN_VALLBL;


                }

            case OPEN_VALLBL:


                if (!strcasecmp(buffer, "<value_label_names>")) {
                    int curr = 0;
                    if (hdr.fileByteorder == LSF) {

                        while (*c != '<') {
                            char buf[33];
                            strncpy(buf, c, 33);

                            if (buf[0] != 0)
                            {
                                cout << "value labels: " << buf << endl;
                                vList.at(curr)->vallbl = buf;
                            }
                            c += 33;
                            curr++;
                        }
                    } else {
                        // not implemented yet
                    }
                    advance();
                    currentState = OPEN_VARLBL;


                }
                break;

            case OPEN_VARLBL:

                if (!strcasecmp(buffer, "<variable_labels>")) {
                    int curr = 0;
                    if (hdr.fileByteorder == LSF) {

                        while (*c != '<') {
                            char buf[81];
                            strncpy(buf, c, 81);

                            if (buf[0] != 0) {
                                vList.at(curr)->varlbl = buf;
                                cout << vList.at(curr)->varname << " " << vList.at(curr)->varlbl << vList.at(curr)->format << endl;
                            }

                            c += 81;
                            curr++;
                        }
                    } else {
                        // not implemented yet
                    }



                    advance();
                    currentState = OPEN_CHARACTERISTICS;


                }
                break;

           case OPEN_CHARACTERISTICS:



                if (!strcasecmp(buffer, "<characteristics>"))
                {
                    advance();

                    while(!strcasecmp(buffer, "<ch>"))
                    {
                        uint32_t * sn = (uint32_t *)c;

                        if (hdr.fileByteorder == MSF)
                        {
                           *sn = ntohl(*sn);
                        }

                        c += 4;
                        cout << "Characteristics: " << c << endl;
                        c += *sn;

                        advance();
                        advance();
                    }

                    currentState = OPEN_DATA;

                }
                break;

            case OPEN_DATA:
                cout <<  "Lets get the data" << endl;
                if (!strcasecmp(buffer, "<data>"))
                {
                    advance();
                    for (vector<StataVariables *>::iterator it = vList.begin(); it != vList.end(); ++it)
                    {
                        switch((*it)->type)
                        {
                            case ST_STRL:
                                    cout << "STRL" << endl;
                                    break;
                            case ST_DOUBLE:
                                    cout << "DOUBLE" << endl;
                                    break;
                            case ST_FLOAT:
                                    cout << "FLOAT" << endl;
                                    break;
                            case ST_LONG:
                                    cout << "LONG" << endl;
                                    break;
                            case ST_INT:
                                    cout << "INTEGER" << endl;
                                    break;
                            case ST_BYTE:
                                    cout << "BYTE" << endl;
                                    break;
                            default:
                                    if ((*it)->type > 0 && (*it)->type <= 2045)
                                        cout << "STRING OF LENGTH " << (*it)->type << endl;
                                    else
                                        cout << "UNKNOWN TYPE" << endl;
                                    break;

                        }


                    }
                }
                else
                {
                    cout << c << endl;
                }
                break;


            default:
                exit(1);
                break;

	}
    }
	
}


                                        
