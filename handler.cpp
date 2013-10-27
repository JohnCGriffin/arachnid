
#include "handler.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include "str_utils.hpp"

namespace arachnid {

  using namespace arachnid::HTTPStringConstants;

  void notImplemented ( HTTPRequest& req, HTTPResponse &res, const char* http_method_name )
  {
    res.code = ResponseCode::RESPONSE_METHOD_NOT_ALLOWED;	
    std::ostringstream oss;
    oss << "HTTP method " << http_method_name << " disallowed for " << req.uri << std::endl;
    res.setTextContent(oss.str());
  }

}



