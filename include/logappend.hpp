#ifndef LOG_APPEND_HPP
#define LOG_APPEND_HPP

#include <map>
#include <string>

bool sanatizeInput(int argc, char* argv[], std::map<std::string, std::string>& resultingMap);


int main();



#endif