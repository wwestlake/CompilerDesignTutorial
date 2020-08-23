
#include <cstdarg>
#include "utils.h"

std::string getString(std::vector<char>& buffer)
{
    std::stringstream ss;
    for (auto ch : buffer)
    {
        ss << ch;
    }
    return ss.str();
}


void ClearBuffer(std::vector<char>& buf)
{
    buf.clear();
}

void AppendBufferChar(std::vector<char>& buf, char ch)
{
    buf.push_back(ch);
}

void AppendBufferString(std::vector<char>& buf, char* str)
{
    std::string thestr(str);
    for (auto ch : thestr)
    {
        AppendBufferChar(buf, ch);
    }
}

std::string* error(const char* fmt...)
{
    std::stringstream ss;
    va_list args;
    va_start(args, fmt);
    char buffer[256];
    std::vsnprintf(buffer, 250, fmt, args);
    ss << buffer;

    va_end(args);
    return new std::string( ss.str() );
}


int parse_number_hob(std::string num_str)
{
    std::string input;
    int base = 10;

    if (num_str.length() > 2)
    {
        switch (num_str[1])
        {
            case 'x': input = num_str.substr(2); base = 16; break;
            case 'o': input = num_str.substr(2); base = 8; break;
            case 'b': input = num_str.substr(2); base = 2; break;
            default: input = num_str;
        }
    }
    return stoi(input, 0, base);
}

bool parse_bool(std::string bool_val)
{
    if (bool_val == "true") return true;
    return false;
}
