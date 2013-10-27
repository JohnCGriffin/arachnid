
/////////////// Justification for Matcher class ///////////////
//
// URIMatcher is only utilized so far with a substring match in
// Specified_URI, but one could easily expect a regular
// expression, for instance connecting to an interpreter based
// upon file extension.
//
///////////////////////////////////////////////////////////////

#ifndef _URI_MATCH_HPP_1364926169_
#define _URI_MATCH_HPP_1364926169_

#include <string>

namespace arachnid {

  class URIMatcher {
  public:
    virtual bool matches (const std::string &uri) = 0;
    virtual ~URIMatcher(){}
  };

  class Specified_URI : public URIMatcher {
    std::string name;
  public:
    Specified_URI (std::string name) : name(name){} 
    bool matches (const std::string& uri);
  };

}

#endif
