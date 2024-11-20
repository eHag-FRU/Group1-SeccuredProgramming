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
		cout << "========TEST 1========" << endl << endl;
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

		assert(validTimeStamp(sanitizedResult, true));	
		cout << "valid time stamp passed on empty file" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 2========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", "0"));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		assert(!validTimeStamp(sanitizedResult, true));	
		cout << "invalid time stamp of 0 passed" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 3========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", "1"));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		assert(validTimeStamp(sanitizedResult, true));	
		cout << "Valid time stamp of 1 passed with preexisting entries" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 4========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test9.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", "1"));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test9.txt"));

		assert(!validTimeStamp(sanitizedResult, true));	
		cout << "Invalid time stamp of 1 when current time stamp is 5 passed" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 5========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", "%x"));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		assert(!validTimeStamp(sanitizedResult, true));	
		cout << "Invalid time stamp of %x when current time stamp is 1 passed" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 6========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", ""));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		assert(!validTimeStamp(sanitizedResult, true));	
		cout << "Invalid time stamp of  when current time stamp is 1 passed" << endl << endl;
	}

	{
		//setup
		cout << "========TEST 7========" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanitizedResult;
		sanitizedResult.insert(std::make_pair("programName", "logappend"));
		sanitizedResult.insert(std::make_pair("-T", ""));
		sanitizedResult.insert(std::make_pair("-K", "secret"));
		sanitizedResult.insert(std::make_pair("-G", "Jeff"));
		sanitizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		assert(!validTimeStamp(sanitizedResult, true));	
		cout << "Invalid time stamp of  when current time stamp is 1 passed" << endl << endl;
	}

    cout << endl << "====PASSED VALID TIME STAMP TESTS====" << endl << endl;

    return 0;
}



