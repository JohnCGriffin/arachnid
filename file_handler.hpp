
#ifndef _FILE_HANDLER_1364929115_
#define _FILE_HANDLER_1364929115_

#include "handler.hpp"

namespace arachnid {

  class FileHandler : public BasicGetHandler {

    typedef bool (*FileFilter)(std::string);

    const std::string documentRoot;
    const bool directoryListing;
    const FileFilter filter;
    static bool everything_passes(std::string fileName);

  public:    
    FileHandler (std::string documentRoot, 
		 bool directoryListing = false, 
		 FileFilter filter = everything_passes);

    void get ( HTTPRequest& req, HTTPResponse& res ) const;
    void head ( HTTPRequest& req, HTTPResponse& res ) const;

  };

}

#endif

