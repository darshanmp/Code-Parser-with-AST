/////////////////////////////////////////////////////////////////////
//  Analysis.cpp - metric analysis is carried out in this		       //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Darshan MP, Syracuse University				   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines tge metric analysis used to display functions

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
#include "Analysis.h"
#include <iomanip> 
#include"../AST/TreeOperations.h"
//display functions metrics
void Results::DisplayFunctionMetrics()
{
	std::cout << "\n";
	std::cout.width(80);std::cout << "Function complexity and size analysis\n";
	std::cout.width(80); std::cout << "------------------------------------\n";
	int n = 6;
	std::cout.width(20); std::cout << "Function";
	std::cout.width(20); std::cout << "Size";
	std::cout.width(20); std::cout << "Complexity";
	std::cout.width(20); std::cout << "FileName" << "\n";	
	std::vector<Node*> res = getresults();
	std::vector <std::string> files = getNames();
	for (int i = 0; i < res.size(); i++)
	{		
		DisplayFuncComp(res[i],files[i]);
	}
	res.clear();
}
//return names of files
std::vector<std::string> Results::getNames()
{
	return fileNames;
}
//set the file names
void Results::setName(std::string filename)
{
	fileNames.push_back(filename);
}
//display the function complexity
void Results::DisplayFuncComp(Node* root,std::string file)
{
	//operator or function handling
	if (root->gettype() == "function" || root->gettype() == "operator")
	{
		size_t temp = root->getendlinecount() - root->getstartlinecount();
		size_t exp = root->getendlinecount() - root->getstartlinecount() - 1;
		if (temp == 0 || temp ==1)
			exp = 1;
		else
			exp = temp-1;
		DisplayFunc(root->getname(), exp, root->getcomplexity(),file);
	}
	auto iter = root->getchildren().begin();
	while (iter != root->getchildren().end())
	{
		//recursive call
		DisplayFuncComp(*iter,file);
		++iter;
	}
}
//display the function results
void Results::DisplayFunc(std::string name, size_t size, size_t complexity,std::string filename)
{
	std::cout.width(20); std::cout << name;
	std::cout.width(20); std::cout << size;
	std::cout.width(20); std::cout << complexity;
	std::cout.width(20); std::cout << getPath(filename) << "\n";
}
//set the results vector
void Results::setresults(Node*  elem, std::string fileName)
{
	fileNames.push_back(fileName);
	results.push_back(elem);
}

//get the path from the filename
std::string Results::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(pos+1, fileSpec.length());
	return fileSpec;
}
std::vector<Node*> Results::getresults()
{
	return results;
}
#ifdef TEST_ANALYSIS

int main(int argc, char* argv[])
{
	Results *res = new Results();
	Node* root = new Node("fnction", "ADS", 34, 40);
	res->setresults(root, "F:\Courses\687\Project2\TestFolder\Folder1\TestInputCPP.txt"); //add the tree results into metrics analysis	
	std::vector<Node*> vectres = res->getresults();	
	for (int i = 0; i < vectres.size(); i++)
	{
		std::cout << vectres[i] << std::endl;
	}
	vectres.clear();
}
#endif // ANALYSIS

