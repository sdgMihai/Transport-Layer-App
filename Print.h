#ifndef _PRINT_H
#define _PRINT_H 4

#include "UdpMsg.h"
#include "DataUDP.h"
#include "TcpMsg.h"
#include "ServerAction.h"
#include "UDPtoTCP.h"
#include <vector>

class Print {
 public:
	static void printUdpMsg(UdpMsg&) ;

	void readyToRead() ;

	void printTcpMsg(TcpMsg&) ;

	void bytesRead(char*, int) ;

	static void printTopicLine(std::vector<DataUDP>&
	, const std::string&) ;

	static void topicMap(std::map<std::string, std::vector<DataUDP>>&) ;

	void newTCPConnection(ServerAction&, int) ;

	static void printUDPtoTCP(const UDPtoTCP&) ;

	static void printssClient(const ssTCPClient&);
};

#endif