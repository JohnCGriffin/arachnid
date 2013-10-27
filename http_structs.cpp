
#include "http_structs.hpp"

#include <iterator>
#include <sstream>
#include <cstring>
#include "str_utils.hpp"

namespace arachnid {

  namespace HTTPStringConstants {
    const std::string HTTP_1_1 = "HTTP/1.1";
    const std::string CLOSE = "Close";
    const std::string CONTENT_LENGTH = "Content-Length";
    const std::string CONTENT_TYPE = "Content-Type";
    const std::string LAST_MODIFIED = "Last-Modified";
    const std::string IF_MODIFIED_SINCE = "If-Modified-Since";
    const std::string CONNECTION = "Connection";
    const std::string SERVER = "Server";
    const std::string DATE = "Date";
    const std::string KEEP_ALIVE = "Keep-Alive";
    const std::string HOST = "Host";
    const std::string SET_COOKIE = "Set-Cookie";
    const std::string COOKIE = "Cookie";
    const std::string WWW_AUTHENTICATE = "WWW-Authenticate";
    const std::string AUTHORIZATION = "Authorization";
    const std::string LOCATION = "Location";
    const std::string GET = "GET";
    const std::string HEAD = "HEAD";
    const std::string POST = "POST";
  }

  using namespace arachnid::HTTPStringConstants;

  bool case_insensitive_less::operator()(std::string const& left, std::string const& right) const 
  {
    return strcasecmp(left.c_str(), right.c_str()) < 0;
  }

  void HTTPResponse::setMime ( std::string mime )
  {
    header[ CONTENT_TYPE ] = mime;
  }

  void HTTPResponse::setContent ( std::string text )
  {
    std::istringstream is(text);
    setContent(is);
  }

  void HTTPResponse::setContent ( std::istream& is )
  {
    content.clear();
    is.unsetf(std::ios::skipws);
    std::istream_iterator<char> isIterBegin(is);
    std::istream_iterator<char> isIterEnd;
    std::copy(isIterBegin,isIterEnd, std::back_inserter(content));
    header [ CONTENT_LENGTH ] = std::to_string(content.size());
  }

  void HTTPResponse::setTextContent ( std::string text )
  {
    setContent(text);
    setMime("text/plain");	
  }

  std::string HTTPResponse::codeDescription ( ) const
  {
    switch(code){

    case ResponseCode::RESPONSE_OK : return "OK";

    case ResponseCode::RESPONSE_NOT_MODIFIED : return "Not Modified";

    case ResponseCode::RESPONSE_FORBIDDEN : return "Forbidden";

    case ResponseCode::RESPONSE_NOT_FOUND : return "Not Found";

    case ResponseCode::RESPONSE_METHOD_NOT_ALLOWED : return "Method Not Allowed";

    case ResponseCode::RESPONSE_REQUEST_URI_TOO_LONG : return "Request-URI Too Long";

    case ResponseCode::RESPONSE_INTERNAL_SERVER_ERROR : return "Internal Server Error";

    case ResponseCode::RESPONSE_NOT_IMPLEMENTED : return "Not Implemented";

    case ResponseCode::RESPONSE_SERVER_UNAVAILABLE : return "Server Unavailable";

    case ResponseCode::RESPONSE_AUTHENTICATE: return "Unauthorized";

    case ResponseCode::RESPONSE_MOVED_PERMANENTLY: return "Moved Permanently";

    case ResponseCode::RESPONSE_BAD_GATEWAY: return "Bad Gateway";
    }

    return "";
  }

}

