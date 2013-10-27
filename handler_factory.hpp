
#ifndef _HANDLER_FACTORY_HPP_1365187690_
#define _HANDLER_FACTORY_HPP_1365187690_

#include "handler.hpp"
#include "utilities.hpp"
#include <string>
#include <map>

namespace arachnid {

  typedef Handler* (*HandlerFactory)(SSMap);

  HandlerFactory handler_registrar ( std::string id, HandlerFactory hf = 0 ); 

}

#endif
