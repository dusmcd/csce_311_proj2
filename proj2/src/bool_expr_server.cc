#include "../include/bool_expr_server.h"

const char* SOCKET_PATH = "/tmp/server_socket";
const char EOT = '\004'; // end of transmission character
const char US = '\037'; // unit separator character

int main() {
  ::DomainSocketServer serverSocket = ::DomainSocketServer(SOCKET_PATH, US, EOT);
  ::BoolExprServer server = ::BoolExprServer(serverSocket);
  return 0;
}