
#ifndef _HTTP_UTILS_1364926169_
#define _HTTP_UTILS_1364926169_

#include <string>

namespace arachnid {

  std::string time_string ( time_t t = time(0));

  std::string decode_URI ( const char* uri_buffer );

}

#endif

