#ifndef ENCRYPTION_HANDLER_HPP
#define ENCRYPTION_HANDLER_HPP

#include <string> 
#include <map>

std::string encrypt(const std::string& message, const std::string& key, std::map<std::string, std::string>& input);
std::string decrypt(const std::string& encryptedMessage, const std::string& key, std::map<std::string, std::string>& input);

#endif