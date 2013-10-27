
#ifndef _MIME_HPP_1364926169_
#define _MIME_HPP_1364926169_

#include <string>

namespace arachnid {

  // getMime(".../thing.pdf") => "application/pdf"
  // getMime(".../thing.html") => "text/html"

  std::string getMIME ( std::string fileName );

}

#endif
