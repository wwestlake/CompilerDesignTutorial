#ifndef __UTIL_H
#define __UTIL_H

#include <string>
#include <vector>
#include <sstream>

std::string getString(std::vector<char>& buffer);
void ClearBuffer(std::vector<char>& buffer);
void AppendBufferChar(std::vector<char>& buffer, char ch);
void AppendBufferString(std::vector<char>& buf, char* str);
std::string* error(const char* fmt...);

template <typename T>
T parse_number(std::string str)
{
    std::stringstream ss;
    ss << str;
    T result(0);
    ss >> result;
    return result; 
}

int parse_number_hob(std::string num_str);
bool parse_bool(std::string bool_val);



#endif // __UTIL_H