
#include "tag.hpp"

namespace arachnid {

  Tag::Tag(std::ostream& os,std::string name) : os(os),name(name)
  {
    os << "<" << name << ">\n";
  }

  Tag::~Tag(){
    os << "</" << name << ">\n";
  }

}


