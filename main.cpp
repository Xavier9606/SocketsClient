#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <winsock2.h>
#include "ClientAPI.h"
#include <thread>
#include <mutex>
#include <random>
#include <ctime>

#ifdef NDEBUG
const int NUMBER_OF_CLIENTS_EMULATED = 1;
const int MASSAGE_SLEEP_TIME = 0;
const bool LOG_MASSAGES_TO_CONSOLE = true;
#endif

#ifndef NDEBUG
const int NUMBER_OF_CLIENTS_EMULATED = 3;
const int MASSAGE_SLEEP_TIME = 1000;
const bool LOG_MASSAGES_TO_CONSOLE = true;
#endif


std::mt19937 gen;

int clientCount = 0;
bool lastConnectionSuccessful = false;

struct Client {
  ClientAPI API;
  int id = 0;
};

std::vector<Client *> clients;
std::mutex m;


void clientReceiverLoop(Client *currClient, int clientId) {
  std::string msg = " ";
  int err;
  while (true) {
    msg.assign(currClient->API.receiveMsg(&err));
    if (err <= 0) { break; }
    if (LOG_MASSAGES_TO_CONSOLE) {
      m.lock();
      std::cout << std::endl << "[ Client " << clientId << " ]<--" << msg << std::flush;
      if (NUMBER_OF_CLIENTS_EMULATED < 2) {
        std::cout << std::endl << "[ Client " << clientId << " ]--> " << std::flush;
      }
      m.unlock();
    }
  }
}

void clientInitLoop() {
  m.lock();
  Client *currentClient = new Client();
  currentClient->API.clearBuffer();
  clients.emplace_back(currentClient);
  currentClient->API.initConnection();
  //currentClient->API.setBufferSize(5000);
  lastConnectionSuccessful = true;
  currentClient->id = strtol(currentClient->API.receiveMsg(), nullptr, 0);
  currentClient->API.clearBuffer();
  clientCount++;

  //if (LOG_MASSAGES_TO_CONSOLE) {
  std::cout << "Client " << currentClient->id << " active" << std::endl;
//  }
  m.unlock();

  std::string massage = " ";
  std::thread clientReceiver(clientReceiverLoop, currentClient, currentClient->id);
  clientReceiver.detach();

  auto messageBuffer = new char[1024];

  int i = 0;
  while (true) {
    if (NUMBER_OF_CLIENTS_EMULATED < 2 && LOG_MASSAGES_TO_CONSOLE) {
      std::cout << "[ Client " << currentClient->id << " ]--> " << std::flush;
      std::cin.getline(messageBuffer, 1024);

      massage.assign(messageBuffer);
      //massage += '\0';
      //std::cout << std::endl << std::flush;
    } else {
      // m.lock();
      massage = "Rep ";
      massage += std::to_string(gen() % 100);
      massage += " # ";
      massage += std::to_string(i);
      //std::cout<<"[ Client " << currentClient->id << " ]"<<massage<<std::endl;
      i++;
      //m.unlock();
    }

    memcpy(messageBuffer, massage.c_str(), massage.size());

    currentClient->API.sendMsg(messageBuffer, massage.size());

    Sleep(MASSAGE_SLEEP_TIME);
  }

}

void connectionMaster(int count) {
  std::thread clientThread(clientInitLoop);
  clientThread.detach();
  --count;
  while (count) {
    if (lastConnectionSuccessful) {
      lastConnectionSuccessful = false;
      std::thread nextClientThread(clientInitLoop);
      nextClientThread.detach();
      --count;
    }
  }
}


int main() {
  gen.seed(time(0));


  std::thread connectionMasterLoop(connectionMaster, NUMBER_OF_CLIENTS_EMULATED);
  connectionMasterLoop.detach();

  while (1) {
    Sleep(10000);
  }
  return 0;
}
