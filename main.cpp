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
const int NUMBER_OF_CLIENTS_EMULATED = 2;
const int MASSAGE_SLEEP_TIME = 0;
const bool LOG_MASSAGES_TO_CONSOLE = false;
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
    msg = currClient->API.receiveMsg(&err);
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
  clients.push_back(new Client);
  clients[clientCount]->API.initConnection();
  lastConnectionSuccessful = true;
  clients[clientCount]->id = strtol(clients[clientCount]->API.receiveMsg(), nullptr, 0);
  Client *currentClient = clients[clientCount];
  clientCount++;

  //if (LOG_MASSAGES_TO_CONSOLE) {
  std::cout << "Client " << currentClient->id << " active" << std::endl;
//  }
  m.unlock();

  std::string massage = " ";
  std::thread clientReceiver(clientReceiverLoop, currentClient, currentClient->id);
  clientReceiver.detach();

  int i = 0;
  while (true) {
    if (NUMBER_OF_CLIENTS_EMULATED < 2 && LOG_MASSAGES_TO_CONSOLE) {
      std::cout << "[ Client " << currentClient->id << " ]--> " << std::flush;
      std::cin.getline((char *) massage.c_str(), 1024);
      massage += '\0';
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
    currentClient->API.sendMsg((char *) massage.c_str());

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
