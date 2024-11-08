#include "logappend.hpp"
#include <iostream>
#include <regex>
#include <fstream>

using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;
using std::fstream;


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

    return true;
}

bool sanatizeAriveLeaveTag(int argc, char* argv[], map<string,string>& result, int tagPositon) {

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
    //regex logFileNameMatcher("[^\\](.+)$", std::regex_constants::ECMAScript);

    //Two routes, depending on if the room argument is given
    if (argc == 11) {
        cout << "In 11 path" << endl;
        //Room argument path
        // cout << argv[10] << endl;
        // if (regex_match(argv[10], logFileNameMatcher)) {
        //     //Matches, now put in map
        //      result.insert({"logFile", argv[10]});
        // } else {
        //     //No file name found or invalid
        //     return false;
        // }
    } else if (argc == 9) {
        //Non-Room Argument Path
        //Room argument path
        // if (regex_match(argv[8], logFileNameMatcher)) {
        //     //Matches, now put in map
        //      result.insert({"logFile", argv[8]});
        // } else {
        //     //No file name found or invalid
        //     return false;
        // }
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


int main(int argc, char* argv[]) {
    map<string, string> sanatizedResult;

    //Inital check of how many arguments
    cout << "Argument count: " << argc << endl;

    //Check the count, determine if batch or if line is full command
    if (argc == 3) {
        cout << "Given batch file" << endl;
    } else if (argc == 9 || argc == 11) {
        //Have a full line of arguments
        cout << "Full line of arguments given" << endl;

        //Now send off to sanatize the full command
        //Returns T/F => T = Successful / F = Invalid
        if (!sanatizeInput(argc, argv, sanatizedResult)) {
            cout << "invalid" << endl;
            return 255;
        }

        
        //Print out the resulting map
        resultMapToString(sanatizedResult);

    } else {
        cout << "invalid" << endl;
        return 255;
    }


    //Now sanatized!
    //Now


    return 0;
}