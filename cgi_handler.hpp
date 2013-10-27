
#ifndef _CGI_HANDLER_1364929115_
#define _CGI_HANDLER_1364929115_

#include "handler.hpp"

namespace arachnid {

  class CGIHandler : public DefaultHandler {
    std::string executablesLocation;
  public:
    CGIHandler ( std::string executablesLocation );
    void getOrPost ( HTTPRequest& req, HTTPResponse& res, const std::string& method ) const;
    virtual void get ( HTTPRequest& req, HTTPResponse& res ) const;
    virtual void post ( HTTPRequest& req, HTTPResponse& res ) const;
  };

}

#endif

