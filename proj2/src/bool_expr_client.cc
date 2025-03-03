#include <bool_expr_client.h>
#include <string>
#include <iostream>
#include <signal.h>

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
    
  return bytesRead;
}

::ssize_t BoolExprClient::ReadFromServer(std::string* buffer, char eot) {
  return clientSocket_.Read(eot, buffer);
}

std::array<int, 3> BoolExprClient::FormatResponse(const std::string res, char us, char eot) {
  std::string val;
  char current;
  std::array<int, 3> results;
  int i = 0;
  int j = 0;

  do {
    current = res[i];
    i++;

    if (current == us || current == eot) {
      results[j] = atoi(val.c_str());
      j++;
      val.clear();
      continue;
    }

    val += current;
  } while (current != eot);

  return results;
}



int main(int argc, char** argv) {
  std::string SOCKET_PATH = argv[1];
    
  ::DomainSocketClient clientSocket = ::DomainSocketClient(SOCKET_PATH.c_str());
  ::BoolExprClient client = ::BoolExprClient(clientSocket);

  if (signal(SIGPIPE, SIG_IGN))
    std::cout << "Writer disconnected\n";

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
  std::array<int, 3> results = client.FormatResponse(response, US, EOT);
  std::cout << "True Evaluations:\t" << results[0] << "\n";
  std::cout << "False Evaluations:\t" << results[1] << "\n";
  std::cout << "Could Not Evaluate\t" << results[2] << "\n";
  return 0;
}