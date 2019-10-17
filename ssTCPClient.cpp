#include <cstring>

#include "ssTCPClient.h"
#include <string>

ssTCPClient::ssTCPClient(const struct sockaddr_in addr1, char* id, bool isOnline) {
  memcpy(&addr, &addr1, sizeof(addr));
  strcpy(this->id, id);
  this->isOnline = isOnline;
}

ssTCPClient::ssTCPClient() {
  isOnline = 0;
}

ssTCPClient::ssTCPClient(const ssTCPClient& cp) {
  memcpy(&(this->addr), &(cp.addr), sizeof(addr));
  strcpy(this->id, cp.id);
  this->topics = cp.topics;
  this->savedMsgs = cp.savedMsgs;
  isOnline = cp.isOnline;
}

void ssTCPClient::cSubscribe(TcpMsg& tcpMsg) {
  std::string topic(tcpMsg.payload);
  topics.push_back(make_pair(topic, tcpMsg.sf));
}

void ssTCPClient::cUnsubscribe(TcpMsg& tcpMsg) {
  std::string topic(tcpMsg.payload);
  for (auto it = topics.begin(); it != topics.end(); it++) {
    if (topic == it->first) {
      topics.erase(it);
      return;
    }
  }
}

bool ssTCPClient::isSubscribed(const char* topic) {
  std::string key(topic);
  for (auto it = topics.begin(); it != topics.end(); it++) {
    if (key == it->first) {
      return true;
    }
  }
  return false;
}

bool ssTCPClient::isSubscribed(std::string key) {
  for (auto it = topics.begin(); it != topics.end(); it++) {
    if (key == it->first) {
      return true;
    }
  }
  return false;
}

bool ssTCPClient::isSf(std::string& key) {
  for (auto it = topics.begin(); it != topics.end(); it++) {
    if (key == it->first) {
      return (it->second == true);
    }
  }
  return false;
}
