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
#include <list>
#include <boost/asio.hpp>
#include "StataHeader.h"
#include "StataVariables.h"
#include "StataRead.h"
#include "State.h"

using namespace boost;
using namespace std;
using std::cout;
using boost::asio::ip::tcp;

unsigned short ops;

// Options
//
// Read file - check metadata
// Read file - export to database
// Read database - export to file

#define PROGRAM_NAME "stata_readwrite"
#define PROGRAM_VERSION 0.1

int main(int argc, char **argv)
{
  namespace po = boost::program_options;
  Context *ctx = NULL;

  po::options_description desc("Allowed options");
  desc.add_options()
  ("help", "produce help message")
  ("stata-file", po::value< string >(), "stata file")
  ("metadata,m", "show metadata")
  ;

  po::positional_options_description p;
  p.add("stata-file", -1);
  

  po::variables_map args;
  po::store (po::command_line_parser(argc, argv).options(desc).positional(p)
  .style (po::command_line_style::default_style | po::command_line_style::allow_long_disguise)
  .run (), args);
  po::notify(args);

  if (args.count("help") || !args.count("stata-file"))
  {
    cout << "Usage: " << PROGRAM_NAME << " [options] <stata file>\n";
    cout << desc;
    return 0;
  }

  ifstream stfs(args["stata-file"].as< string >().c_str(), ios::in | ios::binary);
  std::string stata((std::istreambuf_iterator<char>(stfs)), std::istreambuf_iterator<char>());
  ctx = new Context((char *)stata.c_str(), stata.length());

  if (ctx != NULL) {
    ctx->advance();
  }

  // Show the metadata
  if (args.count("metadata"))
  {
    cout << ctx->hdr.showHeader() << endl;
  }

  

  
  

  //  ctx->exportToDB((char *)"export_context.db");
  

  
}
