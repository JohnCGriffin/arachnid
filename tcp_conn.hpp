
#ifndef _TCP_CNTN_1364926169_
#define _TCP_CNTN_1364926169_

#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>
#include <cstring>
#include <string>

namespace arachnid {

  int get_server_socket(int port, int backlog = 10, std::string* err = 0);

  int get_client_socket ( std::string host, int port, std::string* err = 0);

  int data_available (int fd, int seconds=0, int microseconds=0);

}

#endif

