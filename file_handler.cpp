
#include "file_handler.hpp"

#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "http_utils.hpp"
#include "str_utils.hpp"
#include "mime.hpp"
#include "file_utils.hpp"
#include "handler_factory.hpp"

namespace arachnid {

  using namespace arachnid::HTTPStringConstants;

  static void set_not_found ( HTTPRequest& req, HTTPResponse& res ) 
  {
    res.code = ResponseCode::RESPONSE_NOT_FOUND;
    res.setContent("<html><head><title>404 - not found</title></head><body>"
		   + req.uri
		   + " not found </body></html>");
    res.setMime("text/html");
  }

  bool FileHandler::everything_passes ( std::string ) { return true; }

  FileHandler::FileHandler ( std::string documentRoot, bool directoryListing, FileFilter filter )  
    : documentRoot(documentRoot), directoryListing(directoryListing), filter(filter)  { }

  void FileHandler::get ( HTTPRequest& req, HTTPResponse& res ) const
  {
    struct stat sb;

    // const char* is more useful here
    std::string file_name = singleSlash(documentRoot + "/" + req.uri);

    if(stat(file_name.c_str(), &sb)){
      set_not_found (req,res);
      return;
    }

    if(sb.st_mode && S_ISDIR(sb.st_mode)){

      {
	std::string index = file_name + "/index.html";
	if(!stat(index.c_str(),&sb)){
	  res.code = ResponseCode::RESPONSE_MOVED_PERMANENTLY;	
	  res[LOCATION] = singleSlash(req.uri + "/index.html");
	  return;
	}
      }


      //if(endsWith(fileName[strlen(fileName)-1] != '/'){
      if(!ends_with(file_name,"/")){
	res.code = ResponseCode::RESPONSE_MOVED_PERMANENTLY;	
	res[LOCATION] = req.uri + "/";
	res.setTextContent("MOVED\n");
	return;
      }

      res.setMime("text/html");

      if(directoryListing){
	std::ostringstream oss;
	std::vector<std::string> d = directory(file_name.c_str());

	oss << "<HTML><body><pre>\n";

	for(std::string f : d){
	  if(filter && !filter(f)){
	    continue;
	  }
	  oss << "<a href=" 
	      << singleSlash(req.uri + "/" + f)
	      << ">" << f << "</a>\n";
	}

	oss << "</pre></body></html>\n";

	res.setContent(oss.str());
	res.code = ResponseCode::RESPONSE_OK;

      } else {
	res.code = ResponseCode::RESPONSE_FORBIDDEN;
	res.setContent("directory listing disallowed");
      }
      return;
    }

    const std::string lastModified = time_string (sb.st_mtime);
    if(lastModified == req[ IF_MODIFIED_SINCE ]){
      res.code = ResponseCode::RESPONSE_NOT_MODIFIED;
      return;
    }

    res [ LAST_MODIFIED ] = lastModified;

    std::ifstream is(file_name);
    res.setContent (is);
    res.setMime (getMIME(file_name));
    res.code = ResponseCode::RESPONSE_OK;
  }

  ///////////////////////////////////////////////////////////////////
  // For the HEAD feature, just use get and whack content
  // This approach is fine for static content but not for 
  // state-altering dynamic stuff.  
  ///////////////////////////////////////////////////////////////////

  void FileHandler::head ( HTTPRequest& req, HTTPResponse& res ) const
  {
    get(req,res);
    if(res.code == ResponseCode::RESPONSE_OK){
      res.content.clear();
    }
  }


  static Handler* createHandler (SSMap config)
  {
    return new FileHandler(config["documentRoot"], 
			   lower_case(config["directoryListing"]) == "true");
  }

  static bool registered = handler_registrar ("FileHandler", createHandler) == createHandler;
}
