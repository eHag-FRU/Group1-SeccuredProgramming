#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>
#include <fstream>
#include <utility>
#include <sstream>
#include "../include/inputSanatizer.hpp"
#include "./encryptionHandler.hpp"

using std::cout; using std::endl; using std::map; using std::string; using std::regex; 
using std::regex_match; using std::iterator; using std::ifstream; using std::pair; using std::vector;


vector<string> splitString(const string& str, char delimiter);
vector<string> rTagFunctionality(map<string,string>& cmdLine, bool debugMode);


// Function to split a string by a delimiter and return a vector of substrings
vector<string>   splitString(const string& str, char delimiter) {
    vector<string> tokens;
    std::istringstream stream(str);
    string token;
    
    while (getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

vector<string> rTagFunctionality(map<string,string>& cmdLine, bool debugMode) {
	//declare tags that could contain the persons name
	string E = "-E";
	string G = "-G";
	string name;
	bool isEmployee;

	if (cmdLine.find(G) != cmdLine.end()) {
		//Guest name
		isEmployee = false;
		name = cmdLine[G];
	} else {
		//Employee name
		isEmployee = true;
		name = cmdLine[E];	
	}

	//Open the file
	ifstream logFile;

	logFile.open(cmdLine["logFile"], std::ios::in | std::ios::binary);

	if (!logFile.is_open()) {
		//Could not open the file
		std::cerr << "File could not be opened for reading in rTagFunctionality!" << std::endl;
	} 

	
    // Ensure the file pointer is at the beginning
    logFile.clear(); // Clear any error flags
    logFile.seekg(0, std::ios::beg); // Move to the beginning of the file

	//Curent line, just for getline to throw it into
	string currentLine;

	// vector that contains the room numbers
	vector<string> roomNumbers;

	// Check if the file is empty
    logFile.seekg(0, std::ios::end);
    if (logFile.tellg() == 0) {
        std::cerr << "The file is empty." << std::endl;
		roomNumbers.push_back("-1");
        return roomNumbers;
    }
	 else {		
		logFile.seekg(0, std::ios::beg); 								// Reset to beginning of the file
																		// if the file is not empty, then we can start reading the file	
		while(getline(logFile, currentLine)) {
			//cout << "currentLine: " << currentLine << endl;

			//Decrypt the current line
			if(!debugMode) {
				currentLine = decrypt(currentLine, cmdLine["-K"], cmdLine);
			}

			// split the line by space
			vector<string> line = splitString(currentLine, ' ');
			// check if a room number is input. if it doesnt the vector will have a size of 4
			if (line.size() > 4) {
				// check if the name is in the line
				auto nameIt = find(line.begin(), line.end(), name);
				if (nameIt != line.end()) {
					// check if the person entered a room 
					auto roomIt = find(line.begin(), line.end(), "A");
					auto personIt = line.begin();
					// if the person is an employee, then check the line for "E", then we can add the room number
					if (isEmployee) {
						auto personIt = find(line.begin(), line.end(), "E");
						if (roomIt != line.end() && personIt != line.end()) {
							// get the room number
							roomNumbers.push_back(*(roomIt + 1));
						}
					} else {
						auto personIt = find(line.begin(), line.end(), "G");
						if (roomIt != line.end() && personIt != line.end()) {
							// get the room number
							roomNumbers.push_back(*(roomIt + 1));
						}
					} 
				}
			}
		}

		// check if the name was never found in the log file
		if (roomNumbers.size() == 0) {
			std::cerr << "Name not found in the log file or never entered a room!" << std::endl;
			roomNumbers.push_back("-1");
		}
		

		// print out the room numbers with proper formatting
		if (roomNumbers.size() == 1) {
			cout << roomNumbers[0] << endl;
			
		} else {
			int i;
			for (i = 0; i < roomNumbers.size() - 1; i++) {
				cout << roomNumbers[i] << ", ";
			}
			cout << roomNumbers[i] << endl;
		}
		

		//Close the file
		logFile.close();
		return roomNumbers;
	}
}
