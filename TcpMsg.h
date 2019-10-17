#ifndef _TCPMSG_H
#define _TCPMSG_H 5
#include "helpers.h"

class TcpMsg {
 public:
	char topic[TOPIC_SIZE];
	char payload[BUFLEN - TOPIC_SIZE - 1];
	char sf;
};

#endif