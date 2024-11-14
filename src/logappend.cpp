#include "../include/logappend.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <vector>
//#include <openssl/evp.h>
//#include <openssl/conf.h>
#include <sstream>

using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;
using std::fstream; using std::getline; using std::strtok; using std::vector; using std::ios;


bool sanatizeTime(int argc, char* argv[], map<string,string>& result) {
    //Now we have a valid time flag
    //Now check the time stamp
    //Valid range 1 to 1073741823 

    //convert to int
    int timeStamp = atoi(argv[2]);

    if (timeStamp != 0) {
        cout << "sanatizeTime, line 20: timeStamp > 0!!" << endl;
        //Check if with in valid range
        if (timeStamp >= 1 && timeStamp <= 1073741823) {
            cout << "sanatizeTime, line 24: timeStamp >= 1 && timeStamp <= 1073741823!!" << endl;
            //Valid timestamp was found, now add to map
            result.insert({argv[1], argv[2]});
        } else {
            //Invalid timestamp
            cout << "Time stamp not in range" << endl;
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
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if(regex_match( argv[4], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["K"] = string(argv[4]);
            result.insert({argv[3], argv[4]});
        } else {
            // Token was not valid
            cout << "Token was not valid!" << endl;
            cout << "Token: |" << argv[4] << "|" << endl;
            return false;
        }

    return true;
}


bool sanatizeEmployeeOrGuestName(int argc, char* argv[], map<string,string>& result, int tagPostion) {

    //Guest OR Employee
    regex guestNameDashMatcher("-G", std::regex_constants::ECMAScript);
    regex employeeNameDashMatcher("-E", std::regex_constants::ECMAScript);

    regex guestEmployeeNameMatcher("([a-z]|[A-Z])*", std::regex_constants::ECMAScript);

    if (regex_match(argv[tagPostion], employeeNameDashMatcher)) {
        //Now we have a valid employee flag
        //Now check the employee name
        if(regex_match( argv[tagPostion + 1], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["E"] = string(argv[4]);
            result.insert({argv[tagPostion], argv[tagPostion + 1]});
        } else {
            // Employee name was not valid
            cout << "Employee name was not valid!" << endl;
            cout << "Employee Name: |" << argv[6] << "|" << endl;
            return false;
        }
    } else if (regex_match(argv[5], guestNameDashMatcher)) {
        //Now we have a valid guest flag
        //Now check the guest name
        if(regex_match( argv[6], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            //resultingMap["G"] = string(argv[4]);
            result.insert({argv[tagPostion], argv[tagPostion + 1]});
        } else {
            // Guest name was not valid
            cout << "Guest name was not valid!" << endl;
            cout << "Guest Name: |" << argv[6] << "|" << endl;
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
        result.insert({argv[tagPositon], "A"});

    } else if (regex_match(argv[tagPositon], leaveDashMatcher)) {
        result.insert({argv[tagPositon], "L"});

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
            result.insert({argv[8], argv[9]});
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
    cout << "In sanatizeFilePath, line 94" << endl;
    cout << "argc:" << argc << endl;

    //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
    //Then adapted and tweaked to allow for optional path characters
    regex logFileNameMatcher("^(\\\\)*(.+\\\\)*(.+)", std::regex_constants::ECMAScript);
    cout << regex_match(argv[8], logFileNameMatcher) << endl;

    //Two routes, depending on if the room argument is given
    if (argc == 11) {
        cout << "In 11 path" << endl;
        //Room argument path
        cout << argv[10] << endl;
        if (regex_match(argv[10], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert({"logFile", argv[10]});
        } else {
            //No file name found or invalid
            return false;
        }
    } else if (argc == 9) {
        cout << "In 9 path" << endl;
        //Non-Room Argument Path
        if (regex_match(argv[8], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert({"logFile", argv[8]});
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


bool sanatizeInput(int argc, char* argv[], map<string, string>& result) {
    cout << ("Sanatizing the input!!!!") << endl;

    //Check for the correct argument count again
    if (argc != 9 && argc != 11) {
        return false;
    }

    //
    //TIME
    //

    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);

    //Run the matcher and check
    if(regex_match(argv[1], timeStampDashMatcher)) {
        //Good time flag found, now pull in the time
        if (!sanatizeTime(argc, argv, result)) {
            //Bad time stamp
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

    if (regex_match(argv[3], tokenDashMatcher)) {
        //Now we have a valid token flag
        if(!sanatizeToken(argc, argv, result)) {
            return false;
        }
    } else {
        return false;
    }


    //
    //  Employee OR Guest OR Arrive OR Leave
    //
    regex nameTagDashMatcher("(-E|-G){1,1}", std::regex_constants::ECMAScript);
    regex arrivalLeaveTagDashMatcher("(-A|-L){1,1}", std::regex_constants::ECMAScript);

    if (regex_match(argv[5], nameTagDashMatcher)) {
        if (!sanatizeEmployeeOrGuestName(argc, argv, result, 5)) {
            //Failed to sanatize and parse
            return false;
        }

        //Now parse the arrive/leave tag
        if(!sanatizeAriveLeaveTag(argc, argv, result, 7)) {
            //Failed to sanatize and parse
            return false;
        }

    } else if (regex_match(argv[5], arrivalLeaveTagDashMatcher)) {

        //Now parse the arrive/leave tag
        if(!sanatizeAriveLeaveTag(argc, argv, result, 5)) {
            //Failed to sanatize and parse
            return false;
        }

        if (!sanatizeEmployeeOrGuestName(argc, argv, result, 6)) {
            //Failed to sanatize and parse
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
    if (argc == 11) {
        regex roomDashMatcher("-R", std::regex_constants::ECMAScript);
        if (regex_match(argv[8], roomDashMatcher)) {
            //Now we have a valid token flag
            if (!sanatizeRoomID(argc, argv, result)) {
                return false;
            }
        } else {
            return false;
        }
    }
    
    //
    // LOG FILE
    //
    if (!sanatizeFilePath(argc, argv, result)) {
        return false;
    }

    return true;
}

void resultMapToString(map<string,string>& sanatizedResult) {
    //Print out the map
    cout << "Resulting map size: " << sanatizedResult.size() << endl;

    auto iterator = sanatizedResult.begin();

    cout << "sanatizedResult: " << endl;
    while (iterator != sanatizedResult.end()) {
        cout << "[" << iterator->first << "] = " << iterator->second << endl;

        iterator++;
    }
}


bool validTimeStamp(map<string, string>& commandLineArguments, fstream& logFile) {
    cout << "validating time stamp" << endl;
    cout << "logFile open? : " << logFile.is_open() << endl;
    cout << "logFile at EOF?: " << logFile.eof() << endl;

    //Cycle through and get the last line
   string line;

   while(getline(logFile, line)) {
        //Do nothing
        cout << "Number of chars read previously: " << logFile.gcount() << endl;
   }

   cout << "logFile at EOF?: " << logFile.eof() << endl;
   cout << line << endl;

   //We have the last line

    //Empty? => NO FILE CONTENTS!!!, automatically valid!!!
    if (line.empty()) {
        //Automatically true
        //Since the log is EMPTY!
        return true;
    }


   //Now parse into an array of substrings
   //split on the single space

    cout << "Last Time Stamp: " << line[0] << endl;

    int lastTimeStamp = atoi(string({line[0]}).c_str());

    if (lastTimeStamp == 0) {
        //Conversion failed
        //OR is a 0 time stamp (not valid)
        return false;
    }

    int newLineTimeStamp = atoi(commandLineArguments["-T"].c_str());

    if (newLineTimeStamp == 0) {
        //Conversion failed
        //OR is a 0 time stamp (not valid)
        return false;
    }

    if (newLineTimeStamp < lastTimeStamp) {
        //INVALID TIME STAMP FOUND!!!
        return false;
    }
   
    //Time stamp is valid!!!
    return true;
}

bool validArrivalLeave(map<string, string>& commandLineArguments, fstream& logFile, string name) {
    // No employee or guest should enter a room without first entering the gallery. 
    //No employee or guest should enter a room without having left a previous room. 
    //Violation of either of these conditions implies inconsistency with the current 
    //log state and should result in logappend exiting with an error condition.
    //Same for leave

	bool justEnteredGallery = false;

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

    //cout << "logFile at EOF?: " << logFile.eof() << endl;

    while (getline(logFile, currentLine)) {

        //Find the guest or employee flag
        //E OR G will be the first occurance in the log
		//split by " " and save strings to vector
		vector<string> currentLineSplit = splitString(currentLine, ' ');

		//Check if the name is in the line and save the initial action for the initial arrival
        if (currentLine.find(name) != string::npos && personLastActionLine.empty()) {
			if(commandLineArguments.size() == 6 && commandLineArguments["-A"] == "A") {
				return true;
			}else {
				//save action to personLastActionLine
				for (int i = 0; i < currentLineSplit.size(); i++) {
					if(currentLineSplit[i] == "A" || currentLineSplit[i] == "L") {
						personLastActionLine.push_back(currentLineSplit[i]);
						justEnteredGallery = true;
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
						justEnteredGallery = false;												// this ensures that after a person enters a room that they must leave before entering again
					} else if (currentLineSplit[i] == "L") {
						personLastActionLine.push_back(currentLineSplit[i]);
						justEnteredGallery = false;
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
		if (personLastActionLine[actionVecSize - 1] == "L" || justEnteredGallery == true) {
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
			if (personLastActionLine[actionVecSize - 1] == "A" && justEnteredGallery == false) {
				//Valid
				return true;
			} else {
				std::cerr << "ERROR: " << name << " has not entered the gallery" << std::endl;
				return false;
			}
		} else {
			if (personLastActionLine[actionVecSize - 1] == "A" && commandLineArguments["-R"] == personLastRoomLine[roomVecSize - 1] && justEnteredGallery == false) {
				//Valid
				return true;
		} else {
			std::cerr << "ERROR: " << name << " has not entered the gallery" << std::endl;
			return false;
		}
		}
		
	}
}

 bool commandExecuter(int argc, char* argv[], map<string, string>& sanatizedResult) {
     //Have a full line of arguments
    cout << "Full line of arguments given" << endl;

    //Now send off to sanatize the full command
    //Returns T/F => T = Successful / F = Invalid
    if (!sanatizeInput(argc, argv, sanatizedResult)) {
        return false;
    }

    
    //Print out the resulting map
    resultMapToString(sanatizedResult);

    //Now open the map
    fstream log;

    log.open(sanatizedResult["logFile"]);

    //Check if error when opening,
    //if so, error or as invalid!
    if (!log.is_open()) {
        cout << "FILE NOT OPEN!" << endl;
        return false;
    }

    cout << "LOG IS OPEN" << endl;

    //Now open, now check if the timestamp is valid
    if (!validTimeStamp(sanatizedResult, log)) {
        return false;
    }

    //Now check for valid arrival/leave details
    if(!validArrivalLeave(sanatizedResult, log)) {
        return false; 
    }

    //
    // WRITING TO THE FILE, ALL GOOD SO NOW CAN WRITE!!
    //

    //Close the log file to reopen for appending only!!
    log.close();

    //Reopen with append privs ONLY!!!
    log.open(sanatizedResult["logFile"], std::ios::app); 

    string encryptedLine;

    //Encrypt with the key


    //Write to the file
    log.write(encryptedLine.c_str(), encryptedLine.size());

    //Close the file out
    log.close();

    return true;
}


 //OpenSSL Encryption & Decryption


int encrypt(unsigned char* text, int text_length, unsigned char* key, unsigned char* cipher) {
    int cipher_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);

    EVP_EncryptUpdate(ctx, cipher, &len, text, text_length);

    cipher_len += len;

    EVP_EncryptFinal_ex(ctx, cipher + len, &len);

    cipher_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return cipher_len;
}


// Function to split a string by a delimiter and return a vector of substrings
vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    std::istringstream stream(str);
    string token;
    
    while (getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}







int main(int argc, char* argv[]) {
    map<string, string> sanatizedResult;

    //Inital check of how many arguments
    cout << "Argument count: " << argc << endl;

    //Check the count, determine if batch or if line is full command
    if (argc == 3) {
        cout << "Given batch file" << endl;

        //Batch file file stream
        fstream batchFile;

        //Open the batch file

        if(!batchFile.is_open()) {
            //File could not be opened
            cout << "invalid" << endl;
            return 255;
        }

        //Grabs each line of input and executes it
        string currentLine;
        
        while(getline(batchFile, currentLine)) {
            //Split into argument array of c_strs

            //Grab count

            //Parsed, to send to execute each line
        }

        

    } else if (argc == 9 || argc == 11) {
       if(!commandExecuter(argc, argv, sanatizedResult)) {
            cout << "invalid" << endl;
            return false;
       }

    } else {
        cout << "invalid" << endl;
        return 255;
    }


    return 0;
}