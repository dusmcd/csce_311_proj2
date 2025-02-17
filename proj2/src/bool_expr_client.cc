#include "../include/bool_expr_client.h"

const char* SOCKET_PATH =  "/tmp/client_socket";

bool ::BoolExprClient::Connect() {
  if (!clientSocket_.Init())
    return false;
  
  return true;
}

int main() {
  ::DomainSocketClient clientSocket = ::DomainSocketClient(SOCKET_PATH);
  ::BoolExprClient client = ::BoolExprClient(clientSocket);

  if (!client.Connect()) {
    exit(1);
    return -1;
  }
  // if we get here, we have successfully connected to the server
  return 0;
}