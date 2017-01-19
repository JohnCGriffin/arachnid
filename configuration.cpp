
#include "configuration.hpp"

#include "get_handler.hpp"
#include "utilities.hpp"
#include <fstream>
#include <string>
#include <stdexcept>

/*************************
Read config of form

host1 port1 uri1 handler1 { key1 : val1 ...  keyN : valN }
...
hostN portN uriN handlerN { key1 : val1 ...  keyN : valN }


example configuration for http://mysite.com:12345 with options
for documentRoot and allowance for directory listing.  A remote
SCGI service is forwarded to a host named internal_scgi on port 
9191 for http://mysite.com:12345/app/.

	mysite.com 12345 / FileHandler {
		documentRoot EXAMPLE-DOCUMENT-ROOT
		directoryListing true
	}

	mysite.com 12345 /app SCGIHandler {
		host internal_scgi
		port 9191
	}




*************************/

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


