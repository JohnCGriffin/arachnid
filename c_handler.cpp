/////////////////////////////////////////////////////////////////////////
//
// This Handler delegates to another Handler (a FileHandler) and then
// doctors the HTTPResponse to change from C++ code to marked up
// HTML with syntax coloring.  The parsing of the original content
// is performed by flex code.
//
/////////////////////////////////////////////////////////////////////////

#include "c_handler.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>
#include <regex.h>
#include "str_utils.hpp"
#include "tag.hpp"
#include "file_handler.hpp"
#include "handler_factory.hpp"


// code produced by flex compiler
void decorate_cpp_code (std::istream&, std::ostream&);


// embedded CSS
#include "c_handler_css.h"

namespace arachnid {

  static std::string implementation_link ( const HTTPRequest& req )
  {
    const char* f = strrchr(req.uri.c_str(),'/');
    if(f && ends_with(f,".hpp")){
      std::string cpp = replace(f+1,".hpp",".cpp");
      return "<div id='IMPLEMENTATION'>Implementation: <a href='" 
	+ cpp + "'>" + cpp + "</a></div>";
    }
    return "";
  }

  void decorate ( HTTPRequest& req, HTTPResponse& res, bool hasCppFile )
  {
    std::ostringstream oss;
    for(char ch : res.content){
      oss << ch;
    }
    std::istringstream iss(oss.str());
    oss.str("");

    oss << "<!DOCTYPE html>\n";

    TAG(oss, "html"){
      {
	TAG(oss,"head"){
	  oss << "<title>" << after_last(req.uri,'/') << "</title>\n";
	  TAG(oss,"style"){
	    oss.write(c_handler_css, c_handler_css_len);
	  }
	}
      }
      TAG(oss,"body"){
        if(hasCppFile) oss << implementation_link(req);
        TAG(oss,"pre"){
          decorate_cpp_code(iss,oss);
        }
      }
    }

    res.setContent(oss.str());
    res.setMime("text/html");

  }  

  void CDecoratingHandler::get ( HTTPRequest& req, HTTPResponse& res ) const
  {
    delegate->get(req,res);
    if(ends_with(req.uri,".cpp") || ends_with(req.uri,".hpp")){
      if(res.code == ResponseCode::RESPONSE_OK && res.content.size()){
	bool hasCppFile = false;
	if(ends_with(req.uri,".hpp")){
	  HTTPRequest cppRequest;
	  HTTPResponse cppResponse;
	  cppRequest.uri = replace(req.uri,".hpp",".cpp");
	  delegate->get(cppRequest,cppResponse);
	  if(cppResponse.code == ResponseCode::RESPONSE_OK){
	    hasCppFile = true;
	  }
	}
        decorate(req, res, hasCppFile);
      }
    }
  }

  static bool filter (std::string file_name)
  {
    return file_name == "Makefile" ||
      ends_with(file_name,".l") || 
      ends_with(file_name,".h") || 
      ends_with(file_name,".cpp") || 
      ends_with(file_name, ".hpp");	
  }

  static Handler* createHandler (SSMap config)
  {
    std::string documentRoot = config["documentRoot"];
    Handler* handler = new FileHandler(documentRoot,true,filter);
    return new CDecoratingHandler(std::shared_ptr<Handler>(handler));
  }

  static bool registered = handler_registrar ("CSourceHandler", createHandler) == createHandler;

}

