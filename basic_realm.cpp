
#include "basic_realm.hpp"

#include "str_utils.hpp"

namespace arachnid {

  using namespace arachnid::HTTPStringConstants;

  bool BasicRealmAdaptor::authenticated ( HTTPRequest& req, HTTPResponse& res ) const
  {
    std::string submission = after_last(req[AUTHORIZATION], ' ');
    if(!realm->authenticated (submission)){
      res[WWW_AUTHENTICATE] = "Basic realm=\"" + realm->name() + "\"";
      res.code = ResponseCode::RESPONSE_AUTHENTICATE;
      return false;
    }
    return true;
  }

  void BasicRealmAdaptor::get ( HTTPRequest& req, HTTPResponse& res ) const
  {
    if(authenticated(req,res)){
      delegate->get(req,res);
    }
  }

  void BasicRealmAdaptor::head ( HTTPRequest& req, HTTPResponse& res ) const
  {
    if(authenticated(req,res)){
      delegate->head(req,res);
    }
  }
	
  void BasicRealmAdaptor::post ( HTTPRequest& req, HTTPResponse& res ) const
  {
    if(authenticated(req,res)){
      delegate->post(req,res);
    }
  }

}



