#include "../include/bool_expr_client.h"
#include <string>


bool BoolExprClient::Connect() {
  if (!clientSocket_.Init())
    return false;
  
  return true;
}


const std::string BoolExprClient::FormatMessage(int argc, char** argv, char US, char EOT) {
  std::string vals;

  for (int i = 2; i < argc; i++) {
    vals.append(argv[i]);
    if (i != argc - 1)
      vals += US;
    else
      vals += EOT;
  }

  return vals;
}



int main(int argc, char** argv) {
  std::string tmpPath = "/tmp/";
  std::string SOCKET_PATH = tmpPath.append(argv[1]);
    
  ::DomainSocketClient clientSocket = ::DomainSocketClient(SOCKET_PATH.c_str());
  ::BoolExprClient client = ::BoolExprClient(clientSocket);


  if (!client.Connect()) {
    exit(1);
    return -1;
  }

  // if we get here, we have successfully connected to the server
  // read US and EOT values from server
  std::string message = client.FormatMessage(argc, argv, US, EOT);
  return 0;
}