#ifndef PROJ2_BOOL_EXPR_SERVER_H
#define PROJ2_BOOL_EXPR_SERVER_H

#include <domain_socket.h>
#include <string>
#include <array>

class BoolExprServer {
  public:
    BoolExprServer(::DomainSocketServer serverSocket) : serverSocket_(serverSocket) {}

    bool Connect();

    // this call will block while it waits for a client to connect
    int AcceptConnection();

    ::ssize_t WriteToClient(int client_req_socket_fd, const std::string msg);
    ::ssize_t ReadFromClient(int client_req_socket_fd, std::string* msg);
    std::array<int, 3> ProcessClientRequest(const std::string& msg, char US, std::string& fileName);
    std::string CreateClientResponse(std::array<int, 3> results, char US);

  private:
    ::DomainSocketServer serverSocket_;
};



#endif // PROJ2_BOOL_EXPR_SERVER_H