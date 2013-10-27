
#include "benchmark_handler.hpp"

#include "handler.hpp"
#include "handler_factory.hpp"
#include "utilities.hpp"

#include <string>
#include <map>

namespace arachnid {

  void BenchmarkHandler::get ( HTTPRequest& , HTTPResponse& res ) const
  {
    res.code = ResponseCode::RESPONSE_OK;
    res.setTextContent("Short network-only benchmark");
  }

  static Handler* createHandler (SSMap)
  {
    return new BenchmarkHandler();
  }

  static bool registered = handler_registrar ("BenchmarkHandler", createHandler) == createHandler;

}
