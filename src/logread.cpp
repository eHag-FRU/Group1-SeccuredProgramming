#include <iostream>
#include <map>
#include <string>
#include <regex>



using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match;

bool sanatizeInput(int argc, char* argv[], map<string,string>& result) {
    //Look at token flag
    regex tokenFlagMatcher("-K", std::regex_constants::ECMAScript);
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    //Validate token
    if (regex_match(argv[1], tokenFlagMatcher)) {
        //Valid flag, now check contents
        if (regex_match(argv[2], tokenMatcher)) {
            //Valid token, now store in the map
            result[argv[1]] = argv[2];
        } else {
            return false;
        }
    }

    //Check size of the argc
    //5, only -S <log> remains
    if (argc == 5) {
        //5, only -S <log> remains
        regex fileFlagMatcher("-S", std::regex_constants::ECMAScript);
        regex fileMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

        //Validate token
        if (regex_match(argv[1], fileFlagMatcher)) {
            //Valid flag, now check contents
            if (regex_match(argv[2], fileMatcher)) {
                //Valid token, now store in the map
                result[argv[1]] = argv[2];
            } else {
                return false;
            }
        } else {
            return false;
        }
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
    if(!sanatizeInput(argc, argv, inputResult)) {
        cout << "invaild" << endl;
        return 255;
    }





    return 0;
}