#include <bool_expr_server.h>
#include <string>
#include <bool_expr_parser.h>
#include <iostream>
#include <fstream>
#include <array>
#include <signal.h>

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

::ssize_t BoolExprServer::WriteToClient(int client_req_socket_fd, const std::string msg) {
  ::ssize_t bytesWritten = serverSocket_.Write(client_req_socket_fd, msg);
  return bytesWritten;
}


::ssize_t BoolExprServer::ReadFromClient(int client_req_socket_fd, std::string* msg) {
  ::ssize_t bytesRead = serverSocket_.Read(client_req_socket_fd, msg);
  return bytesRead;
}


std::array<int, 3> BoolExprServer::ProcessClientRequest(const std::string& msg, char US, std::string& fileName) {
  std::string booleanVals = ::Explode(msg.c_str(), US);
  std::unordered_map<char, bool> valMap = ::BuildMap(booleanVals);
  std::array<int, 3> evaluations = {};

  std::ifstream inputFile(fileName);
  inputFile.seekg(0, inputFile.beg); // set cursor to beginning of file

  std::string line; // temp buffer
  while (std::getline(inputFile, line)) {
    const std::string token = ::Explode(line.c_str(), ' ');
    ::BooleanExpressionParser parser(token, valMap);
    bool result = parser.Parse();

    if (parser.HasError())
      evaluations[2]++;
    else if (result)
      evaluations[0]++;
    else
      evaluations[1]++;
  }
   
  inputFile.close();
  return evaluations;
  
}

std::string BoolExprServer::CreateClientResponse(std::array<int, 3> results, char US) {
  std::string result;
  for (int i = 0; i < 3; i++) {
    result.append(std::to_string(results[i]));
    if (i != 2) {
      result += US;
    }
  }
  return result;
}

int main(int argc, char** argv) {
  if (signal(SIGPIPE, SIG_IGN))
    std::cout << "Broken pipe\n";

  if (argc < 5) {
    std::cerr << "Not enough arguments provided\n";
    exit(1);
    return -1;
  }

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
    std::cout << "Client connected\n";

    // respond with unit-separator (US) and end of transmission (EOT)
    std::string msg = "";
    msg += US;
    ::ssize_t bytesWritten = server.WriteToClient(client_req_socket_fd, msg);
    if (bytesWritten == -1) {
      std::cerr << "Error writing to client\n";
      continue;
    }

    // get client request
    std::string msgFromClient;
    ::ssize_t bytesRead = server.ReadFromClient(client_req_socket_fd, &msgFromClient);
    if (bytesRead == -1) {
      std::cout << "Error reading from client\n";
      continue;
    }
    else {
      // process client request
      std::string fileName;
      fileName.append(boolExprFile);
      std::array<int, 3> result = server.ProcessClientRequest(msgFromClient, US, fileName);
      std::string payload = server.CreateClientResponse(result, US);
      // send respone
      bytesWritten = server.WriteToClient(client_req_socket_fd, payload);
    }

    if (bytesWritten == -1) {
      std::cerr << "Error writing to client\n";
      continue;
    }
    std::cout << "\t" << bytesWritten << "B sent, " << bytesRead << "B received.\n";
    std::cout << "Client disconnected\n";
  }
  
  return 0;
}