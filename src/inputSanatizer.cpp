#include "inputSanatizer.hpp"
#include "encryptionHandler.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <vector>
#include <utility>
#include <sstream>

using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;
using std::fstream; using std::getline; using std::strtok; using std::vector; using std::ios; using std::pair;
using std::ifstream; using std::stoi; using std::cerr;


//
//
//  This meets the sanitization part of secure coding practices
//  This also checks to ensure valid input ranges too
//

bool sanatizeTime(int argc, char* argv[], map<string,string>& result) {
    //Now we have a valid time flag
    //Now check the time stamp
    //Valid range 1 to 1073741823 

    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);
    

    //Run the matcher and check
    if(!regex_match(argv[1], timeStampDashMatcher)) {
        //Bad time flag
        cout << "Bad time flag found" << endl;
       return false;
    }

    //Ensures its at least 1 digit and ensures its just a number, not with anything extra (%x or other)
    regex timeStampMatcher("[0-9]+", std::regex_constants::ECMAScript);
     if(!regex_match(argv[2], timeStampMatcher)) {
        //Bad time
        cout << "Bad time found" << endl;
       return false;
    }


    //convert to int
    int timeStamp = stoi(argv[2]);

    if (timeStamp != 0) {
        //Check if with in valid range
        if (timeStamp >= 1 && timeStamp <= 1073741823) {
            //Valid timestamp was found, now add to map
            result.insert(pair<string,string>(argv[1], argv[2]));
        } else {
            //Invalid timestamp
            //cout << "Time stamp not in range" << endl;
            return false;
        }
    } else {
        //0 means no integer was found, and can not have time stamp 0
        cout << "Invalid time stamp" << endl;
        return false;
    }

    return true;
}


bool sanatizeToken(int argc, char* argv[], map<string,string>& result) {
    regex tokenDashMatcher("-K", std::regex_constants::ECMAScript);

    if (!regex_match(argv[3], tokenDashMatcher)) {
        return false;
    }

    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if(regex_match( argv[4], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            bool sanatizeToken(int argc, char* argv[], map<string,string>& result);
            //result["K"] = string(argv[4]);
            result.insert(pair<string,string>(argv[3], argv[4]));
        } else {
            // Token was not valid
            return false;
        }

    return true;
}


bool sanatizeEmployeeOrGuestName(int argc, char* argv[], map<string,string>& result, int tagPostion) {

    //Guest OR Employee
    regex guestNameDashMatcher("-G", std::regex_constants::ECMAScript);
    regex employeeNameDashMatcher("-E", std::regex_constants::ECMAScript);

    regex guestEmployeeNameMatcher("([a-z]|[A-Z])*", std::regex_constants::ECMAScript);

	int flagPosition;
	
    if (regex_match(argv[tagPostion], employeeNameDashMatcher)) {
        //Now we have a valid employee flag
        //Now check the employee name
        if(regex_match( argv[tagPostion + 1], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["E"] = string(argv[4]);
            result.insert(pair<string,string>(argv[tagPostion], argv[tagPostion + 1]));
        } else {
            // Employee name was not valid
            cout << "Employee name was not valid!" << endl;
            cout << "Employee Name: |" << argv[tagPostion + 1] << "|" << endl;
            return false;
        }
    } else if (regex_match(argv[tagPostion], guestNameDashMatcher)) {
        //Now we have a valid guest flag
        //Now check the guest name
        if(regex_match( argv[tagPostion + 1], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            //resultingMap["G"] = string(argv[4]);
            result.insert(pair<string,string>(argv[tagPostion], argv[tagPostion + 1]));
        } else {
            // Guest name was not valid
            cout << "Guest name was not valid!" << endl;
            cout << "Guest Name: |" << argv[tagPostion + 1] << "|" << endl;
            return false;
        }
    } else {
        cout << "Guest OR Employee flag was not present!" << endl;
        //Token flag was not found
        return false;
    }

    return true;
}

bool sanatizeAriveLeaveTag(int argc, char* argv[], map<string,string>& result, int tagPositon) {
    regex arrivalDashMatcher("-A", std::regex_constants::ECMAScript);
    regex leaveDashMatcher("-L", std::regex_constants::ECMAScript);

    if (regex_match(argv[tagPositon], arrivalDashMatcher)) {
        result.insert(pair<string,string>(argv[tagPositon], "A"));

    } else if (regex_match(argv[tagPositon], leaveDashMatcher)) {
        result.insert(pair<string,string>(argv[tagPositon], "L"));

    } else {
        return false;
    }

    return true;
}

bool sanatizeRoomID(int argc, char* argv[], map<string,string>& result) {
    //Room IDs are non-negative integer 
    //characters with no spaces (ranging from 0 to 1073741823 inclusively)

    //convert to int
    int roomNumber = atoi(argv[9]);

    if (roomNumber != 0) {
        //Check if with in valid range
        if (roomNumber >= 1 && roomNumber <= 1073741823) {
            //Valid room number was found, now add to map
            result.insert(pair<string,string>(argv[8], argv[9]));
        } else {
            //Invalid room number
            return false;
        }
    } else {
        //0 means no integer was found, and can not have room number 0
        return false;
    }

    return true;
}

bool sanatizeFilePath(int argc, char* argv[], map<string,string>& result) {
   // cout << "In sanatizeFilePath, line 94" << endl;
    //cout << "argc:" << argc << endl;

    //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
    //Then adapted and tweaked to allow for optional path characters
    regex logFileNameMatcher("^(\\\\)*(.+\\\\)*(.+)", std::regex_constants::ECMAScript);
    //cout << regex_match(argv[8], logFileNameMatcher) << endl;

    //Two routes, depending on if the room argument is given
    if (argc == 11) {
        //cout << "In 11 path" << endl;
        //Room argument path
       // cout << argv[10] << endl;
        if (regex_match(argv[10], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert(pair<string,string>("logFile", argv[10]));
        } else {
            //No file name found or invalid
            return false;
        }
    } else if (argc == 9) {
        //cout << "In 9 path" << endl;
        //Non-Room Argument Path
        if (regex_match(argv[8], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert(pair<string,string>("logFile", argv[8]));
        } else {
            cout << "FAILED LOGFILE PATH CHECK!" << endl;
            //No file name found or invalid
            return false;
        }
    } else {
        //Not the correct number of arguments!
        return false;
    }


    return true;
}


bool sanatizeInput(int fargc, char* fargv[], map<string, string>& result) {
    //cout << ("Sanatizing the input!!!!") << endl;

    //Check for the correct argument count again
    if (fargc != 9 && fargc != 11) {
		cout << "Invalid number of arguments" << endl;
        return false;
    }

	// insert inittial argv value
	result["programName"] = "logappend";

    //
    //TIME
    //

    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);

    //Run the matcher and check
	//cout << "argv[1]: " << fargv[1] << endl;
    if(regex_match(fargv[1], timeStampDashMatcher)) {
        //Good time flag found, now pull in the time
        if (!sanatizeTime(fargc, fargv, result)) {
            //Bad time stamp
			cerr << "Invalid time stamp" << endl;
            return false;
        }
    } else {
        //No time flag found
        cout << "No -T Found" << endl;
        return false;
    }

    //
    //  TOKEN
    //
    regex tokenDashMatcher("-K", std::regex_constants::ECMAScript);

    if (regex_match(fargv[3], tokenDashMatcher)) {
        //Now we have a valid token flag
        if(!sanatizeToken(fargc, fargv, result)) {
			cerr << "Invalid token" << endl;
            return false;
        }
    } else {
		cerr << "tokenDashMatcher failed" << endl;
        return false;
    }


    //
    //  Employee OR Guest OR Arrive OR Leave
    //
    regex nameTagDashMatcher("(-E|-G){1,1}", std::regex_constants::ECMAScript);
    regex arrivalLeaveTagDashMatcher("(-A|-L){1,1}", std::regex_constants::ECMAScript);

    if (regex_match(fargv[5], nameTagDashMatcher)) {
		cout << "in the name tag dash matcher" << endl;
        if (!sanatizeEmployeeOrGuestName(fargc, fargv, result, 5)) {
            //Failed to sanatize and parse
			cerr << "Invalid employee or guest name" << endl;
            return false;
        }

        //Now parse the arrive/leave tag
        if(!sanatizeAriveLeaveTag(fargc, fargv, result, 7)) {
            //Failed to sanatize and parse
			cerr << "Invalid arrival or leave tag" << endl;
            return false;
        }

    } else if (regex_match(fargv[5], arrivalLeaveTagDashMatcher)) {
        //Now parse the arrive/leave tag
        if(!sanatizeAriveLeaveTag(fargc, fargv, result, 5)) {
            //Failed to sanatize and parse
			cerr << "Invalid arrival or leave tag" << endl;
            return false;
        } 

        if (!sanatizeEmployeeOrGuestName(fargc, fargv, result, 6)) {
            //Failed to sanatize and parse
			cerr << "Invalid employee or guest name" << endl;
            return false;
        }

        

    } else {
        //Not a valid tag at position
        cout <<"Failed at 209" << endl;
        return false;
    }

    //
    // ROOM
    //
    if (fargc == 11) {
        regex roomDashMatcher("-R", std::regex_constants::ECMAScript);
        if (regex_match(fargv[8], roomDashMatcher)) {
            //Now we have a valid token flag
            if (!sanatizeRoomID(fargc, fargv, result)) {
				cerr << "Invalid room ID" << endl;
                return false;
            }
        } else {
			cerr << "roomDashMaster failed" << endl;
            return false;
        }
    }

    //
    // LOG FILE
    //
    if (!sanatizeFilePath(fargc, fargv, result)) {
		cerr << "Invalid file path" << endl;
        return false;
    }

    return true;
}


void resultMapToString(map<string,string>& sanatizedResult) {
    //Print out the map
    //cout << "Resulting map size: " << sanatizedResult.size() << endl;

    auto iterator = sanatizedResult.begin();

    cout << "sanatizedResult: " << endl;
    while (iterator != sanatizedResult.end()) {
        cout << "[" << iterator->first << "] = " << iterator->second << endl;

        iterator++;
    }
}


bool validTimeStamp(map<string, string>& commandLineArguments) {
   // cout << "validating time stamp" << endl;
    //cout << "logFile open? : " << logFile.is_open() << endl;
    //cout << "logFile at EOF?: " << logFile.eof() << endl;

//     //Close and reopen on byte stream
//     logFile.close();

    ifstream logFile;
    logFile.open(commandLineArguments["logFile"], std::ios::in | std::ios::binary);

    //Cycle through and get the second to last line
    //Reason? ====> last line in the file will always be a blank line
    //This is caused by endl being added to the end of each append, therefore moving
    //the cursor down to the next line in the file (1 log entry per 1 line in file)
   string currentLine;
   string lastLine;

   // cout << "BEFORE lastLine SET, IS EMPTY?: " << lastLine.empty() << endl;

   while (getline(logFile, currentLine) && !currentLine.empty()) {
        lastLine = currentLine;
   }

    //Empty? => NO FILE CONTENTS!!!, automatically valid!!!
    if (lastLine.empty()) {
        //Automatically true
        //Since the log is EMPTY!
        return true;
    }
   
    
    //We have the last line

    //Need to decrypt the line to be able to read it
    lastLine = decrypt(lastLine, commandLineArguments["-K"], commandLineArguments);

    if (lastLine == "") {
        return false;
    }

    //cout << "logFile at EOF?: " << logFile.eof() << endl;
    //cout << "lastLine: "<< lastLine << endl;


   //Now parse into an array of substrings
   //split on the single space

   // cout << "Last Time Stamp: " << lastLine[0] << endl;

	int lastTimeStamp = atoi(string(1, lastLine[0]).c_str());

    if (lastTimeStamp == 0) {
        //Conversion failed
        //OR is a 0 time stamp (not valid)
		cerr << "failed at 400" << endl;
        return false;
    }

    int newLineTimeStamp = atoi(commandLineArguments["-T"].c_str());

    if (newLineTimeStamp == 0) {
        //Conversion failed
        //OR is a 0 time stamp (not valid)
		cerr << "failed at 408" << endl;
        return false;
    }

    if (newLineTimeStamp < lastTimeStamp) {
        //INVALID TIME STAMP FOUND!!!
		cerr << "failed at 413" << endl;
        return false;
    }
   
    //Time stamp is valid!!!
    return true;
}

// Function to split a string by a delimiter and return a vector of substrings
vector<string> splitStringT(const string& str, char delimiter) {
    vector<string> tokens;
    std::istringstream stream(str);
    string token;
    
    while (getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}


bool validArrivalLeave(map<string, string>& commandLineArguments, string name, bool testFlag) {
    // No employee or guest should enter a room without first entering the gallery. 
    //No employee or guest should enter a room without having left a previous room. 
    //Violation of either of these conditions implies inconsistency with the current 
    //log state and should result in logappend exiting with an error condition.
    //Same for leave

    //Open the file
    ifstream logFile;

	// determines if the person has only entered the gallery
	bool onlyEnteredGallery = false;

    cout << "in valid arrival/leave" << endl;
	resultMapToString(commandLineArguments);

	// if(commandLineArguments["-L"] == "L"){
	// 	cout << name << "it can find the L" << endl;
	// }

    logFile.open(commandLineArguments["logFile"], std::ios::in | std::ios::binary);

    if (!logFile.is_open()) {
        // Could not open the file, attempt to create it
        cout << "LogFile Name: " << commandLineArguments["logFile"] << endl;
        cout << "validArrivalLeave: COULD NOT OPEN FILE, CREATING NEW FILE" << endl;

        std::ofstream newLogFile(commandLineArguments["logFile"], std::ios::out | std::ios::binary);
        if (!newLogFile.is_open()) {
            cout << "validArrivalLeave: COULD NOT CREATE FILE" << endl;
            return false;
        }
        newLogFile.close();

        // Reopen the file as input
        logFile.open(commandLineArguments["logFile"], std::ios::in | std::ios::binary);
        if (!logFile.is_open()) {
            cout << "validArrivalLeave: COULD NOT OPEN NEWLY CREATED FILE" << endl;
            return false;
        }
    }

	

    //cout << "IN validArrivalLeave" << endl;

    //Make a vector that holds the last line of what the guest/employee was doing
    vector<string> personLastActionLine;
	vector<string> personLastRoomLine;

    //Go through the log
    string currentLine;

    //Clear the current error flag and EOF flag
    //Then seek to the top of the file
    logFile.clear();
    logFile.seekg(0);

   // cout << "logFile at EOF?: " << logFile.eof() << endl;
	//cout << "command line arguments size: " << commandLineArguments.size() << endl;
	// Check if the file is empty and then only return true when the command line arguments are -A A and the size of the command line arguments is 6
	logFile.seekg(0, ios::end);
	if (logFile.tellg() == 0 && commandLineArguments.size() == 6) {
		cout << "The log file is empty." << endl;
		return true;
	}

	if(commandLineArguments["-A"] == "A" && commandLineArguments.size() == 6){
		cout << name << " just entered the gallery." << endl;
		return true;
	}

	//Reset the file to the beginning

	logFile.seekg(0, ios::beg); // Reset to beginning of the file

    while (getline(logFile, currentLine)) {
        //Decrypt the line
        if (!testFlag) {
            currentLine = decrypt(currentLine, commandLineArguments["-K"], commandLineArguments);

             //Error check
            if (currentLine == "") {
                return false;
            }

        }

        //Find the guest or employee flag
        //E OR G will be the first occurance in the log
		//split by " " and save strings to vector
		vector<string> currentLineSplit = splitStringT(currentLine, ' ');
		
		//Check if the name is in the line and save the initial action for the initial arrival
        if (currentLine.find(name) != string::npos && personLastActionLine.empty()) {
			
			if(commandLineArguments.size() == 6 && commandLineArguments["-A"] == "A") {
				return true;
			}else {
				//save action to personLastActionLine
				for (int i = 0; i < currentLineSplit.size(); i++) {
					if(currentLineSplit[i] == "A" || currentLineSplit[i] == "L") {
						if (currentLineSplit[i] == "L") {
							cout << "L is found when saving action" << endl;
						}
						
						personLastActionLine.push_back(currentLineSplit[i]);
						onlyEnteredGallery = true;
					}
				}
				continue;
			}
		} 
		
		//find last action and room of the person
		
		if (currentLine.find(name) != string::npos) {
			//save action to personLastActionLine
			for (int i = 0; i < currentLineSplit.size(); i++) {
					if(currentLineSplit[i] == "A") { 
						personLastActionLine.push_back(currentLineSplit[i]);
						personLastRoomLine.push_back(currentLineSplit[i+1]);
						onlyEnteredGallery = false;												// this ensures that after a person enters a room that they must leave before entering again
					} else if (currentLineSplit[i] == "L") {
						personLastActionLine.push_back(currentLineSplit[i]);
						onlyEnteredGallery = false;
					}
				}
		} 
    }
	int roomVecSize = personLastRoomLine.size();
	int actionVecSize = personLastActionLine.size();
	if (actionVecSize == 0) {
		//Person has not entered the gallery
		return false;
	}
	//Check if the person is entering or leaving
	if (commandLineArguments["-A"] == "A") {
		//Check if the last action was a leave
		if (personLastActionLine[actionVecSize - 1] == "L" || onlyEnteredGallery == true) {
			//Valid
			return true;
		} else {
			std::cerr << "ERROR: " << name << " has not left the previous room" << std::endl;
			return false;
		}
	} else if (commandLineArguments["-L"] == "L") {
		//Check if the last action was an arrival
		

		// initial if statement is to check if the person has only entered the gallery, if so then we dont need to check for the room
		// this is really ugly im sorry
		if (roomVecSize == 0) {
			if (personLastActionLine[actionVecSize - 1] == "A" && onlyEnteredGallery == false) {
				//Valid
				return true;
			} else {
				std::cerr << "ERROR: " << name << " has not entered the gallery" << std::endl;
				return false;
			}
		} else {
			if (personLastActionLine[actionVecSize - 1] == "A" && commandLineArguments["-R"] == personLastRoomLine[roomVecSize - 1] && onlyEnteredGallery == false) {
				//Valid
				return true;
		} else {
			std::cerr << "ERROR: " << name << " has not entered the gallery" << std::endl;
			return false;
		}
		}
		
	}

    //Everything is valid, so return true
    return true;
}