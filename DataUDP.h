#ifndef _DATAUDP_H
#define _DATAUDP_H 3

#include "helpers.h"
#include "UdpMsg.h"

class __attribute__((packed)) DataUDP
{
 public:
	char data_type;
	char payload[BUFLEN - TOPIC_SIZE - sizeof(data_type)];
	int pSize;
	struct sockaddr_in addr;

	DataUDP() ;

	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	DataUDP(const UdpMsg&, int, struct sockaddr_in&) ;
	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	DataUDP(const DataUDP&, int, struct sockaddr_in&) ;

	uint8_t getDataType() ;

	uint32_t getInt() ;

	double getShortReal() ;

	double getFloat() ;

	const char* getString();

  /*DataUDP& operator=(const DataUDP&) ;*/
};

#endif
