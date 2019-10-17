#include <cmath>
#include "Print.h"

void Print::printUdpMsg(UdpMsg& msg){
	printf("\ntopic : %s data type : ", msg.topic);

	uint32_t* intP;  // for DataType::INT and DataType::FLOAT value
	uint32_t resInt;
	uint16_t* shortP;  // for DataType::SHORT_REAL
	uint8_t* decP;  // for DataType::FLOAT
	uint8_t decPow;
	double resShort;  // result get from short, BUT type is double
	double resFloat;

	switch (msg.data_type) {
		case DataType::INT:
		/*
		* get from network-byte order a byte coding sign
		* and a uint32_t keeping number.
		*/
			printf("INT ");
			resInt = (msg.payload[0] == 0 ? 1 : -1);  // sign decode
			intP = (uint32_t* )(msg.payload + 1);
			resInt *= ntohl(*intP);
			printf("number : %d\n", resInt);
		break;
		case DataType::SHORT_REAL:
		/*
		* get from network-byte order a short value
		* that is multiplied by 100.
		*/
			printf("SHORT_REAL ");
			shortP = (uint16_t* )(msg.payload);
			resShort = htons(*shortP) / 100.;
			printf("number : %f\n", resShort);
		break;
		case DataType::FLOAT:
		/*
		* get from network-byte order a byte coding sign
		* and a uint32_t keeping number
		* and a uint8_t keeping the negative module of the 10'th power
		* the uint32_t value must be multiplied by to get
		* the float result.
		*/
			printf("FLOAT ");
			resInt = (msg.payload[0] == 0 ? 1 : -1);
			intP = (uint32_t* )(msg.payload + 1);
			resInt *= htonl(*intP);
			decP = (uint8_t* )(msg.payload + 5);
			decPow = *decP;
			// int cast ensures that the value will be converted as result
			// to double not the bytes.
			resFloat = (double)((int)resInt / (double)pow(10, decPow));
			printf("number : %f\n", resFloat);
		break;
		case DataType::STRING:
			printf("STRING ");
			printf("payload : %s\n", msg.payload);
		break;
	default:
		fprintf(stderr, "error print\n");
		break;
	}
}

void Print::readyToRead() {
	printf("\nCLIENT: Citesc de la tastatura:");  // reminder for client
	fflush(stdout);  // so he doesn't forget he can write
}

void Print::printTcpMsg(TcpMsg& msg) {
	printf("TODO");
}

void Print::bytesRead(char* s, int n) {
	printf("Mesajul contine %lu bytes + 1(null) : and n is %d\n"
	, (unsigned long)strlen(s), n);
}

void Print::printTopicLine(std::vector<DataUDP>& value
, const std::string& key) {
	for(auto it = value.begin(); it != value.end(); ++it) {
		UdpMsg msg(key.c_str(), *it, (*it).pSize);
		printUdpMsg(msg);
	}
}

void Print::topicMap(std::map<std::string, std::vector<DataUDP> >& tMap) {
	if (tMap.empty() != 0) {
		puts("topicMap is Empty\n");
	}
	for (auto it = tMap.begin(); it!= tMap.end(); ++it) {
		printTopicLine(tMap[it->first], (const std::string) it->first);
	}
}

void Print::newTCPConnection(ServerAction& sAction, int key) {
	printf("New client %s connected from %s:%d\n"
	, sAction.clientMap[key].id
	, inet_ntoa(sAction.clientMap[key].addr.sin_addr)
	, ntohs(sAction.clientMap[key].addr.sin_port));
}

void Print::printUDPtoTCP(const UDPtoTCP& msg) {
	printf("%s:%d - "
	, inet_ntoa(msg.addr.sin_addr)
	, ntohs(msg.addr.sin_port));
	printf("%s - ", msg.topic);
	switch (msg.data_type) {
		case DataType::INT:
			printf("INT - %d", msg.getInt());
			break;
		case DataType::SHORT_REAL:
			printf("SHORT_REAL - %f", msg.getShortReal());
			break;
		case DataType::FLOAT:
			printf("FLOAT - %f", msg.getFloat());
			break;
		case DataType::STRING:
			printf("STRING - %s", msg.getString());
			break;
		default :
			fprintf(stderr, "abnormal use of Print::printUDPtoTCP\n");
	}
	puts("");
}

void Print::printssClient(const ssTCPClient& client) {
	printf("printing client %s", client.id);
	for (auto it = client.topics.begin(); it != client.topics.end(); ++it) {
		printf("topics: %s ", (*it).first.c_str());
	}
	printf("\n");
}