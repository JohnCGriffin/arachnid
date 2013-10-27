
#include "scgi_handler.hpp"

#include <stdexcept>
#include <cstdio>
#include "str_utils.hpp"
#include "utilities.hpp"
#include "handler_factory.hpp"
#include "tcp_conn.hpp"

namespace arachnid {

  using namespace HTTPStringConstants;

  SCGIHandler::SCGIHandler ( std::string host, int port )
    : host(host), port(port)
  {
    if(host.size() == 0){
      throw std::runtime_error("empty hostname supplied to SCGIHandler");
    }
    if(port == 0){
      throw std::runtime_error("bad port supplied to SCGIHandler");
    }
  }

  void SCGIHandler::getOrPost ( HTTPRequest& req, HTTPResponse& res, const std::string& method ) 
	const
  {
    std::string URI = req.uri;
    size_t pos = URI.rfind('/');
    if(pos == std::string::npos){
      res.code = ResponseCode::RESPONSE_NOT_FOUND;
      return;
    }

    SSMap env;

    // copy HTTP headers
    for(auto p : req.header){
      std::string k = upper_case(p.first);
      if (k == "CONTENT-LENGTH" || 
	  k.find("CACHE") != std::string::npos)
	{
	  continue;
	}
      std::string key = replace(upper_case("HTTP_" + p.first),"-","_");
      env[key] = p.second;
    }

    // similar to CGI 
    env["REQUEST_URI"] = req.uri;
    env["REQUEST_METHOD"] = method;
    env["QUERY_STRING"] = URI.find('?') == std::string::npos ? "" : after(URI,'?');
    env["SERVER_PROTOCOL"] = req.protocol;
    env["SERVER_PORT"] = std::to_string(req.port);

    std::string host = before(req.header[HOST],':');
    if(host.length()){
      env["SERVER_NAME"] = host;	
    }

    std::ostringstream oss;
    const char null = (char) 0;
    oss << "CONTENT_LENGTH" << null << req.content.size() << null;
    oss << "SCGI" << null << 1 << null;
    for(auto p : env){
      oss << p.first << null << p.second << null;
    }     

    std::string net_string = std::to_string(oss.tellp()) + ":" + oss.str() + ",";
    std::string problem;

    int s = get_client_socket (host, port, &problem);
    FILE* sfp = s >= 0 ? fdopen(s,"r+w") : 0;

    if(sfp == 0 || s < 0){
      sfp = safe_fclose(sfp);
      if(s >= 0) close(s);
      res.code = ResponseCode::RESPONSE_BAD_GATEWAY;
      res.setMime("text/plain");
      res.setContent(problem);
      return;
    }

    for(char ch : net_string){
      fputc(ch,sfp);
    }
	
    for (char ch : req.content){
      fputc(ch,sfp);
    }

    fflush(sfp);	
    {
      int c;
      char buf[1024];

      while(fgets(buf, sizeof(buf), sfp)){
	std::string line = trim(buf);
	if(line.length() == 0){
	  break;
	}
	std::string key = trim(before(line,':'));
	std::string val = trim(after(line,':'));
	res[key] = val;
      }

      std::ostringstream oss;
      while((c = fgetc(sfp)) != EOF){
	oss << ((char) c);
      } 
      res.code = ResponseCode::RESPONSE_OK;
      res.setContent(oss.str());
    }

    sfp = safe_fclose(sfp);
  }

  void SCGIHandler::get ( HTTPRequest& req, HTTPResponse& res ) const
  {
    getOrPost ( req, res, GET );
  }

  void SCGIHandler::post ( HTTPRequest& req, HTTPResponse& res ) const
  {
    getOrPost ( req, res, POST );
  }

  static Handler* createHandler (SSMap config)
  {
    std::string host = config["host"];
    int port = atoi(config["port"].c_str());
    return new SCGIHandler(host,port);
  }

  static bool registered = handler_registrar ("SCGIHandler", createHandler) == createHandler;
 

}
