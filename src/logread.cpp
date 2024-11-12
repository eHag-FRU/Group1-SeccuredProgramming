#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>



using std::cout; using std::endl; using std::map; using std::string; using std::regex; 
using std::regex_match; using std::iterator; using std::ifstream;


bool sanatizeToken(int argc, char* argv[], map<string,string>& result) {
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if(regex_match(argv[2], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["K"] = string(argv[4]);
            result.insert({argv[1], argv[2]});
        } else {
            // Token was not valid
            cout << "Token was not valid!" << endl;
            cout << "Token: |" << argv[1] << "|" << endl;
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

bool sanatizeFilePath(int argc, char* argv[], map<string,string>& result) {
    cout << "In sanatizeFilePath, line 75" << endl;
    cout << "argc:" << argc << endl;

    //Pattern pulled from: https://stackoverflow.com/questions/9363145/regex-for-extracting-filename-from-path
    //Then adapted and tweaked to allow for optional path characters
    regex logFileNameMatcher("^(\\\\)*(.+\\\\)*(.+)", std::regex_constants::ECMAScript);
    //cout << regex_match(argv[8], logFileNameMatcher) << endl;

    //Two routes, depending on if the room argument is given
    if (argc == 7) {
        cout << "In 7 path" << endl;
        //Room argument path
        cout << argv[6] << endl;
        if (regex_match(argv[6], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert({"logFile", argv[6]});
        } else {
            //No file name found or invalid
            return false;
        }
    } else if (argc == 5) {
        cout << "In 5 path" << endl;
        //Non-Room Argument Path
        if (regex_match(argv[4], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert({"logFile", argv[4]});
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
    //First check for the flag
    
    //
    //  TOKEN
    //
    regex tokenDashMatcher("-K", std::regex_constants::ECMAScript);
    
    cout << "argv[1]: " << argv[1] << endl;

    if (regex_match(argv[1], tokenDashMatcher)) {
        //Now we have a valid token flag
        if(!sanatizeToken(argc, argv, result)) {
            return false;
        }
    } else {
        return false;
    }

    regex dashSTokenMatcher("-S", std::regex_constants::ECMAScript);
    regex dashRTokenMatcher("-R", std::regex_constants::ECMAScript);

    regex dashITokenMatcher("-I", std::regex_constants::ECMAScript);
    regex dashTTokenMatcher("-T", std::regex_constants::ECMAScript);


    //Now determine if the -S or the -R is used
    //The two other are not being implemented,
    //so if token -T or -I is present, then just print
    //Not implemented and exit out
    if (argc == 5) {
        //-S is used
        if(!regex_match(argv[3], dashSTokenMatcher)) {
            //Not Valid!!
            return false;
        }
    } else if (argc == 7) {
        //-R is used
        if(!regex_match(argv[3], dashRTokenMatcher)) {
            //Not Valid!!
            return false;
        }

        //Valid, now do the employee/guest check
        //(REQUIRED TO FOLLOW AFTER -R!!!!)


    } else {
        //Check for the -I or -T
        if (regex_match(argv[3], dashITokenMatcher) || regex_match(argv[3], dashTTokenMatcher)) {
            //Not implemented
            cout << "unimplemented" << endl;
            return false;
        } else {
            //Invalid token
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

int main(int argc, char* argv[]) {
    //Need to check the number of arguments
    if (argc != 5 && argc != 7 && argc != 9) {
        cout << "invalid" << endl;
        return 255;
    }

    //Holds the sanatized input
    map<string, string> inputResult;

    //Check if the sanatize was valid
    // if(!sanatizeInput(argc, argv, inputResult)) {
    //     cout << "invaild" << endl;
    //     return 255;
    // }

    sanatizeInput(argc, argv, inputResult);


    resultMapToString(inputResult);

    //Now can try to open the file



    return 0;
}