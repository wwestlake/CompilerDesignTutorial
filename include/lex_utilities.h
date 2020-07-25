#ifndef __LEX_UTILITIES
#define __LEX_UTILITIES

#include <sstream>


template <typename T>
T parse_number(std::string numstr)
{
  std::stringstream ss;
  ss << numstr;
  T result(0);
  ss >> result;
  return result;
}



#endif // __LEX_UTILITIES