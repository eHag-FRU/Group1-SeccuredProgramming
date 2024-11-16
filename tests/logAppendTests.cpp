#include <iostream>
#include <string>
#include <map>
#include "../include/inputSanatizer.hpp"
#include <cassert>


using std::cout; using std::endl; using std::string; using std::map;

void testSanatizeTime() {
    //Valid
    char* input1[] = {(string("logappend").c_str()), "-T", "1", "-K", "secret", "-A", "-E", "Fred", "log1"};
    
    //Invalid
    char* input3[] = {"logappend", "-T", "0", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input4[] = {"logappend", "-T", "-1", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input5[] = {"logappend", "-T", "0.1", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input6[] = {"logappend", "-T", "1073741824", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input7[] = {"logappend", "-T", "", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input8[] = {"logappend", "-T", "t", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input9[] = {"logappend", "-T", "-0", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};

    

    //Valid
    char* input8[] = {"logappend", "-T", "1", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input9[] = {"logappend", "-T", "1073741823", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    char* input7[] = {"logappend", "-T", "63", "-K", "secret", "-A", "-E", "Fred", "-R", "001" "log1"};
    //"-T 1 -K secret -A -E Fred log1";
    
    map<string, string> result;

    sanatizeTime(3, input1, result);
    assert(result["-T"] == "1");



    // cout << "Resulting map size: " << result.size() << endl;

    // auto iterator = result.begin(); 

    // while (iterator != result.end()) {
    //     cout << "InputResult: " << iterator->first << ": " << iterator->second << endl;

    //     iterator++;
    // }

    

    
}









int main() {
 testSanatize();

 return 0;
}