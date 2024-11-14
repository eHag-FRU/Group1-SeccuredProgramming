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
using std::fstream; using std::getline; using std::strtok; using std::vector; using std::ios;





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

    //Now decrypt the line
    line = decrypt(line, commandLineArguments["-K"]);
    cout << "Decrypted Line: " << line << endl;

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

bool validArrivalLeave(map<string, string>& commandLineArguments, fstream& logFile) {
    // No employee or guest should enter a room without first entering the gallery. 
    //No employee or guest should enter a room without having left a previous room. 
    //Violation of either of these conditions implies inconsistency with the current 
    //log state and should result in logappend exiting with an error condition.
    //Same for leave

    cout << "IN validArrivalLeave" << endl;

    //Make a vector that holds the last line of what the guest/employee was doing
    vector<char*> personLastActionLine;

    //Go through the log
    string currentLine;

    //Clear the current error flag and EOF flag
    //Then seek to the top of the file
    logFile.clear();
    logFile.seekg(0);

    cout << "logFile at EOF?: " << logFile.eof() << endl;

    while (getline(logFile, currentLine)) {
        cout << currentLine << endl;

        //Find the guest or employee flag
        //E OR G will be the first occurance in the log
        //if ()
    }

    return true;
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

    string logLine = sanatizedResult["-T"];



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

    cout << "Line: " << logLine << endl;

    string encryptedLine;

    //Encrypt with the key
    encryptedLine = encrypt(encryptedLine, sanatizedResult["-K"], sanatizedResult);

    cout << encryptedLine << endl;

    //Write to the file
    log.write(encryptedLine.c_str(), encryptedLine.size());

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