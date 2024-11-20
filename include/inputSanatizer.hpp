#ifndef INPUT_SANITIZER_HPP
#define INPUT_SANITIZER_HPP

#include <map>
#include <string>
#include <vector>


bool sanatizeTime(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeToken(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeEmployeeOrGuestName(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeAriveLeaveTag(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeRoomID(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeFilePath(int argc, char* argv[], std::map<std::string,std::string>& result);

bool sanatizeInput(int argc, char* argv[], std::map<std::string, std::string>& resultingMap);


void resultMapToString(std::map<std::string,std::string>& sanatizedResult);
bool validTimeStamp(std::map<std::string, std::string>& commandLineArguments);
bool validArrivalLeave(std::map<std::string, std::string>& commandLineArguments, std::string name, bool testFlag);
std::vector<std::string> splitStringT(const std::string& str, char delimiter);



#endif