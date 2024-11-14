#ifndef INPUT_SANITIZER_HPP
#define INPUT_SANITIZER_HPP

#include <map>
#include <string>


bool sanatizeTime(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeToken(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeEmployeeOrGuestName(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeAriveLeaveTag(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeRoomID(int argc, char* argv[], std::map<std::string,std::string>& result);
bool sanatizeFilePath(int argc, char* argv[], std::map<std::string,std::string>& result);

bool sanatizeInput(int argc, char* argv[], std::map<std::string, std::string>& resultingMap);




#endif