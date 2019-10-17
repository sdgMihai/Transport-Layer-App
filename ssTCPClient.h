#ifndef _SSTCPCLIENT_H
#define _SSTCPCLIENT_H
#include <netinet/in.h>
#include <algorithm>
#include <vector>
#include <string>

#include "TcpMsg.h"
#include "UDPtoTCP.h"

// utilized as value in a map in server
class ssTCPClient {  // server side
 public:
	struct sockaddr_in addr;
	char id[11];
	std::vector<std::pair<std::string, bool >> topics;  // topics and sf
	std::vector<UDPtoTCP> savedMsgs;
	bool isOnline;
	// sockfd is key, so is redundant to keep it here

	ssTCPClient(const struct sockaddr_in, char*, bool) ;

	ssTCPClient() ;

	ssTCPClient(const ssTCPClient&) ;

  /* A client subscribes to a topic */
  void cSubscribe(TcpMsg&) ;

	void cUnsubscribe(TcpMsg&);

	bool isSubscribed(const char* topic) ;

	bool isSubscribed(std::string key) ;

	bool isSf(std::string&);
};

#endif