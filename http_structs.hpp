
#ifndef HTTP_REQUEST_H_1364926169_
#define HTTP_REQUEST_H_1364926169_

#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace arachnid {

  namespace HTTPStringConstants {
    extern const std::string HTTP_1_1 ;
    extern const std::string CLOSE ;
    extern const std::string CONTENT_LENGTH ;
    extern const std::string CONTENT_TYPE ;
    extern const std::string LAST_MODIFIED ;
    extern const std::string IF_MODIFIED_SINCE ;
    extern const std::string CONNECTION ;
    extern const std::string SERVER ;
    extern const std::string DATE ;
    extern const std::string KEEP_ALIVE ;
    extern const std::string HOST ;
    extern const std::string SET_COOKIE ;
    extern const std::string COOKIE ;
    extern const std::string WWW_AUTHENTICATE ;
    extern const std::string AUTHORIZATION ;
    extern const std::string LOCATION ;
    extern const std::string GET ;
    extern const std::string POST ;
    extern const std::string HEAD ;
  }


  enum class ResponseCode {

    RESPONSE_OK = 200,

    RESPONSE_MOVED_PERMANENTLY = 301,
    RESPONSE_NOT_MODIFIED = 304,

    RESPONSE_AUTHENTICATE = 401,
    RESPONSE_FORBIDDEN = 403,
    RESPONSE_NOT_FOUND = 404,
    RESPONSE_METHOD_NOT_ALLOWED = 405,
    RESPONSE_REQUEST_URI_TOO_LONG = 414,

    RESPONSE_INTERNAL_SERVER_ERROR = 500,
    RESPONSE_NOT_IMPLEMENTED = 501,
    RESPONSE_BAD_GATEWAY = 502,
    RESPONSE_SERVER_UNAVAILABLE = 503,

  };

  int toInt (const ResponseCode& );

  struct case_insensitive_less {
    bool operator()(std::string const& left, std::string const& right) const ;
  };

  typedef std::map<std::string,std::string,case_insensitive_less> Header;
  typedef std::map<std::string,std::string> CookieJar;


  struct HeaderedAndCookied {

    CookieJar cookies;
    Header header;

    // allows instance[KEY] as alias for instance.header[KEY]
    std::string& operator[] (const std::string key){ return header[key]; }

    virtual ~HeaderedAndCookied (){}
  };

  struct HTTPRequest  : public HeaderedAndCookied {

    int port;
    std::string method;
    std::string uri;
    std::string protocol;
    std::vector<char> content;

  };


  struct HTTPResponse  : public HeaderedAndCookied {

    ResponseCode code;
    std::vector<char> content;

    // convenience 
    std::string codeDescription() const;
    int size() const { return content.size(); }

    // shortcut for CONTENT-TYPE
    void setMime(std::string mime); 

    // shortcuts for byte-by-byte insert
    void setContent(std::string); 
    void setContent(std::istream&); 

    // shortcut for plain/text text
    void setTextContent(std::string); 
  };

  struct Realm {
    Realm (){}
    virtual ~Realm (){}
    virtual bool authenticated ( std::string submission ) = 0;
    virtual std::string name() = 0;
  };

}

#endif
