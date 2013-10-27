
////////   Common string manipulations ////////
//
// toStr(123) -> "123"
//
// replace("Fred","red","rank") -> "Frank"
//
// upper_case ("Fred") -> "FRED"
// lower_case ("Fred") -> "fred"
//
// trim (" sloppy text ") -> "sloppy text"
//
// before ("William M. Smith",' ') -> "William"
// after ("William M. Smith",' ') -> "M. Smith"
// after_last ("William M. Smith",' ') -> "Smith"
//
// ends_with ("myfile.cpp", ".cpp") -> true
// 
 
#ifndef _STRING_UTILS_HPP_1364926169_
#define _STRING_UTILS_HPP_1364926169_

#include <string>
#include <sstream>
#include <cstring>

namespace arachnid {

  typedef const std::string& constStrRef;

  std::string replace (constStrRef original, constStrRef query, constStrRef replacement);

  std::string upper_case (constStrRef original);
  std::string lower_case (constStrRef original);

  std::string before (constStrRef original, char to_find);
  std::string after (constStrRef original, char to_find);
  std::string after_last (constStrRef original, char to_find);

  std::string trim (constStrRef original);

  bool ends_with ( constStrRef, constStrRef ending );
  bool starts_with ( constStrRef, constStrRef ending );

}

#endif

