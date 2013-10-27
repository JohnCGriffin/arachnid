
#include "tcp_conn.hpp"

namespace arachnid {

  int close_and_fail (int socket, std::string msg, std::string* err)
  {
    if(socket >= 0) 
      close(socket);
    if(err) 
      *err = msg;
    return -1;
  }

  int get_server_socket(int port, int backlog, std::string* err)
  {
    int server_socket;
    struct sockaddr_in serv_addr;
    int yes=1;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      return close_and_fail(-1,"socket",err);

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
      return close_and_fail(server_socket,"setsockopt",err);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof serv_addr) == -1)
      return close_and_fail (server_socket, "bind", err);

    if (listen(server_socket, backlog) == -1)
      return close_and_fail (server_socket, "listen", err);

    return server_socket;
  }

  int get_client_socket ( std::string host, int port, std::string* err)
  {
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int client_socket = socket (AF_INET, SOCK_STREAM, 0);

    if(client_socket < 0)
      return close_and_fail (client_socket, "socket", err);

    if(!(server = gethostbyname ( host.c_str() )))
      return close_and_fail (client_socket, "unresolved host: " + host, err);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if(connect(client_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      return close_and_fail (client_socket, "connection failed: " + std::string(host), err);

    return client_socket;
  }

  int data_available (int fd, int seconds, int microseconds)
  {
    int retval;
    fd_set readfds;
    struct timeval tv;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    tv.tv_sec = seconds;
    tv.tv_usec = microseconds;

    retval = select(fd+1, &readfds, NULL, NULL, &tv);
    return retval > 0;
  }

}
