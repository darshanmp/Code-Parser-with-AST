#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
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
#include <queue>
#include <string>
#include <sstream>
#include "../Parser/Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include"../AST/TreeOperations.h"
#include<map>
///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
class Repository  // application specific
{
	ScopeStack<Node*> stack;
	Scanner::Toker* p_Toker;
	AST* ast;	

public:
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
		ast = new AST();
	}
	ScopeStack<Node*>& scopeStack()
	{
		return stack;
	}
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	size_t startLineCount()//Data container for Start line 
	{
		return (size_t)(p_Toker->currentLineCount() - 1);
	}
	size_t endLineCount()//Data container for End line
	{
		return (size_t)(p_Toker->currentLineCount() - 1);
	}
	bool AddASTNode(Node* elem)//Function for AddAST Node
	{
		ast->GetRoot()->addChild(elem);
		return true;
	}
	Node* GetASTRoot()//return the root elememt
	{
		return ast->GetRoot();
	}
	Node* DisplayAST(Node* root) //display the AST node
	{				
		return ast->DisplayAST(root);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope
class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//	if (pTc->find("{") < pTc->length())
		pTc->trimFront();
		if (pTc->find("{") == 0)
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n--BeginningOfScope rule";
		Node* elem = new Node("unknown", "anonymous", p_Repos->startLineCount(), 0);
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (p_Repos->scopeStack().size() == 0)
			return;
		Node* elem = new Node();
		elem = p_Repos->scopeStack().pop();
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->setcomplexity(p_Repos->scopeStack().top()->getcomplexity() + elem->getcomplexity());
		elem->setendlinecount(p_Repos->endLineCount());
	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements
class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console
class PrintPreproc : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
	//	std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
//Rule to detect function loops such as for, while, if, switch,catch, else
class HandleLoop : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if","catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

//action to handle loops 
class HandleLoopAction : public IAction
{
	Repository* p_Repos;
public:
	HandleLoopAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// push the loop onto stack
		std::string name = (*pTc)[pTc->find("(") - 1];
		Node* elem = new Node(name, name, p_Repos->startLineCount(), 0);

		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		//std::cout<<elem.show();
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
//Rule to detect class declaration
class HandleClassStruct : public IRule
{
public:
	bool isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return true;
		}
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if ((tc[tc.length() - 1] == "{") && isKeyWord(tc))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//action to handle loops 
class HandleClassStructAction : public IAction
{
	Repository* p_Repos;
public:
	HandleClassStructAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"class", "struct","enum"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
					return keys[i];
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		//p_Repos->scopeStack().pop();
		// push the loop onto stack
		ITokCollection& tc = *pTc;
		Node* elem = new Node();
		elem->settype(isKeyWord(tc));
		if (elem->gettype() == "class")
			elem->setname((*pTc)[pTc->find("class") + 1]);
		else if(elem->gettype() == "struct")
			elem->setname((*pTc)[pTc->find("struct") + 1]);
		else
		{
			elem->setname((*pTc)[pTc->find("enum") + 1]);
		}
		elem->setstartlinecount(p_Repos->startLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
//Rule to detect else , try , do while
class HandleSpecialKeywords : public IRule
{
public:
	bool isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"else", "try","do"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tok[j] == keys[i])
				{
					if (tok[j + 1] != "if")
						return true;
				}
		}
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if ((tc[tc.length() - 1] == "{") && isKeyWord(tc))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//action to handle special keywords like do while, else, try 
class HandleSpecialKeywordsAction : public IAction
{
	Repository* p_Repos;
public:
	HandleSpecialKeywordsAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	std::string isKeyWord(ITokCollection& tok)
	{
		const size_t numKeys = 3;
		const static std::string keys[numKeys] = {
			"else", "do","try"
		};
		for (int j = 0; j < tok.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
			{
				if (tok[j] == keys[i])
					return keys[i] == "do" ? "do while" : keys[i];
			}
		}
		return "default";
	}
	void doAction(ITokCollection*& pTc)
	{
		// push the loop onto stack
		ITokCollection& tc = *pTc;
		Node* elem = new Node(isKeyWord(tc), isKeyWord(tc), p_Repos->startLineCount(), 0);
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
//Rule to detect lambdas
class LambdaDetect : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		const size_t numKeys = 6;
		const static std::string keys[numKeys] = {
			"[", "]","(",")", "{","Operator"
		};
		if ((tc.find("main")) < pTc->length())
		{
			return true;
		}
		std::map<std::string, int> operMap;
		for (int j = 0; j < tc.length(); j++)
		{
			for (int i = 0; i < numKeys; ++i)
				if (tc[j] == keys[i])
				{
					operMap[keys[i]] = j;
				}
		}
		int size_1 = operMap["]"] - operMap["["];
		int size_2 = operMap[")"] - operMap["("];
		int size_3 = operMap["{"];
		int size_4 = operMap["Operator"];
		if ((size_1 >= 1) && (size_2 >= 1) && (size_3 >= 1) && (size_4 < 1))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//action to handle special keywords like do while, else, try 
class LambdaDetectAction : public IAction
{
	Repository* p_Repos;
public:
	LambdaDetectAction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		//p_Repos->scopeStack().pop();		
		ITokCollection& tc = *pTc;
		Node* elem = new Node();
		std::string name = (*pTc)[pTc->find("=") - 1] = NULL ? "unnamed" : (*pTc)[pTc->find("=") - 1];
		elem->settype("Lambda");
		elem->setname(name);
		elem->setstartlinecount(p_Repos->startLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions
class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		//check if [ is there and operator keyword is not there for ignoring lambdas
		if ((tc.find("Operator") >= pTc->length()) && (tc.find("[") < pTc->length()) && (tc.find("main") >= pTc->length()))
		{
			return true;
		}
		if ((tc.find("namespace") < pTc->length()) && (tc.find("using") >= pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		if ((tc.find("operator") < pTc->length()) && (tc.find("{") < pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack
class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// std::cout << "\n  FunctionDef: " << pTc->show();
		// pop anonymous scope
		//	p_Repos->scopeStack().pop();
		// push function scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		Node *elem;
		if (pTc->find("namespace") < pTc->length())
		{	
			pTc->trim();
			elem = new Node("namespace", (*pTc)[pTc->find("{") - 1], p_Repos->startLineCount(), p_Repos->endLineCount());
		}
		else if (pTc->find("operator") < pTc->length())
			elem = new Node("operator", "operator"+name, p_Repos->startLineCount(), p_Repos->endLineCount());
		else
			elem = new Node("function", name, p_Repos->startLineCount(), p_Repos->endLineCount());
		if (p_Repos->scopeStack().size() > 0)
			p_Repos->scopeStack().top()->addChild(elem);
		else
			p_Repos->AddASTNode(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect declaration
class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);
		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			//std::cout << "\n  ++ " << tc.show();
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);
			// remove modifiers, comments, newlines, returns, and initializers
			Scanner::SemiExp se;
			return doTest1(tc, se, pTc);

		}
		return true;
	}

	bool doTest1(Scanner::SemiExp &tc, Scanner::SemiExp &se, ITokCollection*& pTc)
	{
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (isModifier(tc[i]))
				continue;
			if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
				continue;
			if (tc[i] == "=" || tc[i] == ";")
				break;
			else
				se.push_back(tc[i]);
		}
		//std::cout << "\n  ** " << se.show();
		if (se.length() == 2)  // type & name, so declaration
		{
			doActions(pTc);
			return true;
		}
	}

};

///////////////////////////////////////////////////////////////
// action to show declaration
class ShowDeclaration : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i<tc.length(); ++i)
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		// show cleaned semiExp
		//	std::cout << "\n  Declaration: " << se.show();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect executable
class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
	bool isModifier(const std::string& tok) // modifiers and initializers.
	{                                       // If you have two things left
		const size_t numKeys = 22;            // its declar else executable.
		const static std::string keys[numKeys] = {
			"const", "extern", "friend", "mutable", "signed", "static",
			"typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
			"public", "protected", "private", ":", "typename", "typedef", "++", "--"
		};
		for (int i = 0; i<numKeys; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void condenseTemplateTypes(ITokCollection& tc)
	{
		size_t start = tc.find("<");
		size_t end = tc.find(">");
		if (start >= end || start == 0)
			return;
		else
		{
			if (end == tc.length())
				end = tc.find(">::");
			if (end == tc.length())
				return;
		}
		std::string save = tc[end];
		std::string tok = tc[start - 1];
		for (size_t i = start; i < end + 1; ++i)
			tok += tc[i];
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		if (save == ">::")
		{
			tok += tc[start + 1];
			tc.remove(start);
		}
		tc[start - 1] = tok;
		//std::cout << "\n  -- " << tc.show();
	}

	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	void removeInvocationParens(ITokCollection& tc)
	{
		size_t start = tc.find("(");
		size_t end = tc.find(")");
		if (start >= end || end == tc.length() || start == 0)
			return;
		if (isSpecialKeyWord(tc[start - 1]))
			return;
		for (size_t i = start; i < end + 1; ++i)
			tc.remove(start);
		//std::cout << "\n  -- " << tc.show();
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& in = *pTc;
		Scanner::SemiExp tc;
		for (size_t i = 0; i<in.length(); ++i)
			tc.push_back(in[i]);
		if (tc[tc.length() - 1] == ";" && tc.length() > 2)
		{
			//std::cout << "\n  ++ " << tc.show();
			removeInvocationParens(tc);
			condenseTemplateTypes(tc);
			// remove modifiers, comments, newlines, returns, and initializers
			Scanner::SemiExp se;
			doTest2(tc, se);

			//std::cout << "\n  ** " << se.show();
			if (se.length() != 2)  // not a declaration
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}

	void doTest2(Scanner::SemiExp &tc, Scanner::SemiExp &se)
	{
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (isModifier(tc[i]))
				continue;
			if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
				continue;
			if (tc[i] == "=" || tc[i] == ";")
				break;
			else
				se.push_back(tc[i]);
		}
	}
};

///////////////////////////////////////////////////////////////
// action to show executable
class ShowExecutable : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		// remove comments
		Scanner::SemiExp se;
		for (size_t i = 0; i < tc.length(); ++i)
		{
			if (!se.isComment(tc[i]))
				se.push_back(tc[i]);
		}
		// show cleaned semiExp
		//	std::cout << "\n  Executable: " << se.show();
	}
};

#endif
