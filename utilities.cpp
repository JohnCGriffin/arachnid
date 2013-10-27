

#include "utilities.hpp"

namespace arachnid {

  FILE *safe_fclose(FILE* fp)
  {
    if(fp){
      fclose(fp);
    }
    return 0;
  }

}
