
#ifndef _HANDLER_IMPLEMENTATIONS_HPP_1364926169_
#define _HANDLER_IMPLEMENTATIONS_HPP_1364926169_

#include <vector>
#include "http_structs.hpp"
#include "handler.hpp"
#include "utilities.hpp"

namespace arachnid {

  Handler* get_handler ( HTTPRequest& req );

  std::set<int> getConfiguredPorts();

  void add_server ( std::string host, int port, std::string directory, std::string handlerName, SSMap options);

}

#endif
