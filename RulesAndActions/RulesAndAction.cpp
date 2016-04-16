/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 3.0                                                        //
//  Language:      C++ Visual Studio 2015                          //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:		   Darshan Masti Prakash						   //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module the rules and actions packages that consists of rules and actions which are defined
for the various scopes in C++

Public Interface:
=================
std::queue<std::string> resultsQ;
PreprocToQ ppq(resultsQ);
PreprocStatement pps;
pps.addAction(&ppq);
FunctionDefinition fnd;
PrettyPrintToQ pprtQ(resultsQ);
fnd.addAction(&pprtQ);
Toker toker("../RulesAndAction.h");
SemiExp se(&toker);
Parser parser(&se);
parser.addRule(&pps);
parser.addRule(&fnd);
while (se.get())
parser.parse();
size_t len = resultsQ.size();
for (size_t i = 0; i<len; ++i)
{
std::cout << "\n  " << resultsQ.front().c_str();
resultsQ.pop();
}
std::cout << "\n\n";

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
ver 3.0 : Made changes to handle the various rules and actions which are used to handling
the various scopes
ver 2.1 : 15 Feb 16
- small functional change to a few of the actions changes display strategy
- preface the (new) Toker and SemiExp with Scanner namespace
ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/
//

#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "RulesAndAction.h"
#include "Tokenizer.h"
#include "SemiExpression.h"

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ActionsAndRules class\n "
		<< std::string(30, '=') << std::endl;

	try
	{
		std::queue<std::string> resultsQ;
		PreprocToQ ppq(resultsQ);
		PreprocStatement pps;
		pps.addAction(&ppq);

		FunctionDefinition fnd;
		PrettyPrintToQ pprtQ(resultsQ);
		fnd.addAction(&pprtQ);

		Toker toker("../RulesAndAction.h");
		SemiExp se(&toker);
		Parser parser(&se);
		parser.addRule(&pps);
		parser.addRule(&fnd);
		while (se.get())
			parser.parse();
		size_t len = resultsQ.size();
		for (size_t i = 0; i<len; ++i)
		{
			std::cout << "\n  " << resultsQ.front().c_str();
			resultsQ.pop();
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
}
#endif
