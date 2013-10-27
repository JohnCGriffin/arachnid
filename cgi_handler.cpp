
#include "cgi_handler.hpp"

#include <cstdio>
#include "str_utils.hpp"
#include "process.hpp"
#include "utilities.hpp"
#include "handler_factory.hpp"

namespace arachnid {

  using namespace HTTPStringConstants;

  CGIHandler::CGIHandler ( std::string executablesLocation ) 
	: executablesLocation(executablesLocation){}

  void CGIHandler::getOrPost ( HTTPRequest& req, HTTPResponse& res, const std::string& method ) 
	const
  {
    std::string URI = req.uri;
    size_t pos = URI.rfind('/');
    if(pos == std::string::npos){
      res.code = ResponseCode::RESPONSE_NOT_FOUND;
      return;
    }

    std::string executable = before(URI.substr(pos),'?');
    std::string exe = replace(executablesLocation + executable, "//", "/");

    SSMap env;

    // copy HTTP headers
    for(auto p : req.header){
      std::string key = replace(upper_case("HTTP_" + p.first),"-","_");
      env[key] = p.second;
    }

    // CGI specific stuff
    env["REQUEST_URI"] = req.uri;
    env["REQUEST_METHOD"] = method;
    env["SCRIPT_FILENAME"] = exe;
    env["QUERY_STRING"] = URI.find('?') == std::string::npos ? "" : after(URI,'?');
    env["SCRIPT_NAME"] = before(URI,'?');
    env["SERVER_PROTOCOL"] = req.protocol;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_SOFTWARE"] = "Arachnid (C++)";

    std::string host = before(req.header[HOST],':');
    if(host.length()){
      env["SERVER_NAME"] = host;	
    }

    Process process(exe, env);

    FILE* ofp = fdopen(process.wfd,"w");
    FILE* ifp = fdopen(process.rfd,"r");

    if(!ofp || !ifp){
      res.code = ResponseCode::RESPONSE_INTERNAL_SERVER_ERROR;
      res.setTextContent("failed to execute CGI program referenced by " + req.uri);
      return;
    }

    for(char ch : req.content){
      fputc(ch,ofp);
    }

    fclose(ofp);

    {
      int c;
      char buf[1024];

      while(fgets(buf, sizeof(buf), ifp)){
	std::string line = trim(buf);
	if(line.length() == 0){
	  break;
	}
	std::string key = trim(before(line,':'));
	std::string val = trim(after(line,':'));
	res[key] = val;
      }

      std::ostringstream oss;
      while((c = fgetc(ifp)) != EOF){
	oss << ((char) c);
      } 
      fclose(ifp);
      res.code = ResponseCode::RESPONSE_OK;
      res.setContent(oss.str());
    }
  }

  void CGIHandler::get ( HTTPRequest& req, HTTPResponse& res ) const
  {
    getOrPost ( req, res, GET );
  }

  void CGIHandler::post ( HTTPRequest& req, HTTPResponse& res ) const
  {
    getOrPost ( req, res, POST );
  }

   static Handler* createHandler (SSMap config)
  {
    std::string executablesLocation = config["executablesLocation"];
    return new CGIHandler(executablesLocation);
  }

  static bool registered = handler_registrar ("CGIHandler", createHandler) == createHandler;
 

}
