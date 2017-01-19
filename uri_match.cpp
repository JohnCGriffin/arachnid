
#include "uri_match.hpp"

#include "str_utils.hpp"
#include <string>
#include <iostream>

namespace arachnid {

  Specified_URI::Specified_URI (std::string name) : name(name){} 

  bool Specified_URI::matches (const std::string& uri) const {
    return starts_with(uri, name);
  }

}

