#include <iostream>
#include <string>
#include <map>
#include "../include/logappend.hpp"
#include <cassert>


using std::cout; using std::endl; using std::string; using std::map;

void testSanatizeTime() {
    char* input1[] = {"logappend", "-T", "1"};
    //"-T 1 -K secret -A -E Fred log1";
    
    map<string, string> result;

    sanatizeTime(3, input1, result);



    cout << "Resulting map size: " << result.size() << endl;

    auto iterator = result.begin();

    while (iterator != result.end()) {
        cout << "InputResult: " << iterator->first << ": " << iterator->second << endl;

        iterator++;
    }

    assert(result["-T"] == "1");

    
}









int main() {
 testSanatize();

 return 0;
}