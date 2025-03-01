#include <bool_expr_client.h>
#include <string>
#include <iostream>


bool BoolExprClient::Connect() {
  freopen("/dev/null", "r", stdin);
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

::ssize_t BoolExprClient::GetSpecialChars(std::string* buffer) {
  size_t bytesToRead = 2;
  ::ssize_t bytesRead = clientSocket_.Read(bytesToRead, buffer);
  if (bytesRead != 2)
    return -1;
  
  return bytesRead;
}

::ssize_t BoolExprClient::WriteToServer(char eot, std::string msg) {
  ::ssize_t bytesRead = clientSocket_.Write(msg, eot);
  if (bytesRead == -1)
    return -1;
  
    return bytesRead;
}

::ssize_t BoolExprClient::ReadFromServer(std::string* buffer, char eot) {
  return clientSocket_.Read(eot, buffer);
}



int main(int argc, char** argv) {
  std::string SOCKET_PATH = argv[1];
    
  ::DomainSocketClient clientSocket = ::DomainSocketClient(SOCKET_PATH.c_str());
  ::BoolExprClient client = ::BoolExprClient(clientSocket);


  if (!client.Connect()) {
    exit(1);
    return -1;
  }

  // if we get here, we have successfully connected to the server
  std::cout << "BoolExprClient connecting...\n";

  // read US and EOT values from server
  std::string buffer;
  if (client.GetSpecialChars(&buffer) == -1) {
    std::cerr << "Error getting US and EOT characters from server\n";
    exit(1);
    return -1;
  }

  const char US = buffer[0];
  const char EOT = buffer[1];
  std::cout << "Unit separator: " << std::oct << static_cast<int>(US) << "\n";
  std::cout << "EOT: " << static_cast<int>(EOT) << "\n";
  std::string message = client.FormatMessage(argc, argv, US, EOT);

  ::ssize_t bytesWritten = client.WriteToServer(EOT, message);
  if (bytesWritten == -1) {
    std::cerr << "Error writing to server\n";
    exit(1);
    return -1;
  }

  // read from server
  std::string response;
  ::ssize_t bytesRead = client.ReadFromServer(&response, EOT);
  if (bytesRead == -1) {
    std::cerr << "Error reading from server\n";
    exit(1);
    return -1;
  }

  // output results...
  std::cout << "Finished with " << bytesRead << "B received, " << bytesWritten << "B sent.\n";
  std::cout << "Results\n";
  return 0;
}