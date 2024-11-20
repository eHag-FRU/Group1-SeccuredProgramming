#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>
#include <fstream>
#include <utility>
#include <algorithm>
#include <sstream>
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