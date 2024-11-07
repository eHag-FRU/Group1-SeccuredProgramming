#include "../include/logappend.hpp"
#include <iostream>
#include <regex>
#include <fstream>



using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;
using std::fstream;

bool sanatizeEmployeeGuestName(char* argv[], map<string, string>& resultingMap) {
    //Guest OR Employee
    regex guestNameDashMatcher("-G", std::regex_constants::ECMAScript);
    regex employeeNameDashMatcher("-E", std::regex_constants::ECMAScript);

    regex guestEmployeeNameMatcher("([a-z]|[A-Z])*", std::regex_constants::ECMAScript);

    if (regex_match(argv[5], employeeNameDashMatcher)) {
        //Now we have a valid employee flag
        //Now check the employee name
        if(regex_match( argv[6], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            resultingMap["E"] = string(argv[4]);
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
            resultingMap["G"] = string(argv[4]);
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

//bool sanatizeEmployeeGuestName(char* argv[], map<string, string>& resultingMap)

bool sanatizeInput(int argc, char* argv[], map<string, string>& resultingMap) {
    cout << ("Sanatizing the input!!!!") << endl;

    ///Check for the correct argument count again
    if (argc != 9 && argc != 11) {
        return false;
    }

    //Now make the regex matchers
    
    //TimeStamp
    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);

    //Run the matcher and check
    if(regex_match(argv[1], timeStampDashMatcher)) {
        //Now we have a valid time flag
        //Now check the time stamp
        //Valid range 1 to 1073741823 
        
    } else {
        cout << "Timestamp flag was not found!" << endl;
        //The -T is missing
        return false;
    }


    //Token
    regex tokenDashMatcher("-K", std::regex_constants::ECMAScript);
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if (regex_match(argv[3], tokenDashMatcher)) {
        //Now we have a valid token flag
        //Now check the token
        if(regex_match( argv[4], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            resultingMap["K"] = string(argv[4]);
        } else {
            // Token was not valid
            cout << "Token was not valid!" << endl;
            cout << "Token: |" << argv[4] << "|" << endl;
            return false;
        }
    } else {
        cout << "Token flag was not present!" << endl;
        //Token flag was not found
        return false;
    }

    //Check for the next flag, either guest/employee OR
    // arrival or leave
    if (argv[5] == "-G" || argv[5] == "-E") {
        sanatizeEmployeeGuestName(argv, resultingMap);
    } else if (argv[5] == "-A" || argv[5] == "-L") {
        //Arrival or leave
        cout << "Found arrival or leave!" << endl;
    } else {
        //Invalid flag!
        return false;
    }


    

    
    //Room ID (OPTIONAL ARGUMENT)
    if (argc == 11) {
        //Then there is an actual room id
        regex roomIDDashMatcher("-R", std::regex_constants::ECMAScript);
        regex roomIDMatcher("^[0-9]+$", std::regex_constants::ECMAScript);

        if (regex_match(argv[8], roomIDDashMatcher)) {
            //Now we have a valid room id flag
            //Now check the room id
            if(regex_match( argv[9], roomIDMatcher)) {
                //Now sanitized and safe, now put in the map
                int roomIDValue = std::stoi(argv[9]);
                if (roomIDValue >= 0 && roomIDValue <= 1073741823) {
                    // Room ID is valid
                    resultingMap["R"] = string(argv[9]);
                } else {
                    // Room ID is out of range
                    cout << "Room ID was out of range!" << endl;
                    return false;
                }
                
            } else {
                // Room ID was not valid
                cout << "Room ID was not valid!" << endl;
                cout << "Room ID: |" << argv[9] << "|" << endl;
                return false;
            }
        } else {
            cout << "Room ID flag was not present!" << endl;
            //Room ID flag was not found
            return false;
        }

        //Logfile name
        //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
        regex logFileNameMatcher("[^\\](.+)\.txt$", std::regex_constants::ECMAScript);
        if(regex_match(argv[10], logFileNameMatcher)) {
            //Now we have a valid log file name
            resultingMap["logFile"] = string(argv[10]);
        } else {
            cout << "Log file name was not valid!" << endl;
            cout << "Log file name: |" << argv[10] << "|" << endl;
            return false;
        }

    }

    // Room ID with 9 command line arguments
    if (argc == 9) {
        //Logfile name
        //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
        regex logFileNameMatcher("[^\\](.+)\.txt$", std::regex_constants::ECMAScript);
        if(regex_match(argv[8], logFileNameMatcher)) {
            //Now we have a valid log file name
            resultingMap["logFile"] = string(argv[8]);
        } else {
            cout << "Log file name was not valid!" << endl;
            cout << "Log file name: |" << argv[8] << "|" << endl;
            return false;
        }
    }
  



    
   


    return true;
}


bool appendToLog(map<string, string>& argumentMap) {
    //Check if the log exits
    fstream log(argumentMap["logFile"]);

    if (!log.is_open()) {

        return false; 
    }

    //Log exists, now grab the last line

    //Decrypt the line


    //Check if the time is 1 - the time in the argument map

    //Passed so now encrypt the line and add it to the file


    //Close the file stream
    log.close(); 
    
    //Return the append was successful
    return true;
}



bool batchRunner(int argc, char* argv[]) {
    if (argc != 3) {
        return false;
    }

    regex batchDashMatcher("-B", std::regex_constants::ECMAScript);
    regex batchFileNameMatcher(".*", std::regex_constants::ECMAScript);

    //check for valid flag
    if (regex_match(argv[1], batchDashMatcher)) {
        //Now grab the file name and open it

        //Until end of file, process each command

        //Sanatize the line
        
        //Append to file

        

    } else {
        return false;
    }


    return true;
}



int main(int argc, char* argv[]) {
    map<string, string> sanatizedResult;

    //Inital check of how many arguments
    cout << "Argument count: " << argc << endl;

    if (argc == 3) {
        cout << "Given batch file" << endl;
    } else if (argc == 9 || argc == 11) {
        //Have a full line of arguments
        cout << "Full line of arguments given" << endl;
        
        //Sanatize the input
        bool validSanatized = sanatizeInput(argc, argv, sanatizedResult);

        //Was it able to match formatting?
        if (!validSanatized) {
            //Not correct format, so invalid
            cout << "invalid" << endl;
            return 255;
        }

        //Now that is done, now check if the file exists
        if (appendToLog(sanatizedResult)) {

        } else {
            cout << "invalid" << endl;
            return 255;
        }

    } else {
        //Not enough args or too many (by count)
        cout << "invalid" << endl;
        return 255;
    }


    


    return 0;
}