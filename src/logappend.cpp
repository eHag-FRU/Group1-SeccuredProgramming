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
        //Check if with in valid range
        if (timeStamp >= 1 && timeStamp <= 1073741823) {
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


    return true;
}


bool sanatizeEmployeeOrGuestName(int argc, char* argv[], map<string,string>& result) {

    return true;
}

bool sanatizeAriveLeaveTag(int argc, char* argv[], map<string,string>& result) {

    return true;
}

bool sanatizeRoomID(int argc, char* argv[], map<string,string>& result) {

    return true;
}

bool sanatizeFilePath(int argc, char* argv[], map<string,string>& result) {

    return true;
}


bool sanatizeInput(int argc, char* argv[], map<string, string>& result) {
    cout << ("Sanatizing the input!!!!") << endl;

    ///Check for the correct argument count again
    if (argc != 9 || argc != 11) {
        return false;
    }

    //
    //TIME
    //

    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);

    //Run the matcher and check
    if(regex_match(argv[1], timeStampDashMatcher)) {
        //Good time flag found, now pull in the time
        sanatizeTime(argc, argv, result);
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
        sanatizeToken(argc, argv, result);
    } else {
        return false;
    }




    return true;
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
        if (sanatizeInput(argc, argv, sanatizedResult)) {
            cout << "invalid" << endl;
            return 255;
        }
    } else {
        cout << "invalid" << endl;
        return 255;
    }


    return 0;
}