#ifndef PROJ2_BOOL_EXPR_CLIENT_H
#define PROJ2_BOOL_EXPR_CLIENT_H

#include "../../ipc/include/domain_socket.h"

class BoolExprClient {
  public:
    BoolExprClient(::DomainSocketClient clientSocket) : clientSocket_(clientSocket) {}

    // connect to local server using clientSocket_
    bool Connect();
    
    // formats the T/F values with the unit-separator(US) char between values
    const std::string FormatMessage(int argc, char** argv, char US, char EOT);
    
  private:
    ::DomainSocketClient clientSocket_;
};

#endif //PROJ2_BOOL_EXPR_CLIENT_H