#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>
#include "inputSanatizer.hpp"



using std::cout; using std::endl; using std::map; using std::string; using std::regex; 
using std::regex_match; using std::iterator; using std::ifstream;



bool sanatizeLogReadInput(int argc, char* argv[], map<string, string>& result) {
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

    sanatizeLogReadInput(argc, argv, inputResult);


    resultMapToString(inputResult);

    //Now can try to open the file



    return 0;
}