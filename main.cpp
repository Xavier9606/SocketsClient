#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <winsock2.h>
#include "ClientAPI.h"
#include <thread>
#include <mutex>
#include <time.h>

std::mutex m;

ClientAPI clients[10];
//ClientAPI client, client2;
int clientCount = 0;


void clientReceiverLoop(int clientId) {
  while (true) {
    std::cout << std::endl << "[ Client " << clientId << " ]<--" << clients[clientCount].receiveMsg() << std::endl
              << std::flush;
    std::cout << "[ Client " << clientId << " ]--> " << std::flush;
  }
}


void clientInitLoop() {
  m.lock();
  clients[clientCount].initConnection();
  // Sleep(500);
  int clientId = strtol(clients[clientCount].receiveMsg(), nullptr, 0);
  clientCount++;
  if (clientCount > 0) { clientCount = 0; };//comment this to create more than 1 thread in 1 file
  std::cout << "Client " << clientId << " active" << std::endl;
  m.unlock();

  std::string massage = " ";
  std::thread clientReceiver(clientReceiverLoop, clientId);

  // int i = 0;
  while (true) {
//    massage = "Rep ";
//    massage += std::to_string(rand() % 100);
//    massage += " # ";
//    massage += std::to_string(i);
    std::cout << "[ Client " << clientId << " ]--> " << std::flush;
    std::cin.getline((char *) massage.c_str(), 1024);
    //std::cout << std::endl << std::flush;
    clients[clientCount].sendMsg((char *) massage.c_str());
    //i++;
    Sleep(10);
  }

}


int main() {
  srand(time(NULL));

  std::thread clientThread1(clientInitLoop);
  clientThread1.detach();
  //Sleep(500);
//  std::thread clientThread2(clientInitLoop);
//  clientThread2.detach();
// // Sleep(500);
//  std::thread clientThread3(clientInitLoop, clientCount);
//  clientThread3.detach();
// // Sleep(500);
//  std::thread clientThread4(clientInitLoop, clientCount);
//  clientThread4.detach();
// // Sleep(500);
//  std::thread clientThread5(clientInitLoop, clientCount);
//  clientThread5.detach();


  Sleep(1000000);

  return 0;
}
