#include <iostream>
#include <string>
#include <map>
#include "../include/logread.hpp"
#include <cassert>

using std::cout; using std::endl; using std::string; using std::map;

int main(int argc, char* argv[]) {

	// checking what happens with an empty log file
	{
		cout << "========Test 1========" << endl << endl;

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-R", "R"));
		sanatizedResult.insert(std::make_pair("-E", "Jeff"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/rTagTxtFiles/test1.txt"));

		// invokation
		bool result = sTagFunctionality(sanatizedResult, true);


		assert(result);
		cout << "Passed: empty log file" << endl << endl;
	}

	
	cout << "======ALL sTAG TEST PASSED======" << endl;

}