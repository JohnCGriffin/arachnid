
#include "mime.hpp"

#include <fstream>
#include <unordered_map>
#include "str_utils.hpp"

namespace arachnid {

  // if first 1K indicates lines less than 100 chars long
  // and nothing binary, then it's probably viewable as text.
  // Use case: Makefile

  static bool plausible_text ( std::string file_name )
  {
    int newlines = 0;	
    int count = 0;
    std::ifstream ifs(file_name);
    char c;
    ifs.unsetf(std::ios::skipws);
    while(count++ < 1024 && (ifs >> c)){
      if(c < 9 || c > 126){
        return false;
      }
      if(c == '\n') newlines++;
    }	
    return newlines && (count / newlines) < 100;
  }


  std::string getMIME ( std::string file_name )
  {
    static std::unordered_map<std::string, std::string> mime_by_extension {
	{ "html", "text/html" },
	{ "css",  "text/css" },
	{ "js",   "application/x-javascript" },
	{ "json", "application/json" },
	{ "txt",  "text/plain" },
	{ "c",    "text/plain" },
	{ "h",    "text/plain" },
	{ "hpp",  "text/plain" },
	{ "cpp",  "text/plain" },
	{ "svg",  "image/svg+xml" },
	{ "jpg",  "image/jpeg" },
	{ "png",  "image/png" },
	{ "gif",  "image/gif" },
	{ "pdf",  "application/pdf" },
	{ "mp3",  "audio/mpeg" },
	{ "wav",  "audio/x-wav" },
	{ "mov",  "video/quicktime" },
	{ "mpg",  "video/mpeg" },
	{ "mpeg", "video/mpeg" },
	{ "mp4",  "video/mp4" }
    };

    const std::string ext = lower_case(after_last(file_name,'.'));

    const std::string result = mime_by_extension[ext];

    return result.size() ? result
		: plausible_text ( file_name ) ? "text/plain"
		: "application/octect-stream";
  }

}


