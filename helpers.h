#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <cstring>
#include <utility>
#include <list>

/*
 * Macro de verificare a erorilor
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 */

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

#define BUFLEN		1552	// max data size of a message 1500 + 1 + 50 + 1 for \0 or safety 
#define MAX_CLIENTS	50	// maximum number of clients waiting in list
#define TOPIC_SIZE 50  // the msg topic, both udp and tcp

enum DataType {
	INT = 0,
	SHORT_REAL = 1,
	FLOAT = 2,
	STRING = 3
};

class TCPClients {
 public:
	std::list<struct sockaddr_in> addr;
	std::list<int> sockfd;
};

/* here were dataUdp and UdpMsg and tcpMsg in the first place */

/* aicia fost clasa print */

#endif
