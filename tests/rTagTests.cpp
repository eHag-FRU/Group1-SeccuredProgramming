#include <iostream>
#include <string>
#include <map>
#include "../include/rTag.hpp"
#include <cassert>

using std::cout; using std::endl; using std::string; using std::map;

int main(int argc, char* argv[]) {

	// checking what happens with an empty log file
	{
		cout << "========Test 1========" << endl << endl;

		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "Jeff"});
		sanatizedResult.insert({"logFile", "./tests/rTagTxtFiles/test1.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);

		cout << "after rtag invokaiton" << endl;

		assert(roomNumbers[0] == "-1");
		cout << "Passed: empty log file" << endl << endl;
	}

	// tests the case when a person is not in the gallery
	{
		cout << "========Test 2========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "Jeff"});
		sanatizedResult.insert({"logFile", "./tests/rTagTxtFiles/test2.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "-1");
		cout << "Passed: name not found in logFile" << endl << endl;
	}

	// tests the case when a person is in the gallery but has not entered a room
	{
		cout << "========Test 3========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "George"});
		sanatizedResult.insert({"logFile", "./tests/rTagTxtFiles/test2.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "-1");
		cout << "Passed: name never entered a room" << endl << endl;
	}

	// tests the case when a person is in the gallery but has not entered a room
	{
		cout << "========Test 4========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "George"});
		sanatizedResult.insert({"logFile", "./tests/rTagTxtFiles/test2.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "-1");
		cout << "Passed: name never entered a room" << endl << endl;
	}

	// tests the case when a person is in the gallery and has entered a room
	{
		cout << "========Test 5========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "Lucy"});
		sanatizedResult.insert({"logFile", "./tests/rTagTxtFiles/test2.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "2");
		cout << "Passed: name has entered a room" << endl << endl;
	}

	// tests the case when a person is in the gallery and has entered multiple rooms
	{
		cout << "========Test 6========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-G", "Lucy"});
		sanatizedResult.insert({"logFile", "../tests/rTagTxtFiles/test3.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "2");
		assert(roomNumbers[1] == "3");
		cout << "Passed: name has entered 2 rooms" << endl << endl;
	}

	// tests the case when a guest has the same name as an employee
	{
		cout << "========Test 7========" << endl << endl;

		// setup
		map<string, string> sanatizedResult;
		sanatizedResult.insert({"programName", "logappend"});
		sanatizedResult.insert({"-K", "secret"});
		sanatizedResult.insert({"-R", "R"});
		sanatizedResult.insert({"-E", "Lucy"});
		sanatizedResult.insert({"logFile", "../tests/rTagTxtFiles/test5.txt"});

		

		// invokation
		vector<string> roomNumbers = rTagFunctionality(sanatizedResult, true);
		

		// assert
		assert(roomNumbers[0] == "1");
		assert(roomNumbers[1] == "7");
		assert(roomNumbers[2] == "8");
		cout << "Passed: name has entered multiple rooms" << endl << endl;
	}
	

}