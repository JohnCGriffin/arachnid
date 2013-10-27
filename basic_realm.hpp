
#ifndef _BASIC_REAL_ADAPTOR_2938746_
#define _BASIC_REAL_ADAPTOR_2938746_

  /*************************************************************************
  BasicRealmAdaptor decorates another Handler for web authentication.
  For instance, this code with demand login/password of howdy/doody.
        
  struct HowdyDoodyRealm  : public Realm {
    HowdyDoodyRealm () : Realm (){}
    bool authenticated ( std::string submission ) {
      return submission == "aG93ZHk6ZG9vZHk="; // base64 of howdy:doody
    }
    std::string name(){ return "HOWDY DOODY"; }
  };
  *************************************************************************/

#include "handler.hpp"
#include <memory>

namespace arachnid {

  typedef std::shared_ptr<Handler> SharedPtrHandler;
  typedef std::shared_ptr<Realm> SharedPtrRealm;

  struct BasicRealmAdaptor : public Handler {
    SharedPtrRealm realm;
    SharedPtrHandler delegate;

    BasicRealmAdaptor  ( SharedPtrRealm realm, SharedPtrHandler delegate )
      : realm(realm), delegate(delegate) { }

    bool authenticated ( HTTPRequest& req, HTTPResponse& res ) const;

    virtual void get ( HTTPRequest& req, HTTPResponse& res ) const;
    virtual void head ( HTTPRequest& req, HTTPResponse& res ) const;
    virtual void post ( HTTPRequest& req, HTTPResponse& res ) const;
  };

}

#endif
