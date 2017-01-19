
#include "get_handler.hpp"

#include <string>
#include <stdexcept>
#include "uri_match.hpp"
#include "str_utils.hpp"
#include "utilities.hpp"
#include "cgi_handler.hpp"
#include "c_handler.hpp"
#include "file_handler.hpp"
#include "benchmark_handler.hpp"
#include "handler_factory.hpp"

namespace arachnid {

  using namespace arachnid::HTTPStringConstants;

  struct ConfigEntry {
    std::string host;
    int port;
    Handler* handler;
    URIMatcher* matcher;
  };

  static std::vector<ConfigEntry> entries;

  void add_server ( std::string host, 
		    int port, 
		    std::string directory, 
		    std::string handlerName, 
		    SSMap options)
  {
    HandlerFactory hf = handler_registrar (handlerName);

    if(!hf){
      throw std::runtime_error("unknown handler " + handlerName);
    }

    ConfigEntry entry;
    entry.host = lower_case(host);
    entry.port = port;
    entry.handler = hf(options);
    entry.matcher = new Specified_URI(directory);

    entries.push_back(entry);
  }

  Handler* get_handler ( HTTPRequest& req )
  {
    static Handler* defaultHandler = new DefaultHandler;
    auto host = lower_case(before(req[HOST], ':'));
    auto uri = req.uri;

    auto match = std::find_if(entries.begin(),entries.end(),
			      [=](const ConfigEntry& e){
				return e.port == req.port 
				&& e.host == host 
				&& e.matcher->matches(uri);
			      });

    return match == entries.end() ? defaultHandler : match->handler;
  }

  
  // multiple configuration entries may probably point
  // to the same port, e.g. 80.  One listening socket
  // per port is needed in the main loop.
  
  std::set<int> getConfiguredPorts()
  {
    std::set<int> ports;
    for(auto entry : entries) ports.insert(entry.port);
    return ports;
  }

}

