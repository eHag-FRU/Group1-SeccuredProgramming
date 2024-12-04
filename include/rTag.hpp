#ifndef RTAG_HPP
#define RTAG_HPP

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

// Trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// Trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// Trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// Remove null characters from a string
static inline void removeNulls(std::string &s) {
    s.erase(std::remove(s.begin(), s.end(), '\0'), s.end());
}

// Function to split a string by a delimiter and return a vector of substrings
vector<string> splitString2(const string& str, char delimiter) {
    vector<string> tokens;
    std::istringstream stream(str);
    string token;
    
    while (getline(stream, token, delimiter)) {
        removeNulls(token); // Remove null characters
        trim(token); // Trim whitespace
        tokens.push_back(token);
    }
    
    return tokens;
}
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

	//cout << "in rTagFunctionality" << endl;

	if (cmdLine.find(G) != cmdLine.end()) {
		//Guest name
		isEmployee = false;
		name = cmdLine[G];
	} else if (cmdLine.find(E) != cmdLine.end()) {
		//Employee name
		isEmployee = true;
		name = cmdLine[E];	
	} 

	//Open the file
	ifstream logFile;

	logFile.open(cmdLine["logFile"], std::ios::in | std::ios::binary);

	if (!logFile.is_open()) {
		//Could not open the file
		//std::cerr << "File could not be opened for reading in rTagFunctionality!" << std::endl;
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
        //std::cerr << "The file is empty." << std::endl;
		roomNumbers.push_back("-1");
        return roomNumbers;
    }
	 else {		
		logFile.seekg(0, std::ios::beg); 								// Reset to beginning of the file
																		// if the file is not empty, then we can start reading the file	
		while(getline(logFile, currentLine)) {
			

			//Decrypt the current line
			if(!debugMode) {
				currentLine = decrypt(currentLine, cmdLine["-K"], cmdLine);
			}

			// split the line by space
			vector<string> line = splitString2(currentLine, ' ');

			// cout << "line size: " << line.size() << endl;
			// check if a room number is input. if it doesnt the vector will have a size of 4
			if (line.size() > 4) {
				// check if the name is in the line
				auto nameIt = find(line.begin(), line.end(), name);
				if (nameIt != line.end()) {
					// check if the person entered a room 
					auto roomIt = find(line.begin(), line.end(), "A");
					if (roomIt != line.end()) {
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
					} else{
						// probably hit a -L tag
						continue;
					}
				}
			}
		}

		// check if the name was never found in the log file
		if (roomNumbers.size() == 0) {
			//std::cerr << "Name not found in the log file or never entered a room!" << std::endl;
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

#endif