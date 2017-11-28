/* 
 * File:   main.cpp
 * Author: adrianmo
 *
 * Created on May 16, 2014, 5:51 PM
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <boost/regex.hpp>
#include <boost/variant.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <list>
#include <boost/asio.hpp>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include "StataDB.h"
#include "State.h"

using namespace boost;
using namespace std;
using std::cout;
using boost::asio::ip::tcp;

unsigned short ops;

// Options
//
// Read file - check metadata - done
// Read file - export to database
// Read database - export to file

#define PROGRAM_NAME "stata_readwrite"
#define PROGRAM_VERSION 0.1

int main(int argc, char **argv)
{
  namespace po = boost::program_options;
  Context *ctx = NULL;

  try
  {
    po::options_description desc("General options");
    desc.add_options()
    ("help", "produce help message")
    ("stata-file", po::value< string >(), "stata input file")
    ;

    po::options_description db_export("Database options");
    db_export.add_options()
    ("db-type,d", po::value< string >(), "select database type [sqlite]")
    ("connection-string,c", po::value< string >(), "connection string" )
    ;

    po::positional_options_description p;
    p.add("stata-file", -1);

    po::positional_options_description db;
    db.add("db-type", 1).add("connection-string", 2);
    
    po::options_description all("Allowed options");
    all.add(desc).add(db_export);
    
    po::variables_map args;
    
    po::store (po::command_line_parser(argc, argv).options(all).positional(p).positional(db)
    .style (po::command_line_style::default_style | po::command_line_style::allow_long_disguise)
    .run (), args);

    po::notify(args);

    if (args.count("help") || !args.count("stata-file"))
    {
      cout << "Usage: " << PROGRAM_NAME << " [options] <stata file>\n";
      cout << all << endl;
      return 0;
    }

    ifstream stfs(args["stata-file"].as< string >().c_str(), ios::in | ios::binary);
    std::string stata((std::istreambuf_iterator<char>(stfs)), std::istreambuf_iterator<char>());
    ctx = new Context((char *)stata.c_str(), stata.length());

    if (ctx != NULL) {
      ctx->advance();
    }

    
 
    if (args.count("db-type"))
    {
      DatabaseTypes db_type = SQLITE;

      if (!args.count("connection-string"))
      {
        cout << "Connection string required" << endl;
        return 0;     
      }
     
      if (args["db-type"].as< string >() == "sqlite")
      {
        db_type = SQLITE;
      }
      else
      {
        cout << "Database type not supported" << endl;
        return 0;
      }
    
      cout << "Exporting to " << args["connection-string"].as< string >().c_str() << endl;
      ctx->exportToDB( db_type, (char *)args["connection-string"].as< string >().c_str()); 
      return 1;
       
        
      
    }
    else
    {
        // Show the metadata of the file and exit 
         cout << "Information on Stata file: " << args["stata-file"].as< string >() << endl;
         cout << ctx->hdr.showHeader() << endl;
         return 1;
    }

  }
  catch(std::exception& e) {
      cout << e.what() << "\n";
  }
    

  
  

  //  ctx->exportToDB((char *)"export_context.db");
  

  
}
