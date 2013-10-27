
#ifndef _SCGI_HANDLER_1364929115_
#define _SCGI_HANDLER_1364929115_

#include "handler.hpp"

namespace arachnid {

  class SCGIHandler : public DefaultHandler {
    std::string host;
    int port;
  public:
    SCGIHandler ( std::string host, int port );
    void getOrPost ( HTTPRequest& req, HTTPResponse& res, const std::string& method ) const;
    virtual void get ( HTTPRequest& req, HTTPResponse& res ) const;
    virtual void post ( HTTPRequest& req, HTTPResponse& res ) const;
  };

}

#endif

