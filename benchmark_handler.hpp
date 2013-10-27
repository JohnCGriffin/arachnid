
#ifndef _BENCHMARK_HANDLER_HPP_1365194564_
#define _BENCHMARK_HANDLER_HPP_1365194564_

#include "http_structs.hpp"
#include "handler.hpp"

namespace arachnid {

  struct BenchmarkHandler : public DefaultHandler {
    virtual void get ( HTTPRequest& req, HTTPResponse& res ) const;
  };

}

#endif
