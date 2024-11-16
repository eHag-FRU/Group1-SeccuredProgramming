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

    

    

    //Now open, now check if the timestamp is valid
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
            return 255;
       }

    } else {
        cout << "invalid" << endl;
        return 255;
    }


    return 0;
}