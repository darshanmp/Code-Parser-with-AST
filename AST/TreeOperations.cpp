/////////////////////////////////////////////////////////////////////
//  TreeOperations.cpp - declares new parsing rules and actions      //
//  ver 3.0                                                        //
//  Language:      C++, Visual Studio 2015                         //
//  Platform:      Lenovo Y50								        //
//  Application:   TreeOperations				                    //
// Author:      Darshan Masti Prakash, Syracuse University,         //
//				dmastipr@syr.edu								    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187         //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module defines ops to get AST and display AST

Public Interface:
=================
AST* clAst;
Node* elem;
elem->type = "namespace";
elem->name = "global namespace";
elem->startLineCount = 0;
elem->endLineCount = 0;
clAst->AddAST(elem);
elem.show();

Build Process:
==============
Required files
- TreeOperations.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv Project2.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.0 : 17 Mar 16
- first release

*/
//
#include "TreeOperations.h"

//show the element
std::string Node::show()
{	
	std::ostringstream temp;
	temp << "(";
	temp << gettype();
	temp << ", ";
	temp << getname();
	temp << ", ";
	temp << getstartlinecount();
	temp << ", ";
	temp << getendlinecount();
	temp << ", ";
	temp << getcomplexity();
	temp << ")";
	temp << "\n";	
	return temp.str();
}

//display the AST by traversing the elements
Node* AST::DisplayAST(Node* root)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << root->show();
	auto iter = root->getchildren().begin();
	++indentLevel;
	while (iter != root->getchildren().end())
	{
		DisplayAST(*iter);
		++iter;
	}
	--indentLevel;
	return root;
}

//get the root element 
Node* AST::GetRoot()
{
	return root;
}
//add an element to the AST
bool AST::AddAST(Node* elem)
{
	root->addChild(elem);
	return true;
}


#ifdef TEST_AST
int main()
{
	AST* clAst;
	Node* elem;
	elem->type = "namespace";
	elem->name = "global namespace";
	elem->startLineCount = 0;
	elem->endLineCount = 0;
	clAst->AddAST(elem);
	elem.show();
}
#endif // TEST_AST
