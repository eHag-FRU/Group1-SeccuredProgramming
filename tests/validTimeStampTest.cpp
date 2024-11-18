#include <iostream>
#include <string>
#include <map>
#include "../include/logappend.hpp"
#include "inputSanatizer.hpp"
#include <cassert>

using std::cout; using std::endl; using std::string; using std::map;


int main() {
    cout << "====TESTING VALID TIME STAMP====" << endl << endl
	
	{
	//setup
	cout << "TEST 1" << endl << endl;
	std::fstream logFile;
	logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

	if (!logFile) {
        std::cerr << "File could not be opened for writing!" << std::endl;
        return 1;
    }

	map<string, string> sanatizedResult;
	sanatizedResult.insert({"programName", "logappend"});
	sanatizedResult.insert({"-T", "1"});
	sanatizedResult.insert({"-K", "secret"});
	sanatizedResult.insert({"-G", "Jeff"});
	sanatizedResult.insert({"logFile", "./tests/testTxtFiles/test1.txt"});

	assert(!validArrivalLeave(sanatizedResult, logFile, "Fred", false));	
	cout << "false name passed" << endl << endl;
	}
	







    cout << endl << "====PASSED VALID TIME STAMP TESTS====" << endl << endl;

    return 0;
}



