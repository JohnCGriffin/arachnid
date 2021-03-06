
#ifndef _HANDLER_FACTORY_HPP_1365187690_
#define _HANDLER_FACTORY_HPP_1365187690_

#include "handler.hpp"
#include "utilities.hpp"
#include <string>
#include <map>

/* This is all about getting around
   not having defined order in C++
   static initialization. */

namespace arachnid {

  typedef Handler* (*HandlerFactory)(SSMap);

  HandlerFactory handler_registrar ( std::string id, HandlerFactory hf = 0 ); 

}

#endif
