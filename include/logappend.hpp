#ifndef LOG_APPEND_HPP
#define LOG_APPEND_HPP

#include <map>
#include <string>
#include <fstream>
#include <vector>
using std::string; using std::map; using std::fstream;

bool sanatizeTime(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeToken(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeEmployeeOrGuestName(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeAriveLeaveTag(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeRoomID(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeFilePath(int argc, char* argv[], std::map<std::string,std::string>& result);
bool validArrivalLeave(map<string, string>& commandLineArguments, fstream& logFile, string name);
std::vector<string> splitString(const string& str, char delimiter);
void resultMapToString(map<string,string>& sanatizedResult);
bool sanatizeInput(int argc, char* argv[], std::map<std::string, std::string>& resultingMap);




#endif