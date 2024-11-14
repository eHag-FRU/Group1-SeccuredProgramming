#ifndef LOG_APPEND_HPP
#define LOG_APPEND_HPP

#include<map>
#include<string>
#include<fstream>
#include<vector>

void resultMapToString(std::map<std::string,std::string>& sanatizedResult);
bool validTimeStamp(std::map<std::string, std::string>& commandLineArguments, std::fstream& logFile);
bool validArrivalLeave(std::map<std::string, std::string>& commandLineArguments, std::fstream& logFile, std::string name);
std::vector<std::string> splitStringT(const std::string& str, char delimiter);

bool commandExecuter(int argc, char* argv[], std::map<std::string, std::string>& sanatizedResult);

#endif