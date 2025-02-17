#ifndef PROJ2_BOOL_EXPR_SERVER_H
#define PROJ2_BOOL_EXPR_SERVER_H

#include "../../ipc/include/domain_socket.h"

class BoolExprServer {
  public:
    BoolExprServer(::DomainSocketServer serverSocket) : serverSocket_(serverSocket) {}

  private:
    ::DomainSocketServer serverSocket_;
};



#endif // PROJ2_BOOL_EXPR_SERVER_H