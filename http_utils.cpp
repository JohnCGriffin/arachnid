
#include "http_utils.hpp"

#include <string>
#include <time.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
  
namespace arachnid {

  std::string time_string ( time_t t )
  {
    char buf[128];
    struct tm tms;
    gmtime_r(&t, &tms);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tms);
    return buf;
  }

  std::string decode_URI (const char *iptr)
  {
    unsigned val;
    char buf[strlen(iptr)+1];
    char* optr = buf;

    // the QUERY_STRING must remain encoded
    while(*iptr && *iptr != '?'){
      if(*iptr == '%' && sscanf(iptr+1,"%2x",&val) == 1){
	*optr++ = val;
	iptr += 3;
      } else {
	*optr++ = *iptr++;
      }
    }

    // simple copy once hitting the ? character
    while(*iptr){
	*optr++ = *iptr++;
    }

    *optr = 0;
    return buf;
  }

}


