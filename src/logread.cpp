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



using std::cout; using std::endl; using std::map; using std::string; using std::regex; 
using std::regex_match; using std::iterator; using std::ifstream; using std::pair;
using std::vector; using std::getline; using std::multimap; using std::find;


bool sTagFunctionality(map<string, string> input, bool debugMode) {
    //Holds the guest and employee names
    vector<string> employees;
    vector<string> guests;
    multimap<int, string> roomOccupency;

    //Open the file
    ifstream log(input["logFile"], std::ios::in | std::ios::binary);

    //Holds the previous two lines
    string currentLine;
    string previousLine;

    while(getline(log, currentLine) && !currentLine.empty()) {
        previousLine = currentLine;

       //decrypt the line
       cout << decrypt(previousLine, input["-K"], input) << endl;
       previousLine = decrypt(previousLine, input["-K"], input);

       //Now tokenize
       vector<string> tokens = splitString(previousLine, ' ');

       //Grab arrival/leave tag
       string arrivalLeaveTag = tokens[1];

        //Grab the room number if present
       string roomNumber;
       if (tokens.size() == 5) {
        //Size allows for room number token
         roomNumber = tokens[2];
       }

       //Grab the guest/employee tag
       string employeeGuestTag;
        if (tokens.size() == 5) {
            //Size allows for room number token
            employeeGuestTag = tokens[3];
       } else {
            employeeGuestTag = tokens[2];
        
       }
       

       //Grab the name
       string name;
       switch(tokens.size()) {
        case 5:
            name = tokens[4];
            break;
        default:
            //This is size of 4
            name = tokens[3];
       }


       //Build employee / Guest list
       if (employeeGuestTag == "E") {
        //Check if its an arrival or leave
        if (arrivalLeaveTag == "A") {
            //Its an arrival, so add in
            employees.push_back(name);


            //Add to the map with roomNumber
            if(!roomNumber.empty()) {
                cout << "Adding to room occupency multimap" << endl;
                //We have a room number
                //Add in the person with their room number they are in as the key
                roomOccupency.insert(pair<int, string>(stoi(roomNumber), name));
            }
        } else if (arrivalLeaveTag == "L") {
            //Its an leave
            //Check if name is in the employees vector
            auto employeeNamePostion = find(employees.begin(), employees.end(), name);
            if (employeeNamePostion != employees.end()) {
                //Found the element, remove it
                employees.erase(employeeNamePostion);
            } else {
                //Could not find employee, LOG NOT VALID!!
                return false;
            }
        }
        
       } else if (employeeGuestTag == "G") {
         //Check if its an arrival or leave
        if (arrivalLeaveTag == "A") {
            //Its an arrival, so add in
            guests.push_back(name);
            //roomOccupency.insert(pair<int, string>(stoi(roomNumber), name));
            //Add to the map with roomNumber
            // if(!roomNumber.empty()) {
            //     //We have a room number
            //     //Add in the person with their room number they are in as the key
            //     roomOccupency.insert(pair<int, string>(stoi(roomNumber), name));
            // }
        } else if (arrivalLeaveTag == "L") {
            //Its an leave
            //Check if name is in the employees vector
            auto guestNamePostion = find(guests.begin(), guests.end(), name);
            //Can only remove if leaving the gallery fully and NOT JUST A ROOM!!!!
            if (guestNamePostion != guests.end()) {
                //Found the element, remove it
                guests.erase(guestNamePostion);
            } else {
                //Could not find employee, LOG NOT VALID!!
                return false;
            }
        }
       }
    }

    //print out each of the lines
    //Employees
    for (int i = 0; i < employees.size(); ++i) {
        cout << employees[i];

        //Determine if the i is the last one
        if (i < employees.size() - 1) {
            cout << ", ";
        }
    }

    cout << "\n";

    //Guests
    for (int i = 0; i < guests.size(); ++i) {
        cout << guests[i];

        //Determine if the i is the last one
        if (i < guests.size() - 1) {
            cout << ", ";
        }
    }

    cout << "\n";

    //Room
    for (auto roomNumber = roomOccupency.begin(); roomNumber != roomOccupency.end(); ++roomNumber) {
        //Now go through the values (people in it) of that key (room number)
         cout << roomNumber->first << ": ";
        for(auto person = roomOccupency.begin(); person != roomOccupency.end(); ++person) {
            if (person->first == roomNumber->first) {
                cout << person->second << " ";
            }
        }
    }

    //Was successful
    return true;

}

//Versions for log read
bool sanatizeLogReadToken(int argc, char* argv[], map<string,string>& result) {
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    if(regex_match(argv[2], tokenMatcher)) {
            //Now sanitized and safe, now put in the map
            //result["K"] = string(argv[4]);
            result.insert(pair<string,string>(argv[1], argv[2]));
        } else {
            // Token was not valid
            cout << "Token was not valid!" << endl;
            cout << "Token: |" << argv[1] << "|" << endl;
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
            cout << "Employee name was not valid!" << endl;
            cout << "Employee Name: |" << argv[6] << "|" << endl;
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
        cout << "In 5 path" << endl;
        //Non-Room Argument Path
        if (regex_match(argv[4], logFileNameMatcher)) {
            //Matches, now put in map
             result.insert(pair<string,string>("logFile", argv[4]));
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
			std::cerr << "-S token not valid" << endl;
            return false;
        } else {
			cout << "-S token is valid" << endl;
		}

       //Now add the -S token to the result
       result.insert(pair<string,string>(argv[3], argv[3]));

    } else if (argc == 7) {
        //-R is used
        if(!regex_match(argv[3], dashRTokenMatcher)) {
            //Not Valid!!
			std::cerr << "-R token not valid" << endl;
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
    if (!sanatizeLogReadFilePath(argc, argv, result)) {
        return false;
    }

    return true;
}

// vector<string> splitString(const string& str, char delimiter) {
//     vector<string> tokens;
//     std::istringstream stream(str);
//     string token;
    
//     while (getline(stream, token, delimiter)) {
//         tokens.push_back(token);
//     }

//     return tokens;
// }




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

    sanatizeLogReadInput(argc, argv, inputResult);


    //resultMapToString(inputResult);

    //Now can try to open the file
    
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
    }


    return 0;
}