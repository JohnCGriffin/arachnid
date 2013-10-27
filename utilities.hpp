#ifndef _UTILITIES_HPP_1364926169_
#define _UTILITIES_HPP_1364926169_

#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include <string>
#include <set>


namespace arachnid {

  // too common not to rename
  typedef std::map<std::string,std::string> SSMap;

  // fp = safe_fclose(fp); avoids conditional and reassignment of NULL pointer
  FILE* safe_fclose(FILE *fp);

}

#endif

