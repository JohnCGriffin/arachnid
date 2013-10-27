
#ifndef _C_SOURCE_HANDLER_HPP_
#define _C_SOURCE_HANDLER_HPP_

#include <memory>
#include "handler.hpp"


namespace arachnid {

  class CDecoratingHandler : public BasicGetHandler {
    std::shared_ptr<Handler> delegate;
  public:
    CDecoratingHandler ( std::shared_ptr<Handler> delegate ) : delegate(delegate){}
    void get ( HTTPRequest& req, HTTPResponse& res ) const;
  };

}

#endif
