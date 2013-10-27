
#include "uri_match.hpp"

#include "str_utils.hpp"
#include <string>
#include <iostream>

namespace arachnid {

  bool Specified_URI::matches (const std::string& uri) {
    return starts_with(uri, name);
  }

}

