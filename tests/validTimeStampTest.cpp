#include <iostream>
#include <string>
#include <map>
#include "../include/logappend.hpp"
#include "inputSanatizer.hpp"
#include <cassert>

using std::cout; using std::endl; using std::string; using std::map;


int main() {
    cout << "====TESTING VALID TIME STAMP====" << endl << endl;
	
	{
	//setup
	cout << "TEST 1" << endl << endl;
	std::fstream logFile;
	logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

	if (!logFile) {
        std::cerr << "File could not be opened for writing!" << std::endl;
        return 1;
    }

	map<string, string> sanitizedResult;
	sanitizedResult.insert(std::make_pair("programName", "logappend"));
	sanitizedResult.insert(std::make_pair("-T", "1"));
	sanitizedResult.insert(std::make_pair("-K", "secret"));
	sanitizedResult.insert(std::make_pair("-G", "Jeff"));
	sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

	assert(!validArrivalLeave(sanitizedResult, "Fred", false));	
	cout << "false name passed" << endl << endl;
	







    cout << endl << "====PASSED VALID TIME STAMP TESTS====" << endl << endl;

    return 0;
	}
}



