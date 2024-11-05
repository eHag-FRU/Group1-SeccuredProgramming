#include <iostream>
#include <regex>
#include <string>
#include <map>



using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;

bool sanatizeInput(int argc, char* argv[], map<string, string>& resultingMap) {
    cout << ("Sanatizing the input!!!!") << endl;

    ///Check for the correct argument count again
    if (argc != 9 && argc != 11) {
        return false;
    }

    //Now make the regex matchers
    
    //TimeStamp
    regex timeStampDashMatcher("-T", std::regex_constants::ECMAScript);
    regex timeStampMatcher (".*", std::regex_constants::ECMAScript);

    //Run the matcher and check
    if(regex_match(argv[1], timeStampDashMatcher)) {
        //Now we have a valid time flag
        //Now check the time stamp
        if (regex_match(argv[2], timeStampMatcher)) {
            //Now sanitized and safe, now put in the map
            resultingMap["T"] = string(argv[2]);
        } else {
            cout << "Timestamp was invalid!" << endl;
            //The timestamp did not match format
            return false;
        }
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

    //Guest OR Employee
    regex guestNameDashMatcher("-G", std::regex_constants::ECMAScript);
    regex employeeNameDashMatcher("-E", std::regex_constants::ECMAScript);

    regex guestEmployeeNameMatcher("([a-z]|[A-Z])*", std::regex_constants::ECMAScript);

    if (regex_match(argv[5], employeeNameDashMatcher)) {
        //Now we have a valid employee flag
        //Now check the employee name
        if(regex_match( argv[6], tokenMatcher)) {
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
        if(regex_match( argv[6], tokenMatcher)) {
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

    //Arrival or leave

    
    //Room ID (OPTIONAL ARGUMENT)
    if (argc == 11) {
        //Then there is an actual room id
    }
  

    //Logfile name

    
   


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

    } else {
        //Not enough args or too many (by count)
        cout << "invalid" << endl;
        return 255;
    }


    


    return 0;
}