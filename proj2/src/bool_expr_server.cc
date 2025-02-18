#include <bool_expr_server.h>
#include <string>

bool BoolExprServer::Connect() {
  freopen("/dev/null", "r", stdin);
  if (!serverSocket_.Init(10)) {
    return false;
  }

  return true;
}

int BoolExprServer::AcceptConnection() {
  int client_req_socket_fd = serverSocket_.Accept();

  return client_req_socket_fd;
}

::ssize_t BoolExprServer::RespondToClient(int client_req_socket_fd, const std::string msg) {
  std::cout << "Client connection received...\n";
  ::ssize_t bytesWritten = serverSocket_.Write(client_req_socket_fd, msg);
  std::cout << bytesWritten << " bytes written to client\n";

  return bytesWritten;
}

int main(int argc, char** argv) {
  const char* boolExprFile = argv[1];
  std::string SOCKET_PATH = argv[2];
  char US = argv[3][0];
  char EOT = argv[4][0];

  ::DomainSocketServer serverSocket = ::DomainSocketServer(SOCKET_PATH.c_str(), US, EOT);
  ::BoolExprServer server = ::BoolExprServer(serverSocket);

  if (!server.Connect()) {
    exit(1);
    return -1;
  }

  while (true) {
    int client_req_socket_fd = server.AcceptConnection();

    // respond with unit-separator (US) and end of transmission (EOT)
    std::string msg = "";
    msg += US;
    ::ssize_t bytesWritten = server.RespondToClient(client_req_socket_fd, msg);

    // get client request
    // respond to client request
  }
  
  return 0;
}