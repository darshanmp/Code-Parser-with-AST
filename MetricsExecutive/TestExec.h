#ifndef TESTEXEC_H
#define TESTEXEC_H
/////////////////////////////////////////////////////////////////////
// TestExec.h - test layer					                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Darshan Masti Prakash, Syracuse University,        //
//				dmastipr@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
Test layer to automate the testing of SemiExp and Tokenizer packages
*/
/*
*  Public Interface
*  ----------------
//main entry for the program
// collecting tokens from files, named on the command line
if (argc < 2)
{
std::cout<< "\n Please enter name of file/directory to process on command line\n\n";
return 1;
}
std::string fileSpec = "";
TestExec test;
test.Req1();
fileSpec = FileSystem::Path::getFullFileSpec(argv[1]);
std::string msg = "Processing files from the file/directory : " + fileSpec;
Util::title(msg);
std::vector<std::string> pattColl;
for (int i = 2; i < argc; ++i)
{
pattColl.push_back(argv[i]);
putline();
}
test.ExtractFiles(fileSpec,pattColl);
test.Req2();
std::getchar();
return 0;

* Package Operations:
* -------------------
* This package contains different functionalities to meet the requriements and automate the testing
* so that it can be run in future without any supervision
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp , SemiExp.cpp, SemiExp.h,MetricsAnalysis.h
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 17 Mar 2016
* - first release of new design
*
*/
#include "../AST/TreeOperations.h"
#include<vector>
#include <queue>
#include <string>
#include <sstream>
#endif