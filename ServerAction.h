#ifndef _SERVERACTION_H
#define _SERVERACTION_H

#include <map>
#include <vector>

#include "ssTCPClient.h"
#include "helpers.h"
#include "DataUDP.h"

class ServerAction {
 public:
  /* Map client information to socket */
	std::map <int, ssTCPClient> clientMap;
  std::map <std::string, std::vector<DataUDP> > topicMap;

  void putClient(const char*, TCPClients&, int) ;

  bool putMsg(UdpMsg& myUdpMsg, struct sockaddr_in&) ;

  void subscribe(int, const char*) ;

  void unsubscribe(int, const char*);

  void sendMsg(const char*) ;
};

#endif
