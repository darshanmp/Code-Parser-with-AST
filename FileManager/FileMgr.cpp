/////////////////////////////////////////////////////////////////////
// FileMgr.cpp - find files matching specified patterns            //
//               on a specified path                               //
// ver 2.0                                                         //
// Jim Fawcett, Help Session Demo, Spring 2015                     //
/////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include "DataStore.h"
#include <iostream>

#ifdef TEST_FILEMGR

int main()
{
	std::cout << "\n  Testing FileMgr";
	std::cout << "\n =================";
	DataStore ds;
	FileMgr fm("..", ds);
	fm.addPattern("*.h");
	fm.addPattern("*.c*");
	//fm.addPattern("*.partial");
	fm.search();
	std::cout << "\n\n  contents of DataStore";
	std::cout << "\n -----------------------";
	for (auto fs : ds)
	{
		std::cout << "\n  " << fs;
	}
	std::cout << "\n\n";
	return 0;
}
#endif
