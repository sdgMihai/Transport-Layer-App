#include <cstring>

#include "ServerAction.h"
#include "TcpMsg.h"
#include "UDPtoTCP.h"
#include "Print.h"

void ServerAction::putClient(const char* buffer, TCPClients& cli, int key) {
  TcpMsg tcpMsg;  // get id
  memcpy(&tcpMsg, buffer, BUFLEN - 1);

  ssTCPClient cpInfo(cli.addr.front(), tcpMsg.payload, true);
  cli.addr.pop_front();
  cli.sockfd.pop_front();
  for (auto& elem : clientMap[key].topics) {
    if (elem.second == true) {
      cpInfo.topics.push_back(elem);
    }
  }
  for (auto& cMsg : clientMap[key].savedMsgs) {
    send(key,
        (const char* )&cMsg, BUFLEN + sizeof(uint32_t) + 1 + sizeof(cMsg.addr)
        , 0);
  }

  clientMap[key] = cpInfo;
}

bool ServerAction::putMsg(UdpMsg& myUdpMsg, struct sockaddr_in& addr){
  DataUDP value(myUdpMsg, myUdpMsg.pSize, addr);
  std::string key(myUdpMsg.topic);

  bool sbStored = false; // should the msg be stored
  // - aka are clients subscribed to this msg topic
  if (clientMap.empty()) { // no clients no need to store msg's
    sbStored = false;
  } else {
    for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
      if (it->second.isSubscribed(key)){
        sbStored = true;
        break;
      }
    }
    
    if (sbStored == true) {
      topicMap[key].push_back(value);
      return true;
    }
  }
  return false;
}

void ServerAction::subscribe(int sockfd, const char* buffer) {
  TcpMsg tcpMsg;  // get topic to subscribe to
  memcpy(&tcpMsg, buffer, BUFLEN);

  clientMap[sockfd].cSubscribe(tcpMsg);
}

void ServerAction::unsubscribe(int sockfd, const char* buffer) {
  TcpMsg tcpMsg;  // get topic to subscribe to
  memcpy(&tcpMsg, buffer, BUFLEN - 1);

  clientMap[sockfd].cUnsubscribe(tcpMsg);
}

void ServerAction::sendMsg(const char* topic) {
  std::string key(topic);
  DataUDP& msg = topicMap[key][topicMap[key].size() - 1];
  /* msg to be sent to tcp clients */
  UDPtoTCP cMsg;

  /* retrieving data from UDP */
  int myInt;
  double shortReal;
  double myFloat;
  const char* payload;  // [BUFLEN - TOPIC_SIZE - sizeof(msg.data_type)];

  strcpy(cMsg.topic, topic);
  memcpy(&cMsg.addr, &msg.addr, sizeof(msg.addr));

	switch (msg.data_type) {
		case DataType::INT:
		myInt = msg.getInt();
    fflush(stdout);
    cMsg.setInt(myInt);
		break;
		case DataType::SHORT_REAL:
      shortReal = msg.getShortReal();
      cMsg.setShortReal(shortReal);
		break;
		case DataType::FLOAT:
      myFloat = msg.getFloat();
      cMsg.setFloat(myFloat);
		break;
		case DataType::STRING:
			payload = msg.getString();
      cMsg.setString(payload);
		break;
	default:
		fprintf(stderr, "getUDP payload for TCP\n");
		break;
	}

  cMsg.data_type = msg.data_type;
  cMsg.endOfMsg = 5;

  for (auto it = clientMap.begin(); it != clientMap.end(); ++it) {
    if (it->second.isSubscribed(key)) {
      if (it->second.isOnline) {
        send(it->first,
        (const char* )&cMsg, BUFLEN + sizeof(uint32_t) + 1 + sizeof(cMsg.addr)
        , 0);
      } else if (it->second.isSf(key)){
        it->second.savedMsgs.push_back(cMsg);
      }
    } 
  }
  
}