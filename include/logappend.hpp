#ifndef LOG_APPEND_HPP
#define LOG_APPEND_HPP

void resultMapToString(map<string,string>& sanatizedResult);
bool validTimeStamp(map<string, string>& commandLineArguments, fstream& logFile);
bool validArrivalLeave(map<string, string>& commandLineArguments, fstream& logFile);

bool commandExecuter(int argc, char* argv[], map<string, string>& sanatizedResult);

#endif