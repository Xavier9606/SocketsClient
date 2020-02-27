#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <winsock2.h>
#include "ClientAPI.h"
#include <thread>
#include <mutex>

using namespace std;
mutex m;

ClientAPI client;


void clientThreadLoop(int n) {
  m.lock();

  cout << "Client " << n << " active" << endl;
   int i=0;
   string massage;
  while(true){
    massage="Client ";
    massage+=n;
    massage+=" says ";
    massage+=i;
    client.sendMsg((char*)massage.c_str());
    i++;
  }
  m.unlock();
}

void connect1() {
  client.initConnection();
  client.sendMsg("Hello Web!");
  cout << client.receiveMsg() << endl;
}

int main() {

  int threadCount = 2;

  thread first(connect1);
  first.join();

  std::vector<std::thread> v;
  for (int i = 0; i < threadCount; ++i) {
    v.emplace_back(clientThreadLoop, i);
  }



  return 0;
}
