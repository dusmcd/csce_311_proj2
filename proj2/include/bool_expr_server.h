#ifndef PROJ2_BOOL_EXPR_SERVER_H
#define PROJ2_BOOL_EXPR_SERVER_H

#include "../../ipc/include/domain_socket.h"
#include <string>

class BoolExprServer {
  public:
    BoolExprServer(::DomainSocketServer serverSocket) : serverSocket_(serverSocket) {}

    bool Connect();

    // this call will block while it waits for a client to connect
    int AcceptConnection();

    ::ssize_t RespondToClient(int client_req_socket_fd, const std::string msg);

  private:
    ::DomainSocketServer serverSocket_;
};



#endif // PROJ2_BOOL_EXPR_SERVER_H