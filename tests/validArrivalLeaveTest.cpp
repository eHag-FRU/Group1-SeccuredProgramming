#include <iostream>
#include <string>
#include <map>
#include "../include/logappend.hpp"
#include "inputSanatizer.hpp"
#include <cassert>

using std::cout; using std::endl; using std::string; using std::map;

int main(int argc, char* argv[]) {
	cout << "==== TESTING VALID ARRIVAL & LEAVE ====" << endl;
	

	// with false name
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
	sanatizedResult.insert(std::make_pair("programName", "logappend"));
	sanatizedResult.insert(std::make_pair("-T", "1"));
	sanatizedResult.insert(std::make_pair("-K", "secret"));
	sanatizedResult.insert(std::make_pair("-G", "Jeff"));
	sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

	assert(!validArrivalLeave(sanatizedResult, "Fred", false));	
	cout << "false name passed" << endl << endl;
	
	

	//with correct name and no previous action (just entered the gallery)
	{
	//setup
	cout << "TEST 2" << endl << endl;
	std::fstream logFile;
	logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

	if (!logFile) {
        std::cerr << "File could not be opened for writing!" << std::endl;
        return 1;
    }

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		resultMapToString(sanatizedResult);
		assert(validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "correct name with initially entering gallery passed" << endl << endl;
	}

	// with correct name and the previous action being entering the gallery initially
	{
		// setup
		cout << "TEST 3" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl << endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		// test
		resultMapToString(sanatizedResult);

		//assert
		assert(validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "correct name and entering room passed" << endl << endl;
	}

	//the previous action being entering the gallery initially and then leaving without entering a room
	{
		// setup
		cout << "TEST 4" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test1.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test1.txt"));

		// test
		resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "successfully prevented leaving without entering a room" << endl << endl;
	}


	// testing to see if the person can go to a room without leaving the previous room
	{
		// setup
		cout << "TEST 5" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "2"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "correctly prevented to go to room without leaving" << endl;
	}

	// testing to see if the person can go to a room without entering the gallery
	{
		// setup
		cout << "TEST 6" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "correctly prevented to go to room without first entering the gallery" << endl << endl;
	}

	// testing to see if the person can leave a room correctly
	{
		// setup
		cout << "TEST 7" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "person can successfully leave a room" << endl;
	}

	// testing to see if the person enter a room without first entering the gallery
	{
		// setup
		cout << "TEST 8" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test3.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test3.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person cant enter a room without first entering the gallery" << endl;
	}

	// testing to see that the proper action is assigned to the name
	// jeff trying to arrive to a room he has not left, while lucy has left that room
	{
		// setup
		cout << "TEST 9" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test5.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-A", "A"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "2"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test5.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person cant enter a room without leaving a room" << endl;
	}

	// testing to see that the proper action is assigned to the name
	// jeff trying to leave to a room he has not arrived in, while lucy has arrived to that room
	{
		// setup
		cout << "TEST 10" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test6.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "1"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test6.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person cant leave a room without entering a room" << endl;
	}

	// testing to see that a person cannot leave a room they have not last entered
	{
		// setup
		cout << "TEST 11" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "3"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person cant leave a room they are not currently in" << endl;
	}

	// testing to see that a person cant leave a room they have not last entered with multiple room entries
	{
		// setup
		cout << "TEST 12" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test2.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "2"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test2.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(!validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person cant leave a room they are not currently in" << endl;
	}

	// testing to see that a person can leave a room they are currently in with multiple room entries
	{
		// setup
		cout << "TEST 13" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test7.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "3"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test7.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person can leave a room they are currently in" << endl;
	}

	// testing to see that a person can leave a room they are currently in with multiple room entries with another name in the mix
	{
		// setup
		cout << "TEST 14" << endl << endl;
		std::fstream logFile;
		logFile.open("./tests/testTxtFiles/test8.txt", std::ios::in);

		if (!logFile) {
			std::cerr << "File could not be opened for writing!" << std::endl;
			return 1;
		}

		map<string, string> sanatizedResult;
		sanatizedResult.insert(std::make_pair("programName", "logappend"));
		sanatizedResult.insert(std::make_pair("-T", "1"));
		sanatizedResult.insert(std::make_pair("-K", "secret"));
		sanatizedResult.insert(std::make_pair("-L", "L"));
		sanatizedResult.insert(std::make_pair("-G", "Jeff"));
		sanatizedResult.insert(std::make_pair("-R", "3"));
		sanatizedResult.insert(std::make_pair("logFile", "./tests/testTxtFiles/test8.txt"));

		// test
		//resultMapToString(sanatizedResult);

		//assert
		assert(validArrivalLeave(sanatizedResult, "Jeff", true));
		cout << "passed: person can leave a room they are currently in" << endl;
	}


	cout << endl << "====PASSED VALID ARRIVAL & LEAVE TESTS====" << endl << endl;
}
}
