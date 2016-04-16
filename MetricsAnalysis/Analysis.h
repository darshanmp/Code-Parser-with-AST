#ifndef ANALYSIS_H
#define ANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  Analysis.h - metric analysis is carried out in this		       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Darshan MP, Syracuse University				   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines the metric analysis used to display functions

Public Interface:
=================
Results *res = new Results();
Node* root = new Node("fnction", "ADS", 34, 40);
res->setresults(root, "TestInputCPP.txt"); //add the tree results into metrics analysis
std::vector<Node*> vectres = res->getresults();
for (int i = 0; i < vectres.size(); i++)
{
std::cout << vectres[i] << std::endl;
}
vectres.clear();

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release

*/
//
#include"../AST/TreeOperations.h"
//class used to hold results and display items
class Results
{
private:
	std::vector<Node*> results; //to hold the elements
	std::vector<std::string> fileNames; //filenames data
public:
	//get and set on results
	void setresults(Node*,std::string);
	std::vector<Node*> getresults();	
	void DisplayFunctionMetrics();//display func metrics
	void DisplayFuncComp(Node*,std::string);//display func metrics
	void DisplayFunc(std::string, size_t,size_t,std::string);//display func metrics
	std::vector<std::string> getNames();
	void setName(std::string);
	std::string getPath(const std::string & fileSpec);
};
#endif