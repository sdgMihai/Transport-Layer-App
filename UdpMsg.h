#ifndef _UDPMSG_H
#define _UDPMSG_H 2

#include "helpers.h"

class DataUDP;

/* TODO : CE FAC atunci cand trimit pachetul ca va avea mai mult de 1552
* bytes deci tcp il va sparge in bucati
*/
class __attribute__((packed)) UdpMsg {
 public:
	char topic[TOPIC_SIZE];
	char data_type;
	char payload[BUFLEN - TOPIC_SIZE - sizeof(data_type)];
	int pSize;

	UdpMsg() ;

	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	UdpMsg(const UdpMsg&, int) ;

	/*
	*	Before using this constructor the cp param
	* !!must!! have '\0' at the end of the payload.
	*/
	UdpMsg(const char* , const DataUDP&, int) ;

	uint8_t getDataType() ;

	uint32_t getInt() ;

	double getShortReal() ;

	double getFloat() ;

	const char* getString();

};

#endif
