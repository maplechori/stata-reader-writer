/* 
 * File:   main.cpp
 * Author: adrianmo
 *
 * Created on May 16, 2014, 3:51 PM
 */

#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <boost/regex.hpp>
#include <boost/variant.hpp>
#include <boost/program_options.hpp> 
#include <list>
#include <boost/asio.hpp>
#include "StataHeader.h"
#include "StataVariables.h"

using namespace boost;
using namespace std;
using boost::asio::ip::tcp;


/*
 * 
 */

char *c;
char *start;
char buffer[4096];
unsigned short ops;

enum state {
    OPEN_HEADER,
    OPEN_RELEASE,
    OPEN_BYTEORDER,
    OPEN_K,
    OPEN_N,
    OPEN_LBL,
    OPEN_TS,
    OPEN_MAP,
    OPEN_VARTYPES,
    OPEN_VARNAMES,
    OPEN_SORTLIST,
    OPEN_FORMATS,
    OPEN_VALLBL,
    OPEN_VARLBL,
    OPEN_CHARACTERISTICS,
    OPEN_DATA



};

void advance() {
    for (start = c; c && *c != '>'; c++);

    if (c && *c == '>') {
        c++;
        strncpy(buffer, start, c - start);
        buffer[c - start] = '\0';
    }
}

int main(int argc, char** argv) {

    namespace po = boost::program_options;

    ifstream stfs("/home/adrianmo/lasi.dta", ios::in | ios::binary);
    std::string stata((std::istreambuf_iterator<char>(stfs)), std::istreambuf_iterator<char>());
    state currentState = OPEN_HEADER;
    vector<StataVariables *> vList;
    StataHeader hdr;

    c = (char *) stata.c_str();


    if (!*c || *c != '<') {
        cout << "Unsupported file format" << endl;
        return 0;
    }


    advance();

    while (1) {

        advance();

        switch (currentState) {

            case OPEN_HEADER:
                if (!strcasecmp(buffer, "<header>")) {
                    cout << "Good, found STATA header" << endl;
                    currentState = OPEN_RELEASE;
                }
                break;

            case OPEN_RELEASE:


                if (!strcasecmp(buffer, "<release>")) {
                    strncpy(buffer, c, 3);
                    buffer[3] = 0;
                    cout << "Release: " << buffer << endl;

                    hdr.fileRelease = R117;
                    currentState = OPEN_BYTEORDER;
                    advance();
                }
                break;


            case OPEN_BYTEORDER:
                if (!strcasecmp(buffer, "<byteorder>")) {
                    strncpy(buffer, c, 3);
                    buffer[3] = 0;
                    cout << "ByteOrder: " << buffer << endl;

                    if (!strcasecmp(buffer, "LSF"))
                        hdr.fileByteorder = LSF;
                    else
                        hdr.fileByteorder = MSF;

                    currentState = OPEN_K;
                    advance();

                }

                break;

            case OPEN_K:

                if (!strcasecmp(buffer, "<K>")) {
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
                if (!strcasecmp(buffer, "<N>")) {
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
                if (!strcasecmp(buffer, "<label>")) {
                    if (hdr.fileByteorder == LSF) {


                    } else {
                        // not implemented yet
                    }
                    currentState = OPEN_TS;
                    advance();

                }

                break;


            case OPEN_TS:
                if (!strcasecmp(buffer, "<timestamp>")) {
                    char *t = c;
                    int i = 0;

                    unsigned char x = *(unsigned char*) c++;

                    while (*c != '<') {
                        c++;
                        i++;
                    }


                    strncpy(buffer, t + 1, i);
                    buffer[i] = 0;
                    cout << "Size: " << static_cast<int> (x) << " Timestamp: " << buffer << endl;

                    hdr.ts = new char[i + 1];
                    strncpy(hdr.ts, buffer, i);
                    hdr.ts[i] = 0;
                    currentState = OPEN_MAP;
                    advance();

                }
                break;

            case OPEN_MAP:
                if (!strcasecmp(buffer, "<map>")) {
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
                    c += (hdr.variables + 1) * 2;
                    advance();
                    currentState = OPEN_FORMATS;

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
                                vList.at(curr)->vallbl = buf;                               
                            
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
                                cout << vList.at(curr)->varname << " " << vList.at(curr)->varlbl << endl;
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
                        cout << c << endl;
                        c += *sn;
                                            
                        advance();                                             
                        advance();                   
                    }
                        
                   
                    currentState = OPEN_DATA;
                    
                    
                    
                           
                    
                    
                }
                break;
                
                
                
            case OPEN_DATA:
                
              
                
                
                
                
                
                break;
                
                
                
                
                

            default:
                exit(1);
                break;



        }


    }













}

