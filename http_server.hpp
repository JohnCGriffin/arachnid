

#ifndef _HTTP_SERVER_HPP_1365098423_
#define _HTTP_SERVER_HPP_1365098423_

namespace arachnid {

  struct PortAndSocket {
    int port;
    int socket;
    PortAndSocket (int port, int socket);
  };

  void* HTTP_server (PortAndSocket* ps);

}

#endif
