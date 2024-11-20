#ifndef INPUT_SANITIZER_HPP
#define INPUT_SANITIZER_HPP

#include <map>
#include <string>

bool sTagFunctionality(std::map<std::string, std::string> input, bool debugMode);
bool sanatizeLogReadToken(int argc, char* argv[], std::map<std::string, std::string>& result);
bool sanatizeLogReadEmployeeOrGuestName(int argc, char* argv[], std::map<std::string, std::string>& result, int tagPostion);
bool sanatizeLogReadFilePath(int argc, char* argv[], std::map<std::string, std::string>& result);
bool sanatizeLogReadInput(int argc, char* argv[], std::map<std::string, std::string>& result);




#endif