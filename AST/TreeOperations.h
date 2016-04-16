#ifndef TREEOPERATIONS_H
#define TREEOPERATIONS_H
/////////////////////////////////////////////////////////////////////
//  TreeOperations.h - declares new parsing rules and actions      //
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
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <memory>
#include <iostream>
#include <sstream>

//Class to hold the Node item for an AST
class Node
{
public:
	Node()
	{
			
	}
	//Paramterized constructor
	Node(const std::string type, const std::string name , size_t startLineCount, size_t endLineCount)
	{
		this->name = name;
		this->type = type;
		this->startLineCount = startLineCount;
		this->endLineCount = endLineCount;
	}
	//get and set properties for children
	std::vector<Node*>& getchildren() { return children_; }
	void addChild(Node* pNode) { children_.push_back(pNode); }
	//get and set properties for name
	void setname(std::string name) { this->name = name; };
	std::string getname() { return name; }
	//get and set properties for type
	void settype(std::string type) { this->type = type; };
	std::string gettype() { return type; }
	//get and set properties for startlinecount
	void setstartlinecount(size_t startCount) { this->startLineCount =startCount; }
	size_t getstartlinecount() { return startLineCount; }
	//get and set properties for endlinecount
	void setendlinecount(size_t endCount) { this->endLineCount = endCount; }
	size_t getendlinecount() { return endLineCount; }
	//get and set properties for complexity
	void setcomplexity(size_t comp) { this->complexityCount = comp; }
	size_t getcomplexity() { return complexityCount; }
	std::string show();
private:
	std::string type; //type
	std::string name; //name
	size_t startLineCount; //start line
	size_t endLineCount; //end line
	size_t complexityCount = 1; //complexity
	std::vector<Node*> children_; //children
};

//class to hold AST
class AST
{
private:
	Node* root;	//root element of the tree
public:
	AST()
	{
		root = new Node("namespace", "global namespace",1,1);		
	}
	Node* GetRoot();				//Get the root element
	bool AddAST(Node* elem);		//add node to the root
	/*void DisplayFunctComp(Node * root);*/
	Node* DisplayAST(Node* root);    //display tree ops by walking through it
};
#endif