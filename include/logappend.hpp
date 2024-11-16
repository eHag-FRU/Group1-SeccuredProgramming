#ifndef LOG_APPEND_HPP
#define LOG_APPEND_HPP

#include<map>
#include<string>
#include<fstream>
#include<vector>



bool commandExecuter(int argc, char* argv[], std::map<std::string, std::string>& sanatizedResult);

#endif