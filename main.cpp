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

int main(int argc, char** argv) {
    namespace po = boost::program_options;
    Context * ctx = NULL;

    ifstream stfs(argv[1], ios::in | ios::binary);
    std::string stata((std::istreambuf_iterator<char>(stfs)), std::istreambuf_iterator<char>());

    ctx = new Context((char *)stata.c_str(), stata.length());

    if (ctx != NULL)
      ctx->advance();

}

