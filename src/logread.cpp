#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <iterator>



using std::cout; using std::endl; using std::map; using std::string; using std::regex; using std::regex_match; using std::iterator;

bool sanatizeInput(int argc, char* argv[], map<string,string>& result) {
    cout << "Argc count: " << argc << endl;

    //Look at token flag
    regex tokenFlagMatcher("-K", std::regex_constants::ECMAScript);
    regex tokenMatcher("([a-z]|[A-Z]|[0-9])*", std::regex_constants::ECMAScript);

    //Validate token
    if (regex_match(argv[1], tokenFlagMatcher)) {
        //Valid flag, now check contents
        if (regex_match(argv[2], tokenMatcher)) {
            //Valid token, now store in the map
            //result[argv[1]] = argv[2];
            result.insert({argv[1], argv[2]});
            cout << result["-K"] << endl;
        } else {
            cout << "Invalid token" << endl;
            return false;
        }
    } else {
        cout << "Invalid token flag" << endl;
        return false;
    }

    //Check size of the argc
    //5, only -S <log> remains
    if (argc == 5) {
        //5, only -S <log> remains
        cout << " only -S <log> remains" << endl;
        cout << argv[3] << endl;
        //regex fileFlagMatcher("-S", std::regex_constants::ECMAScript);
        //regex logFileNameMatcher("[^\\](.+)$", std::regex_constants::ECMAScript);

        cout << "Passed regex patterns" << endl;


        //Validate token
        // if (regex_match(argv[3], fileFlagMatcher)) {
        //     cout << "-S matched!!!!!!" << endl;

        //     //Valid flag, now check contents
        //     // if (regex_match(argv[2], logFileNameMatcher)) {
        //     //     //Valid token, now store in the map
        //     //     result[argv[1]] = argv[2];
        //     //     cout << result["-S"] << endl;
        //     // } else {
        //     // cout << "Invalid file path" << endl;

        //     //     return false;
        //     // }
        // } else {
        //     cout << "Invalid file flag" << endl;
        //     return false;
        // }
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
    // if(!sanatizeInput(argc, argv, inputResult)) {
    //     cout << "invaild" << endl;
    //     return 255;
    // }

    sanatizeInput(argc, argv, inputResult);

    cout << "Resulting map size: " << inputResult.size() << endl;

    auto iterator = inputResult.begin();

    while (iterator != inputResult.end()) {
        cout << "InputResult: " << iterator->first << ": " << iterator->second << endl;

        iterator++;
    }



    return 0;
}