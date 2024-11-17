#include "logappend.hpp"
#include "inputSanatizer.hpp"
#include "encryptionHandler.hpp"
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>




using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;
using std::fstream; using std::getline; using std::strtok; using std::vector; using std::ios; using std::vector;
using std::ifstream;





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

    //Close and reopen on byte stream
    logFile.close();
    logFile.open(commandLineArguments["logFile"], std::ios::in | std::ios::binary);

    //Cycle through and get the last line
   string line;

   //Flush to go back to the start of the file
    //Clear the current error flag and EOF flag
    //Then seek to the top of the file
    logFile.clear();
    logFile.seekg(0);

   while(getline(logFile, line)) {
        //Do nothing
   }

    //Need to decrypt the line to be able to read it
    line = decrypt(line, commandLineArguments["-K"], commandLineArguments);

   cout << "logFile at EOF?: " << logFile.eof() << endl;
   cout << "line: "<< line << endl;

   //We have the last line

    //Empty? => NO FILE CONTENTS!!!, automatically valid!!!
    if (line.empty()) {
        //Automatically true
        //Since the log is EMPTY!
        return true;
    }

    //Now decrypt the line
    line = decrypt(line, commandLineArguments["-K"], commandLineArguments);
    cout << "Decrypted Line: " << line << endl;

   //Now parse into an array of substrings
   //split on the single space

    cout << "Last Time Stamp: " << line[0] << endl;

	int lastTimeStamp = atoi(std::string(1, line[0]).c_str());
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


bool validArrivalLeave(map<string, string>& commandLineArguments, string name) {
    // No employee or guest should enter a room without first entering the gallery. 
    //No employee or guest should enter a room without having left a previous room. 
    //Violation of either of these conditions implies inconsistency with the current 
    //log state and should result in logappend exiting with an error condition.
    //Same for leave

    //Open the file
    ifstream logFile;

    logFile.open(commandLineArguments["logFile"], std::ios::in | std::ios::binary);

    if (!logFile.is_open()) {
        //Could not open the file
        cout << "validArrivalLeave: COULD NOT OPEN FILE" << endl;
        return false;
    }

	bool justEnteredGallery = false;

    //cout << "IN validArrivalLeave" << endl;

    //Make a vector that holds the last line of what the guest/employee was doing
    vector<string> personLastActionLine;
	vector<string> personLastRoomLine;

    //Go through the log
    string currentLine;

    //Clear the current error flag and EOF flag
    //Then seek to the top of the file
    //logFile.clear();
    //logFile.seekg(0);

    //cout << "logFile at EOF?: " << logFile.eof() << endl;

    while (getline(logFile, currentLine)) {

        //Decrypt the line
        currentLine = decrypt(currentLine, commandLineArguments["-K"], commandLineArguments);

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

    //Everything is valid, so return true
    return true;
}





bool commandExecuter(int argc, char* argv[], map<string, string>& sanatizedResult) {
    //  //Have a full line of arguments
    // cout << "Full line of arguments given" << endl;

    // //Now send off to sanatize the full command
    // //Returns T/F => T = Successful / F = Invalid
    if (!sanatizeInput(argc, argv, sanatizedResult)) {
        return false;
    }

    
    //Print out the resulting map
    resultMapToString(sanatizedResult);

    //Now open the map
    fstream log;

    // log.open(sanatizedResult["logFile"], std::ios::app | std::ios::binary);

    

    

    // //Now open, now check if the timestamp is valid
    // if (!validTimeStamp(sanatizedResult, log)) {
    //     return false;
    // }

    cout << "NOW VALIDATING ARRIVAL & LEAVE" << endl;

    //Grab the name
    string name;
    if (sanatizedResult.find("-G") != sanatizedResult.end()) {
        //Guest name
        name = sanatizedResult["-G"];
    } else {    //Employee
        name = sanatizedResult["-E"];
    }

    //Now check for valid arrival/leave details
    // if(!validArrivalLeave(sanatizedResult, name)) {
    //     cout << "VALID ARRIVAL/LEAVE FAILED!" << endl;
    //     return false; 
    // }

    // //
    // // WRITING TO THE FILE, ALL GOOD SO NOW CAN WRITE!!
    // //

    // //Close the log file to reopen for appending only!!
    // log.close();

    //Reopen with append privs ONLY!!!
    log.open(sanatizedResult["logFile"], std::ios::app | std::ios::binary); 

    //Check if error when opening,
    //if so, error or as invalid!
    if (!log.is_open()) {
        cout << "FILE NOT OPEN!" << endl;
        return false;
    }


    string logLine = sanatizedResult["-T"];
    //string logLine = "1 A G Jeff";



    //Arrive OR Leave
    if (sanatizedResult.find("-A") != sanatizedResult.end()) {
        logLine.append(" A");
    } else {
        //Leave
        logLine.append(" L");
    }

    //Room number
    if (sanatizedResult.find("-R") != sanatizedResult.end()) {
        int roomNumber = atoi(sanatizedResult["-R"].c_str());

        logLine.append(" " + std::to_string(roomNumber));
    }

    //Guest OR Employee
    if (sanatizedResult.find("-G") != sanatizedResult.end()) {
        logLine.append(" G " + sanatizedResult["-G"]);
    } else {
        //Employee
        logLine.append(" E " + sanatizedResult["-E"]);

    }

    //Add an endl to the file

    cout << "Line: " << logLine << endl;
    cout << "Key: " << sanatizedResult["-K"] << endl;

    string encryptedLine;
    string decryptedLine;

    //Encrypt with the key
    encryptedLine = encrypt(logLine, sanatizedResult["-K"], sanatizedResult);

    cout << "ENCYRPTED LINE (329 logappend.cpp): " << encryptedLine << endl;

    decryptedLine = decrypt(encryptedLine, sanatizedResult["-K"], sanatizedResult);

    cout << "DECRYPTED LINE (335 logappend.cpp): " << decryptedLine << endl;

    //Write to the file
    log.write(encryptedLine.c_str(), encryptedLine.size());

    //Write a endl
    log << endl;

    //Close the file out
    log.close();

    return true;
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