#ifndef PROJ2_BOOL_EXPR_CLIENT_H
#define PROJ2_BOOL_EXPR_CLIENT_H

#include "../../ipc/include/domain_socket.h"

class BoolExprClient {
  public:
    BoolExprClient(::DomainSocketClient clientSocket) : clientSocket_(clientSocket) {}

    // connect to local server using clientSocket_
    bool Connect();
  private:
    ::DomainSocketClient clientSocket_;
};

#endif //PROJ2_BOOL_EXPR_CLIENT_H