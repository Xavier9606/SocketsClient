#include "ClientAPI.h"


int ClientAPI::initConnection() {


  WSAStartup(MAKEWORD(2, 0), &WSAData);

  server = socket(AF_INET, SOCK_STREAM, 0);

  addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  addr.sin_family = AF_INET;
  addr.sin_port = htons(5555);

  connect(server, (SOCKADDR *) &addr, sizeof(addr));
  listen(server, 0);

  return server;
}

void ClientAPI::sendMsg(char *msg) {

  strcpy(buffer,msg);
  send(server, buffer, bufferSize, 0);
  memset(buffer,0, bufferSize);
}

char *ClientAPI::receiveMsg() {
  recv(server, buffer, bufferSize, 0);
  return buffer;
}

void ClientAPI::setBufferSize(int size){
  delete[] buffer;
  bufferSize=size;
  buffer = new char [bufferSize];
}

void ClientAPI::destroyConnection(){
  closesocket(server);
  WSACleanup();
}

void ClientAPI::clearBuffer(){
  memset(buffer,0, bufferSize);
}

ClientAPI::~ClientAPI(){
  closesocket(server);
  WSACleanup();
}