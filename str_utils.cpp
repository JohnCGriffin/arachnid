
#include "str_utils.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>

namespace arachnid {

  std::string upper_case (constStrRef original)
  {
    std::string result;
    std::transform (original.begin(), original.end(), std::back_inserter(result), ::toupper);
    return result;
  }

  std::string lower_case (constStrRef original)
  {
    std::string result;
    std::transform (original.begin(), original.end(), std::back_inserter(result), ::tolower);
    return result;
  }

  std::string replace (constStrRef original, constStrRef query, constStrRef replacement)
  {
    std::string result = original;

    size_t pos = 0;
    size_t qlen = query.length();
    size_t rlen = replacement.length();

    while((pos = result.find(query,pos)) != std::string::npos){
      result.replace(pos, qlen, replacement);
      pos += rlen;
    }
    return result;
  }

  std::string before (constStrRef original, char to_find)
  {
    size_t pos = original.find(to_find);
    return pos == std::string::npos ? original : original.substr(0,pos);
  }

  std::string after (constStrRef original, char to_find)
  {
    size_t pos = original.find(to_find);
    return pos == std::string::npos ? original : original.substr(pos+1);
  }

  std::string after_last (constStrRef original, char to_find)
  {
    size_t pos = original.rfind(to_find);
    return pos == std::string::npos ? original : original.substr(pos+1);
  }

  bool ends_with ( constStrRef original, constStrRef ending )
  {
    return ending.size() <= original.size() 
      && ending == original.substr(original.size() - ending.size());
  }

  bool starts_with ( constStrRef original, constStrRef prefix )
  {
    return prefix.size() <= original.size() 
      && prefix == original.substr(0,prefix.size());
  }

  std::string trim ( constStrRef original )
  {
    if(original.length() == 0){
      return original;
    }

    int first = 0;
    int last = original.length();

    while(first < last && ::isspace(original[first])){
      first++;
    }
    while(last > first && ::isspace(original[last-1])){
      last--;
    }
    return original.substr(first,last-first);
  }

}

