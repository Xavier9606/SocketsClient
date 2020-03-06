#pragma once
#include <winsock2.h>

class ClientAPI {
private:
  WSADATA WSAData;
  SOCKET server;
  SOCKADDR_IN addr;
  int bufferSize = 1024;
  char *buffer = new char[bufferSize];
  //char buffer[1024] ;


public:
  int initConnection();

  void sendMsg(char *msg, int size);

  char *receiveMsg(int *err);

  char *receiveMsg();

  void setBufferSize(int size);

  void clearBuffer();

  void destroyConnection();

  ~ClientAPI();
};



