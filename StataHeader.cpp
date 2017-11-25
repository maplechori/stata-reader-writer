/* 
 * File:   StataHeader.cpp
 * Author: adrianmo
 * 
 * Created on May 19, 2014, 4:41 PM
 */

#include "StataHeader.h"

StataHeader::StataHeader() 
{
    variables = 0;
    observations = 0;
    
}

StataHeader::StataHeader(const StataHeader& orig) 
{

}

StataHeader::~StataHeader() 
{
    
}

string StataHeader::showHeader() {
   std::ostringstream metadata;
   string textRelease;

   switch(fileRelease)
   {
       case R119: 
            textRelease = "119";
            break;
       case R118: 
            textRelease = "118";
            break;
       case R117: 
            textRelease = "117";
            break;
       case R115: 
            textRelease = "115";
            break;
       case R114: 
            textRelease = "114";
            break;
       case R113: 
            textRelease = "113";
            break;
       case R112:
            textRelease = "112";
            break;

   }

   
   metadata << "[Metadata]" << endl
            << "Variables: " << variables << endl
            << "Observations: " << observations << endl
            << "Time Stamp: " << ts << endl
            << "Data Label: " << datalabel << endl
            << "Release: " << textRelease << endl
            << "Bitness: " << (fileByteorder ? "Big Endian" : "Little Endian" ) << endl;


   return string(metadata.str());
}

