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
    map<int, vector<string> > roomOccupency;

    //Open the file
    ifstream log(input["logFile"], std::ios::in | std::ios::binary);

    //Holds the previous two lines
    string currentLine;
    string previousLine;

    while(getline(log, currentLine) && !currentLine.empty()) {
        previousLine = currentLine;

       //decrypt the line
       //cout << decrypt(previousLine, input["-K"], input) << endl;
       previousLine = decrypt(previousLine, input["-K"], input);

       //Error check
       if (previousLine == "") {
        return false;
       }

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
        //cout << "Its an employee" << endl;
        if (arrivalLeaveTag == "A") {
            //cout << "Its an arrival" << endl;
            //Its an arrival, so add in
            //Only do on actual gallery arrivals
            if (roomNumber.empty()) {
                employees.push_back(name);
            }
            
            
            //cout << "Room number: " << roomNumber << endl;
            //cout << "Room number empty?: " << roomNumber.empty() << endl;


            //Check if the room number vector is already there
            if (!roomNumber.empty() && roomOccupency.find(stoi(roomNumber)) == roomOccupency.end()) {
                //Doesn't exist, create it
                int roomNumberKey = stoi(roomNumber);
                
                vector<string> peopleInRoom = {name};

                //Now insert into map
                roomOccupency.insert(pair<int, vector<string> >(roomNumberKey, peopleInRoom));
            } else if (!roomNumber.empty()) {
                //Exists, just tack onto the vector
                roomOccupency[stoi(roomNumber)].push_back(name);
            }

        } else if (arrivalLeaveTag == "L") {
            //Its an leave
            //cout << "Its an leave" << endl;
            //Its an leave, check if the room number is empty
            //if so, just remove from the employee vector
            if (roomNumber.empty()) {
                //Just remove the name from the vector
                auto namePosition = find(employees.begin(), employees.end(), name);
                if ( namePosition != employees.end()) {
                    //Found the name
                    employees.erase(namePosition);
                }
            } else if (!roomNumber.empty()) {
                //Leaving a room
                
                //Grab the names position in the room numbers vector
                auto nameLocation = find(roomOccupency[stoi(roomNumber)].begin(),
                    roomOccupency[stoi(roomNumber)].end(), name);

                if (nameLocation != roomOccupency[stoi(roomNumber)].end()) {
                    //Now remove it
                    roomOccupency[stoi(roomNumber)].erase(nameLocation);
                } else {
                    //Could not find the name, so integ violation
                    return false;
                }
            }

            
        } else {
                //Could not find employee, LOG NOT VALID!!
                return false;
            }
        } else if (employeeGuestTag == "G") {            
            //Check if its an arrival or leave
            if (arrivalLeaveTag == "A") {
                //cout << "Its an arrival" << endl;
                //Its an arrival, so add in
                //Only do on actual gallery arrivals
                if (roomNumber.empty()) {
                    guests.push_back(name);
                }
                
                
                //cout << "Room number: " << roomNumber << endl;
                //cout << "Room number empty?: " << roomNumber.empty() << endl;


                //Check if the room number vector is already there
                if (!roomNumber.empty() && roomOccupency.find(stoi(roomNumber)) == roomOccupency.end()) {
                    //Doesn't exist, create it
                    int roomNumberKey = stoi(roomNumber);
                    
                    vector<string> peopleInRoom = {name};

                    //Now insert into map
                    roomOccupency.insert(pair<int, vector<string>>(roomNumberKey, peopleInRoom));
                } else if (!roomNumber.empty()) {
                    //Exists, just tack onto the vector
                    roomOccupency[stoi(roomNumber)].push_back(name);
                }
            }else if (arrivalLeaveTag == "L") {
                //Its an leave
                //cout << "Its an leave" << endl;
                //Its an leave, check if the room number is empty
                //if so, just remove from the employee vector
                if (roomNumber.empty()) {
                    //Just remove the name from the vector
                    auto namePosition = find(employees.begin(), employees.end(), name);
                    if ( namePosition != employees.end()) {
                        //Found the name
                        employees.erase(namePosition);
                    }
                } else if (!roomNumber.empty()) {
                    //Leaving a room
                    
                    //Grab the names position in the room numbers vector
                    auto nameLocation = find(roomOccupency[stoi(roomNumber)].begin(),
                        roomOccupency[stoi(roomNumber)].end(), name);

                    if (nameLocation != roomOccupency[stoi(roomNumber)].end()) {
                        //Now remove it
                        roomOccupency[stoi(roomNumber)].erase(nameLocation);
                    } else {
                        //Could not find the name, so integ violation
                        return false;
                    }
                }
            }
            else {
                //Could not find guest, LOG NOT VALID!!
                return false;
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
    for(auto itr = roomOccupency.begin(); itr != roomOccupency.end(); ++itr) {
        //Print out the key as that is the room number
        cout << itr->first << ": ";

        //Check if the room is empty, if so, skip it, dont print anything
        if (itr->second.size() == 0) {
            continue;
        } else {
            //Now go through the names in the vector of guests
            for (int personIndex = 0; personIndex < itr->second.size(); ++personIndex) {
                cout << itr->second[personIndex];

                if (personIndex == itr->second.size() - 1) {
                    //Last element, dont print the comma
                    cout << endl;
                    break;
                } else {
                    cout << ", ";
                }

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
			std::cerr << "-S OR -0 token not valid" << endl;
            return false;
        } else {
			cout << "-S token is valid" << endl;
		}

       //Now add the -S OR -0 token to the result
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
    if(!sanatizeLogReadInput(argc, argv, inputResult)) {
        cout << "invaild" << endl;
        return 255;
    }

   


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
                cout << "invalid" << endl;
                return 255;
            }

            cout << decryptedLine << endl;
        }

    }


    return 0;
}