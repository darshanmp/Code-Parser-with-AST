/////////////////////////////////////////////////////////////////////
// DataStore.cpp - Store results of file search                    //
//                                                                 //
// Jim Fawcett, Help Session Demo, Spring 2015                     //
/////////////////////////////////////////////////////////////////////

#include "DataStore.h"
#include <iostream>

#ifdef TEST_DATASTORE

int main()
{
	std::cout << "\n  Testing DataStore";

	DataStore ds;
	ds.save("one");
	ds.save("two");
	ds.save("three");
	DataStore::iterator iter = ds.begin();
	std::cout << "\n  " << (*iter).c_str();

	for (auto item : ds)
	{
		std::cout << "\n  " << item.c_str();
	}
	std::cout << "\n\n";
}

#endif
