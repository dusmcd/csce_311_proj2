#include "../include/bool_expr_server.h"
#include <string>


int main(int argc, char** argv) {
  const char* boolExprFile = argv[1];
  std::string tmpDir = "/tmp/";
  std::string SOCKET_PATH = tmpDir.append(argv[2]);
  char US = argv[3][0];
  char EOT = argv[4][0];

  ::DomainSocketServer serverSocket = ::DomainSocketServer(SOCKET_PATH.c_str(), US, EOT);
  ::BoolExprServer server = ::BoolExprServer(serverSocket);
  return 0;
}