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

	cout << "======EXECUTING COMMAND=======" << endl;

    // //Now send off to sanatize the full command
    // //Returns T/F => T = Successful / F = Invalid
    if (!sanatizeInput(argc, argv, sanatizedResult)) {
		std::cerr << "fails in sanatizeInput in commandExecutor" << endl;
        return false;
    }

    
    //Print out the resulting map
    //resultMapToString(sanatizedResult);

    //Now open the map
    fstream log;

    //log.open(sanatizedResult["logFile"], std::ios::app | std::ios::binary);

    
    //validTimeStamp(sanatizedResult, log);
    


    //Now open, now check if the timestamp is valid
    if (!validTimeStamp(sanatizedResult)) {
		std::cerr << "Invalid time stamp" << endl;
        return false;
    }

    cout << "Valid time stamp" << endl;
   // cout << "NOW VALIDATING ARRIVAL & LEAVE" << endl;

    //Grab the name
    string name;
    if (sanatizedResult.find("-G") != sanatizedResult.end()) {
        //Guest name
        name = sanatizedResult["-G"];
    } else {    //Employee
        name = sanatizedResult["-E"];
    }

    

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

	//Now check for valid arrival/leave details
    if(!validArrivalLeave(sanatizedResult, name, false)) {
        cout << "VALID ARRIVAL/LEAVE FAILED!" << endl;
        return false; 
    }

    //Add an endl to the file

    cout << "Line before encryption: " << logLine << endl;
    //cout << "Key: " << sanatizedResult["-K"] << endl;

    string encryptedLine;
    string decryptedLine;

    //Encrypt with the key
    encryptedLine = encrypt(logLine, sanatizedResult["-K"], sanatizedResult);

    if (encryptedLine == "") {
        return false;
    }

   // cout << "ENCYRPTED LINE (329 logappend.cpp): " << encryptedLine << endl;

    decryptedLine = decrypt(encryptedLine, sanatizedResult["-K"], sanatizedResult);

    if (decryptedLine == "") {
        return false;
    }

   // cout << "DECRYPTED LINE (335 logappend.cpp): " << decryptedLine << endl;

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
   //cout << "Argument count: " << argc << endl;

    //Check the count, determine if batch or if line is full command
    if (argc == 3) {
        cout << "Given batch file" << endl;

        //Batch file file stream
        fstream batchFile;

		//cout << "Batch file name: " << argv[2] << endl;
        //Open the batch file
		batchFile.open(argv[2], ios::in | ios::binary);


        if(!batchFile.is_open()) {
            //File could not be opened
            cout << "batch file could not open" << endl;
            return 255;
        }

        //Grabs each line of input and executes it
        string currentLine;
		string personStatus;
        int i = 0;
        while(getline(batchFile, currentLine)) {
			
            //Split into a vector of strings
			vector<string> lineVec;
			lineVec = splitStringT(currentLine, ' ');

			// Add "logappend" to the first position in the vector
    		lineVec.insert(lineVec.begin(), "logappend");

			// Iterate through lineVec and print each element
			//cout << "Processing line: " << currentLine << endl;
			// for (const auto& element : lineVec) {
			// 	cout << "Element: " << element << endl;
			// }

			// Convert vector<string> to vector<char*>
			vector<char*> argvVec;
			for (const auto& s : lineVec) {
				char* cstr = new char[s.size() + 1];
				strncpy(cstr, s.c_str(), s.size());
				cstr[s.size()] = '\0'; // Null-terminate the string
				argvVec.push_back(cstr);
			}

			// Convert vector<char*> to array of char*
			char** argvArray = new char*[argvVec.size() + 1];
			for (size_t i = 0; i < argvVec.size(); ++i) {
				argvArray[i] = argvVec[i];
			}
			argvArray[argvVec.size()] = nullptr; // Null-terminate the array

            //Grab count
			int count = argvVec.size();
			map<string, string> cmdLine;

			if (count == 9) {
				// determine if person is arriving or leaving so we can set the appropriate value to the map
				if (lineVec[4] == "-A") {
					personStatus = "A";
				} else {
					personStatus = "L";
				}

				// insert values into map
				cmdLine["programName"] = "logappend";
				cmdLine[lineVec[1]] = lineVec[2];
				cmdLine[lineVec[3]] = lineVec[4];
				cmdLine[lineVec[5]] = personStatus;
				cmdLine[lineVec[6]] = lineVec[7];
				cmdLine["logFile"] = lineVec[8];
			} else if (count == 11) {
				// determine if person is arriving or leaving so we can set the appropriate value to the map
				if (lineVec[4] == "-A") {
					personStatus = "A";
				} else {
					personStatus = "L";
				}

				// insert values into map
				cmdLine["programName"] = "logappend";
				cmdLine[lineVec[0]] = lineVec[1];
				cmdLine[lineVec[2]] = lineVec[3];
				cmdLine[lineVec[4]] = personStatus;
				cmdLine[lineVec[5]] = lineVec[6];
				cmdLine[lineVec[7]] = lineVec[8];
				cmdLine["logFile"] = lineVec[10];        
        	} else {
				std::cerr << "Invalid number of arguments in batch file" << endl;
				return -1;
			}

			//cout << "Command Line: " << endl;
			//resultMapToString(cmdLine);

			//cout << "Count: " << count << endl;

			//Parsed, to send to execute each line
			if (!commandExecuter(count, argvArray, cmdLine)) {
				cout << "invalid input on line 251" << endl;
				return 255;
			}

			 // Clean up dynamically allocated memory
			for (char* ptr : argvVec) {
				delete[] ptr;
			}
			delete[] argvArray;
		}

		//Close the file
		batchFile.close();
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