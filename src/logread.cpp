#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>
#include <fstream>
#include <utility>
#include <algorithm>
#include <sstream>
#include "inputSanatizer.hpp"
#include "encryptionHandler.hpp"
#include "../include/rTag.hpp"
#include "../include/logread.hpp"
#include "../include/sTag.hpp"


using std::cout; using std::endl; using std::map; using std::string; using std::regex; 
using std::regex_match; using std::iterator; using std::ifstream; using std::pair;
using std::vector; using std::getline; using std::multimap; using std::find;







//Versions for log read
bool sanatizeLogReadToken(int argc, char* argv[], map<string,string>& result) {
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if(regex_match(argv[2], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["K"] = string(argv[4]);
            result.insert(pair<string,string>(argv[1], argv[2]));
        } else {
            // Token was not valid
            //cout << "Token was not valid!" << endl;
            //cout << "Token: |" << argv[1] << "|" << endl;
            return false;
        }

    return true;
}


bool sanatizeLogReadEmployeeOrGuestName(int argc, char* argv[], map<string,string>& result, int tagPostion) {
	// cout << "map to string: " << endl;
	// resultMapToString(result);
	// cout << "map size: " << result.size() << endl;
	
    //Guest OR Employee
    regex guestNameDashMatcher("-G", std::regex_constants::ECMAScript);
    regex employeeNameDashMatcher("-E", std::regex_constants::ECMAScript);

    regex guestEmployeeNameMatcher("([a-z]|[A-Z])*", std::regex_constants::ECMAScript);
	//cout << "tag Position: " << tagPostion << endl;
	//cout << "argv[4]: " << argv[tagPostion] << endl;
    if (regex_match(argv[tagPostion], employeeNameDashMatcher)) {
        //Now we have a valid employee flag
        //Now check the employee name
        if(regex_match( argv[tagPostion + 1], guestEmployeeNameMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["E"] = string(argv[4]);
            result.insert(pair<string,string>(argv[tagPostion], argv[tagPostion + 1]));
        } else {
            // Employee name was not valid
           // cout << "Employee name was not valid!" << endl;
            //cout << "Employee Name: |" << argv[6] << "|" << endl;
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
            //cout << "Guest name was not valid!" << endl;
           // cout << "Guest Name: |" << argv[6] << "|" << endl;
            return false;
        }
    } else {
        //cout << "Guest OR Employee flag was not present!" << endl;
        //Token flag was not found
        return false;
    }

    return true;
}

bool sanatizeLogReadFilePath(int argc, char* argv[], map<string,string>& result) {
   // cout << "In sanatizeFilePath, line 75" << endl;
   // cout << "argc:" << argc << endl;

    //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
    //Then adapted and tweaked to allow for optional path characters
    regex logFileNameMatcher("^(\\\\)*(.+\\\\)*(.+)", std::regex_constants::ECMAScript);
    //cout << regex_match(argv[8], logFileNameMatcher) << endl;

    //Two routes, depending on if the room argument is given
    if (argc == 7) {
        // cout << "In 7 path" << endl;
        //Room argument path
        // cout << argv[6] << endl;
        if (regex_match(argv[6], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert(pair<string,string>("logFile", argv[6]));
        } else {
            //No file name found or invalid
            return false;
        }
    } else if (argc == 5) {
        //cout << "In 5 path" << endl;
        //Non-Room Argument Path
        if (regex_match(argv[4], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert(pair<string,string>("logFile", argv[4]));
        } else {
            //cout << "FAILED LOGFILE PATH CHECK!" << endl;
            //No file name found or invalid
            return false;
        }
    } else {
        //Not the correct number of arguments!
        return false;
    }


    return true;
}

bool sanatizeLogReadInput(int argc, char* argv[], map<string, string>& result) {
    //First check for the flag
    
    //
    //  TOKEN
    //
    regex tokenDashMatcher("-K", std::regex_constants::ECMAScript);
    
   // cout << "argv[1]: " << argv[1] << endl;

    if (regex_match(argv[1], tokenDashMatcher)) {
        //Now we have a valid token flag
        if(!sanatizeLogReadToken(argc, argv, result)) {
            return false;
        }
    } else {
        return false;
    }

    regex dashSTokenMatcher("-S", std::regex_constants::ECMAScript);
    regex dashRTokenMatcher("-R", std::regex_constants::ECMAScript);
    regex dash0TokenMatcher("-0", std::regex_constants::ECMAScript);

    regex dashITokenMatcher("-I", std::regex_constants::ECMAScript);
    regex dashTTokenMatcher("-T", std::regex_constants::ECMAScript);


    //Now determine if the -S or the -R is used
    //The two other are not being implemented,
    //so if token -T or -I is present, then just print
    //Not implemented and exit out
    if (argc == 5) {
        //-S is used
        if(!regex_match(argv[3], dashSTokenMatcher) && !regex_match(argv[3], dash0TokenMatcher)) {
            //Not Valid!!
			//std::cerr << "-S OR -0 token not valid" << endl;
            return false;
        } else {
			//cout << "-S token is valid" << endl;
		}

       //Now add the -S OR -0 token to the result
       result.insert(pair<string,string>(argv[3], argv[3]));

    } else if (argc == 7) {
        //-R is used
        if(!regex_match(argv[3], dashRTokenMatcher)) {
            //Not Valid!!
			//std::cerr << "-R token not valid" << endl;
            return false;
        }
        //Valid, now do the employee/guest check
        //(REQUIRED TO FOLLOW AFTER -R!!!!)

		//Now add the -R token to the result
        result.insert(pair<string,string>(argv[3], argv[3]));
        
        sanatizeLogReadEmployeeOrGuestName(argc, argv, result, 4);


    } else {
        //Check for the -I or -T
        if (regex_match(argv[3], dashITokenMatcher) || regex_match(argv[3], dashTTokenMatcher)) {
            //Not implemented
            //cout << "unimplemented" << endl;
            return false;
        } else if (regex_match(argv[3], dash0TokenMatcher)) {
            result.insert(pair<string,string>(argv[3], argv[3]));
        } else {
            //Invalid token
            return false;
        }
    }

    
    //
    // LOG FILE
    //
    if (!sanatizeLogReadFilePath(argc, argv, result)) {
        return false;
    }

    return true;
}




int main(int argc, char* argv[]) {
    //Need to check the number of arguments
    if (argc != 5 && argc != 7 && argc != 9) {
        cout << "invalid" << endl;
        return 255;
    }

    //Holds the sanatized input
    map<string, string> inputResult;

    //Check if the sanatize was valid
    if(!sanatizeLogReadInput(argc, argv, inputResult)) {
        cout << "invaild" << endl;
        return 255;
    }

    
    //
    //-S OR -R argument
    //

    if (inputResult.find("-S") != inputResult.end()) {
        //Found the -S, now grab the last 2 lines and print them
        //Valid, now open the file
        //cout << "-S command being handled" << endl;

        if(!sTagFunctionality(inputResult, false)) {
            //Failed, so now not valid
            cout << "invalid" << endl;
            return 255; 
        }
    } else if (inputResult.find("-R") != inputResult.end()) {
		//cout << "-R command being handled" << endl;

        rTagFunctionality(inputResult, false);
    } else if (inputResult.find("-0") != inputResult.end()) {
        //Open the file
        ifstream logFile(inputResult["logFile"], std::ios::in | std::ios::binary);

        string currentLine;
        string previousLine;

        while (getline(logFile, currentLine) && !currentLine.empty()) {
            previousLine = currentLine;

            //Decrypt the line
            string decryptedLine = decrypt(previousLine, inputResult["-K"], inputResult);

            if (decryptedLine == "") {
                //cout << "invalid" << endl;
                return 255;
            }

            //cout << decryptedLine << endl;
        }

    }


    return 0;
}