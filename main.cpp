///////////////////////////////////////////////////////////////////////
//
// At the top level, main opens one pthread for each needed port. 
// Each of those threads listens and then fires off pthread instances
// for each connection.
//
// This is good for a about a thousand concurrent connections. 
// 
///////////////////////////////////////////////////////////////////////

#include "http_server.hpp"

#include "tcp_conn.hpp"
#include "configuration.hpp"
#include "get_handler.hpp"
#include <pthread.h>
#include <errno.h>
#include <set>

using namespace arachnid;

static void* service_main(void* vp)
{
  const int port = (int) (long) vp;
  const int server = get_server_socket (port);

  while(server > -1){

    const int worker = accept (server, NULL, 0);

    if (worker < 0){
      std::cerr << "accept failed - errno " << errno << std::endl;
      continue;
    }

    // On my Linux and OSX boxes, the default appears to be an
    // unnecessarily large 8 Megs of stack, so I'm saying 1 Meg.

    pthread_t p;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr,1024*1024);

    while(pthread_create(&p, &attr, (void* (*)(void*))HTTP_server, new PortAndSocket(port,worker))){
      std::cerr << "pthread_create failed" << std::endl;
      sleep(1);
    }

    pthread_detach(p);
	
  }

  return 0;
}

int main(int argc, const char** argv)
{
  try {

    read_configuration (argc == 2 ? argv[1] : "/usr/local/etc/arachnid.conf");

    std::set<pthread_t> threads;

    for(auto port : getConfiguredPorts()){
      pthread_t p;
      if(pthread_create(&p, 0, service_main, (void*) (long) port)){
	std::cerr << "main pthread_create failed for port " << port << std::endl;
	exit(1);
      }
      threads.insert(p);
    }

    for(auto t : threads){
      pthread_join(t,0);
    }

    return 0;

  } catch (std::exception& e){
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

