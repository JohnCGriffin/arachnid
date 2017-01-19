

#include "handler_factory.hpp"

#include <stdexcept>
#include <unordered_map>

/********************************************
The handler_registrar is an odd construct.
In lieu of having a static map subject to the
unknown static initialization order across
files, I've opted to have the registrar act
both as the depositor and extractor of Handler
instances mapped to names.

There could be a common structure that knows
about all the implementations.  However, this
method allows each implementation to register
itself without coupling the knowledge of its
existence with that common structure.
********************************************/

namespace arachnid {

  HandlerFactory handler_registrar ( std::string id, const HandlerFactory hf )
  {
    static std::unordered_map<std::string,HandlerFactory> factory_addresses;
    if(hf){
      return factory_addresses [ id ] = hf;
    } else {
      return factory_addresses [ id ];
    }
  }

}

