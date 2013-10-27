
#include "file_utils.hpp"

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

namespace arachnid {

  std::string singleSlash (std::string s)
  {
    size_t pos;
    while((pos = s.find("//")) != std::string::npos){
      s = s.substr(0,pos) + s.substr(pos+1);
    }
    return s;
  }

  std::vector<std::string> directory ( std::string location )
  {
    std::vector<std::string> v;
    DIR* dir = opendir(location.c_str());
    struct dirent* dp;
    while((dp = readdir(dir)) != 0){
      if(dp->d_name[0] != '.'){
	v.push_back(dp->d_name);
      }
    }
    closedir(dir);
    std::sort(v.begin(), v.end());
    return v;
  }

}

