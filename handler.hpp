
#ifndef _HANDLER_HPP_1364926169_
#define _HANDLER_HPP_1364926169_

#include "http_structs.hpp"

#include <string>

/****************************************************************
Handler is the equivalent of iHTTPHandler.processRequest in .NET
or Servlet.service in Java.  It takes an HTTPRequest and
modifies the content of a HTTPResponse.  Hello world could be
implemented as

void get (HTTPRequest& req, HTTPResponse& res) const
{
	res.code = RESPONSE_OK;
	res.setContent("Hello World");
	res.setMime ("text/plain");
}
*****************************************************************/

namespace arachnid {

  struct Handler {
    virtual void get  ( HTTPRequest& req, HTTPResponse& res ) const = 0;
    virtual void head ( HTTPRequest& req, HTTPResponse& res ) const = 0;
    virtual void post ( HTTPRequest& req, HTTPResponse& res ) const = 0;
    virtual ~Handler(){}
  };      


  // preimplemented non-working defaults
  void notImplemented ( HTTPRequest& req, HTTPResponse& res, const char* http_method_name );

  struct NoHeadHandler : public Handler {
    void head ( HTTPRequest& req, HTTPResponse& res ) const
    {  notImplemented ( req, res, "HEAD" ); }
  };
        
  struct BasicGetHandler : public NoHeadHandler {
    void post ( HTTPRequest& req, HTTPResponse& res ) const
    { notImplemented ( req, res , "POST" ); }
  };

  struct BasicPostHandler : public NoHeadHandler {
    void get ( HTTPRequest& req, HTTPResponse& res ) const
    { notImplemented ( req, res, "GET" ); }
  };
        
  struct DefaultHandler : public Handler {

    virtual void get ( HTTPRequest& req, HTTPResponse& res ) const
    { notImplemented ( req, res, "GET" ); }

    virtual void post ( HTTPRequest& req, HTTPResponse& res ) const
    { notImplemented ( req, res, "POST" ); }

    virtual void head ( HTTPRequest& req, HTTPResponse& res ) const
    { notImplemented ( req, res, "HEAD" ); }
  };

}

#endif


