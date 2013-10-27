
#include "http_server.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <fstream>
#include <set>
#include <utility>
#include <type_traits>

#include "tcp_conn.hpp"
#include "http_structs.hpp"
#include "http_utils.hpp"
#include "str_utils.hpp"
#include "handler.hpp"
#include "utilities.hpp"
#include "get_handler.hpp"

#include "portability.hpp"

namespace arachnid {

  PortAndSocket::PortAndSocket (int port, int socket) : port(port), socket(socket){}

  using namespace HTTPStringConstants;

  static bool read_token (FILE* fp, std::string& s)
  {
    char buf[1024];
    if(fscanf(fp, "%1023s", buf) == 1){
      s = decode_URI (buf);
      return true;
    }
    return false;
  }

  static bool cookie_token(std::istream& is, std::string& s, char stop)
  {
    is.unsetf(std::ios::skipws);
    char c;
    s.clear();
    while(::isspace(is.peek()) && (is >> c));
    while((is >> c) && c != stop) s += c;
    return s.size() > 0;
  }

  static void read_header_entries (FILE*fp, HTTPRequest& req)
  {
    char buf[1024], key[1024], val[1024];

    while(fgets(buf, sizeof(buf), fp) && sscanf(buf," %[^: ] : %[^\r\n]", key, val) == 2){
      if(COOKIE == key){
	std::istringstream iss(val);
	std::string cookieName, cookieValue;
	while(cookie_token(iss,cookieName,'=') && cookie_token(iss,cookieValue,';')){
	  req.cookies[cookieName] = cookieValue;
	}
      } else {
	req[key] = val;
      }
    }
  }


  void* HTTP_server (PortAndSocket* ps)
  {
    int port = ps->port;
    int worker = ps->socket;
    FILE* sock_fp = fdopen(worker,"r+w");

    delete ps;
    ps = 0;

    try {

      while ( sock_fp ) {

	fflush(sock_fp);

	if (!data_available(worker, 3 )) break;

	HTTPRequest req;

	if(read_token(sock_fp, req.method)
	   && read_token(sock_fp, req.uri)
	   && read_token(sock_fp, req.protocol))
	  {
	    while(fgetc(sock_fp) > '\n');
	  } else break;
    

	req.port = port;
	req.header[CONNECTION] = CLOSE; // this may be overridden by client

	read_header_entries (sock_fp, req);

	{
	  int content_length = atoi(req[CONTENT_LENGTH].c_str());
	  int c;
	  while(content_length-- > 0 && ((c = fgetc(sock_fp)) != EOF)){
	    req.content.push_back(c);
	  }
	}

	HTTPResponse res;

	res[SERVER] = "Arachnid";
	res[CONNECTION] = req[CONNECTION];

	// Copy cookies to response
	res.cookies = req.cookies;

	Handler* h = get_handler (req);

	
	if (req.method == GET)
	  h->get(req,res);
	else if (req.method == POST)
	  h->post(req,res);
	else if (req.method == HEAD)
	  h->head(req,res);
	else
	  break;

	res[ CONTENT_LENGTH ] = std::to_string(res.size()); //numString(res.size());
	res[ DATE ] = time_string();

	std::ostringstream oss;

	// output result code
	oss << HTTP_1_1 << " " <<  toInt(res.code) << " " << res.codeDescription() << "\r\n";

	// output header 
	for(auto p : res.header){
	  oss << p.first << ": " << p.second << "\r\n";
	}

	// output cookies
	for (auto p : res.cookies){
	  oss << SET_COOKIE << ": " << p.first << "=" << p.second << "\r\n";
	}

	// final emptyline
	std::string s = oss.str();
	fprintf(sock_fp, "%s\r\n", s.c_str());

	// Note that the content size may differ from CONTENT_LENGTH
	// when a HEAD response gives length but not data 

	for(char ch : res.content){
	  fputc(ch, sock_fp);
	}

	if(strcasecmp(res[CONNECTION].c_str(), KEEP_ALIVE.c_str())){
	  sock_fp = safe_fclose(sock_fp);
	}

	if(toInt(res.code) >= 500){
	  sock_fp = safe_fclose(sock_fp);
	}

      }

    } catch (std::exception& e){
      std::cerr << "during processing on port " << port << ":" << e.what() << std::endl;
    }

    sock_fp = safe_fclose(sock_fp);

    return 0;
  }

#if HAS_UNDERLYING_TYPE
  int toInt (const ResponseCode& c){
	return static_cast<std::underlying_type<ResponseCode>::type>(c);
  }
#else
  int toInt (const ResponseCode& c){
	//return static_cast<std::underlying_type<ResponseCode>::type>(c);
	int i;
	memcpy(&i,&c, sizeof(i));
	return i;
  }
#endif

}

