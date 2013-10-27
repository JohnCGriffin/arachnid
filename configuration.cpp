
#include "configuration.hpp"

#include "get_handler.hpp"
#include "utilities.hpp"
#include <fstream>
#include <string>
#include <stdexcept>

namespace arachnid {

  void installEntries ( std::istream& is )
  {
    std::string host, uri, key, val, handler;
    SSMap options;
    int port = 0;
    char c;

    while(is >> host >> port >> uri >> handler >> c){
      if(c != '{'){
	throw std::runtime_error("bad config");
      }
      while(is >> key){
	if(key == "}"){
	  break;
	}
	is >> val;
	options[key] = val;
      }
      add_server ( host, port, uri, handler, options);
    }
  }

  void read_configuration ( std::string config_file )
  {
    std::ifstream ifs(config_file);
    if(!ifs){
      throw std::runtime_error("cannot read configuration file");
    }
    installEntries(ifs);
  }

}


